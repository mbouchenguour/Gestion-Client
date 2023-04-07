#include "bdd.h"
#include "gestionait.h"

#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QFileDialog>
#include <QtSql/QSqlError>
#include <QJsonArray>

BDD *BDD::instance = nullptr;

/**
* @author Mohamed
* @date decembre 2022
* @brief implémentation du constructeur
*/
BDD::BDD()
{

}


/**
 * @author Mohamed
 * @date decembre 2022
 * @brief accesseur "instance", l'initialise s'il ne l'est pas
 * @return l'instance du singleton qui gère la bdd
 */
BDD* BDD::getInstance(){
    if (instance == nullptr)
        instance = new BDD();

    return instance;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief accesseur "db" et ouvre la bdd
 * @param chemin : la chaîne de caractères std::string (const) qui contient le chemin du fihcier sqlite
 */
void BDD::setDb(const std::string & chemin)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    //attention à avoir placer le fichier dans le répertoire /tmp
    db.setDatabaseName(QString::fromStdString(chemin));
    if(!db.open())
      qDebug() << "Pas de connexion BDD !";
    else{
        qDebug() << "Connexion BDD réussi!";
        db.exec("PRAGMA foreign_keys=ON");
    }


}

QSqlDatabase BDD::getDb() const
{
    return this->db;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Récupère la liste des contacts avec leurs interactions dans la bdd, ajoute les contacts dans une gestion de contacts
 * @return Instance GestionContact qui contient la liste des contacts
 */
GestionContact* BDD::getContacts(){
    GestionContact * gestion;
    Contact * contact;
    GestionDate * dateCreation;
    GestionDate * dateModification;
    GestionDate * dateAjoutI;
    GestionDate * dateAjoutT;
    QStringList lDateS;
    QStringList listDateCreation;
    QStringList listDateModification;
    QStringList listDateAjoutI;
    QStringList listDateAjoutT;
    std::list<unsigned> telephone;

    //Récuperation derniere date de suppression
    QSqlQuery queryDateS("SELECT * from dateSuppression");
    if(!queryDateS.exec())
        qDebug() << "Impossible d'exécuter la requête !";
    else
    {
      qDebug() << "Requête exécutée";
      if(queryDateS.next()){
          if(!queryDateS.value(0).isNull()){
              lDateS = queryDateS.value(0).toString().split("-");
              gestion = new GestionContact(new GestionDate(lDateS.at(2).toUInt(), lDateS.at(1).toUInt(), lDateS.at(0).toUInt()));
          }
          else
              gestion = new GestionContact(nullptr);
      }
    }


    QSqlQuery query("SELECT * from Contact");
    if(!query.exec())
        qDebug() << "Impossible d'exécuter la requête !";
    else
    {
      qDebug() << "Requête exécutée";
      while(query.next())
      {
          listDateCreation = query.value(7).toString().split("-");

          if(!query.value(8).toString().isEmpty()){
              listDateModification = query.value(8).toString().split("-");
              dateModification = new GestionDate(listDateModification.at(2).toUInt(), listDateModification.at(1).toUInt(), listDateModification.at(0).toUInt());
          }
          else
              dateModification = nullptr;


          for(char i : query.value(5).toString().toStdString()){
              telephone.push_back(int(i) - 48);
          }
          dateCreation = new GestionDate(listDateCreation.at(2).toUInt(), listDateCreation.at(1).toUInt(), listDateCreation.at(0).toUInt());

          contact = new Contact(query.value(0).toUInt(), query.value(1).toString().toStdString(), query.value(2).toString().toStdString(), query.value(3).toString().toStdString(), query.value(4).toString().toStdString(), telephone, query.value(6).toString().toStdString(), dateCreation, dateModification);
          telephone.clear();
          gestion->addContact(contact);

          QSqlQuery query2;
          query2.prepare("SELECT * FROM Interaction WHERE idContact = :idContact");
          query2.bindValue(":idContact", contact->getIdContact());
          if(!query2.exec())
              qDebug() << "Impossible d'exécuter la requête !";
          else{
              qDebug() << "Requête exécutée";
              while(query2.next()){
                  listDateAjoutI = query2.value(2).toString().split("-");
                  dateAjoutI = new GestionDate(listDateAjoutI.at(2).toUInt(), listDateAjoutI.at(1).toUInt(), listDateAjoutI.at(0).toUInt());
                  Interaction * i = new Interaction(query2.value(0).toUInt(), query2.value(1).toString().toStdString(), dateAjoutI);
                  contact->getGestionI()->addInteraction(*i);

                  QSqlQuery query3;
                  query3.prepare("SELECT * from Todo NATURAL JOIN AssociationInteractionTodo WHERE idInteraction = :idInteraction;");
                  query3.bindValue(":idInteraction", i->getIdInteraction());
                  if(!query3.exec()){
                      qDebug() << "Impossible d'exécuter la requête !";
                  }
                  else{

                      while(query3.next()){
                          listDateAjoutT = query3.value(2).toString().split("-");
                          dateAjoutT = new GestionDate(listDateAjoutT.at(2).toUInt(), listDateAjoutT.at(1).toUInt(), listDateAjoutT.at(0).toUInt());
                          Todo * t = new Todo(query3.value(0).toUInt(), query3.value(1).toString().toStdString(), dateAjoutT);
                          AssociationInteractionTodo * a = new AssociationInteractionTodo(query3.value(3).toUInt(), t, i);
                          GestionAIT::getInstance()->addAssociation(*a);
                      }
                  }
              }

          }
    }
}



    return gestion;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Ajoute un contact à la bdd
 * @param "c" : l'instance Contact (const) à ajouter dans la bdd
 */
unsigned BDD::ajoutContact(Contact * const c) const{
    unsigned id = 0;
    QSqlQuery query;
    query.prepare("INSERT INTO Contact(nom, prenom, entreprise, mail, telephone, photo, dateCreation) VALUES ( :nom, :prenom, :entreprise, :mail, :telephone, :photo, :dateCreation)");
    query.bindValue(":nom", QString::fromStdString(c->getNom()));
    query.bindValue(":prenom", QString::fromStdString(c->getPrenom()));
    query.bindValue(":entreprise", QString::fromStdString(c->getEntreprise()));
    query.bindValue(":mail", QString::fromStdString(c->getMail()));
    query.bindValue(":telephone", QString::fromStdString(c->getTelephoneString()));
    query.bindValue(":photo", QString::fromStdString(c->getPhoto()));
    query.bindValue(":dateCreation", QString::fromStdString(c->getDateCreation()->getDateBDD()));
    if(!query.exec())
      qDebug() << "Impossible d'exécuter la requête !";
    else{
      qDebug() << "Requête exécutée";
      id = query.lastInsertId().toUInt();
    }
    return id;
}


/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Modifie un contact de la bdd
 * @param "c" : l'instance Contact (const) à modifier dans la bdd
 */
void BDD::modifierContact(Contact * const c){
    QSqlQuery query;
    query.prepare("UPDATE Contact SET nom = :nom, prenom = :prenom, entreprise = :entreprise, mail = :mail, telephone = :telephone, photo = :photo, dateModification = :dateModification WHERE idContact = :idContact");

    query.bindValue(":nom", QString::fromStdString(c->getNom()));
    query.bindValue(":prenom", QString::fromStdString(c->getPrenom()));
    query.bindValue(":entreprise", QString::fromStdString(c->getEntreprise()));
    query.bindValue(":mail", QString::fromStdString(c->getMail()));
    query.bindValue(":telephone", QString::fromStdString(c->getTelephoneString()));
    query.bindValue(":photo", QString::fromStdString(c->getPhoto()));
    query.bindValue(":dateModification", QString::fromStdString(c->getDateModification()->getDateBDD()));
    query.bindValue(":idContact", c->getIdContact());
    if(!query.exec())
      qDebug() << "Impossible d'exécuter la requête !";
    else
      qDebug() << "Requête exécutée";
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Supprime un contact de la bdd
 * @param id : l'entier non signé (const) correspondant à l'id du contact à supprimer
 */
void BDD::supprimerContact(const unsigned id){
    QSqlQuery query;
    query.prepare("DELETE FROM Contact where idContact = :idContact");
    query.bindValue(":idContact", id);

    if(!query.exec())
    {
      qDebug() << "Impossible d'exécuter la requête !";
    }
    else
    {
      qDebug() << "Requête exécutée";
    }
}


/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Ajoute une Interaction dans la bdd
 * @param i : pointeur de l'Interaction (const) à ajouter dans la bdd
 * @param idContact : l'entier non signé (const) correspondant à l'id du contact a qui on ajoute une interaction
 * @return unsigned : l'id de l'interaction que l'on vient d'ajouter
 */
unsigned BDD::ajouterInteraction(Interaction * const i, const unsigned idContact) const{
    unsigned idInteraction = 0;
    QSqlQuery query;
    query.prepare("INSERT INTO Interaction(contenuInteraction, dateAjout, idContact) values ( :contenueInteraction, :dateAjout, :idContact )");

    query.bindValue(":contenueInteraction", QString::fromStdString(i->getContenuInteraction()));
    query.bindValue(":dateAjout", QString::fromStdString(i->getDateAjout()->getDateBDD()));
    query.bindValue(":idContact", idContact);

    if(!query.exec())
      qDebug() << "Impossible d'exécuter la requête !";
    else{
      qDebug() << "Requête exécutée";
      idInteraction = query.lastInsertId().toUInt();
    }
    return idInteraction;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Ajoute un Todo dans la bdd
 * @param "t" : pointeur du Todo (const) à ajouter dans la bdd
 * @return unsigned : l'id du Todo que l'on vient d'ajouter
 */
unsigned BDD::ajouterTodo(Todo * const t) const{
    unsigned idTodo = 0;
    QSqlQuery query;
    query.prepare("INSERT INTO Todo(contenuTodo, dateRealisation) values ( :contenuTodo, :dateRealisation )");

    query.bindValue(":contenuTodo", QString::fromStdString(t->getContenuTodo()));
    query.bindValue(":dateRealisation", QString::fromStdString(t->getDateRealisation()->getDateBDD()));

    if(!query.exec())
      qDebug() << "Impossible d'exécuter la requête !";
    else{
      qDebug() << "Requête exécutée";
      idTodo = query.lastInsertId().toUInt();
    }
    return idTodo;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Ajoute une Association dans la bdd
 * @param "association" : pointeur de l'Association (const) à ajouter dans la bdd
 * @return unsigned : l'id de l'association que l'on vient d'ajouter
 */
unsigned BDD::ajouterAssociation(AssociationInteractionTodo * const association) const{
    unsigned idAssociation = 0;
    QSqlQuery query;
    query.prepare("INSERT INTO AssociationInteractionTodo(idTodo, idInteraction) values ( :idTodo, :idInteraction )");

    query.bindValue(":idTodo", association->getTodo()->getIdTodo());
    query.bindValue(":idInteraction", association->getInteraction()->getIdInteraction());

    if(!query.exec())
      qDebug() << "Impossible d'exécuter la requête !";
    else{
      qDebug() << "Requête exécutée";
      idAssociation = query.lastInsertId().toUInt();
    }
    return idAssociation;
}



/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Récupère les Contact de la bdd et les convertie en format Json
 * @return QJsonArray : l'instance qui contient le tableau des Contact en format Json
 */
QJsonArray BDD::contactsToJson() const
{
    QJsonArray tableau;
    QSqlQuery query("SELECT * from Contact");
    if(!query.exec())
        qDebug() << "Impossible d'exécuter la requête !";
    else{
        qDebug() << "Requête exécutée";
        while(query.next()){
            QVariantMap map;
            map.insert("idContact", query.value(0));
            map.insert("Nom", query.value(1));
            map.insert("Prenom", query.value(2));
            map.insert("Entreprise", query.value(3));
            map.insert("Mail", query.value(4));
            map.insert("Telephone", query.value(5));
            map.insert("Photo", query.value(6));
            map.insert("dateCreation", query.value(7));
            if(!query.value(8).isNull())
                map.insert("dateModification", query.value(8));
            else
                map.insert("dateModification", QJsonValue());
            tableau.push_back(QJsonObject::fromVariantMap(map));
        }
    }
    return tableau;
}


/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Récupère les interactions de la bdd et les convertie en format Json
 * @return QJsonArray : l'instance qui contient le tableau des interactions en format Json
 */
QJsonArray BDD::interactionsToJson() const
{
    QJsonArray tableau;
    QSqlQuery query("SELECT * from Interaction");
    if(!query.exec())
        qDebug() << "Impossible d'exécuter la requête !";
    else{
        qDebug() << "Requête exécutée";
        while(query.next()){
          QVariantMap map;
          map.insert("idInteraction", query.value(0));
          map.insert("contenuInteraction", query.value(1));
          map.insert("dateAjout", query.value(2));
          map.insert("idContact", query.value(3));
          tableau.push_back(QJsonObject::fromVariantMap(map));
        }
    }
    return tableau;
}



/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Récupère les Todo de la bdd et les convertie en format Json
 * @return QJsonArray : l'instance qui contient le tableau des Todo en format Json
 */
QJsonArray BDD::todosToJson() const
{
    QJsonArray tableau;
    QSqlQuery query("SELECT * from Todo");
    if(!query.exec())
        qDebug() << "Impossible d'exécuter la requête !";
    else{
        qDebug() << "Requête exécutée";
        while(query.next()){
          QVariantMap map;
          map.insert("idTodo", query.value(0));
          map.insert("contenuTodo", query.value(1));
          map.insert("dateRealisation", query.value(2));
          tableau.push_back(QJsonObject::fromVariantMap(map));
        }
    }
    return tableau;
}



/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Récupère les AssociationInteractionTodo de la bdd et les convertie en format Json
 * @return QJsonArray : l'instance qui contient le tableau des AssociationInteractionTodo en format Json
 */
QJsonArray BDD::associationsToJson() const
{
    QJsonArray tableau;
    QSqlQuery query("SELECT * from AssociationInteractionTodo");
    if(!query.exec())
        qDebug() << "Impossible d'exécuter la requête !";
    else{
        qDebug() << "Requête exécutée";
        while(query.next()){
          QVariantMap map;
          map.insert("idAssociation", query.value(0));
          map.insert("idTodo", query.value(1));
          map.insert("idInteraction", query.value(2));
          tableau.push_back(QJsonObject::fromVariantMap(map));
        }
    }
    return tableau;
}








