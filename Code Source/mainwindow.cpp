#include "mainwindow.h"
#include "bdd.h"
#include "ui_mainwindow.h"
#include "gestioncontact.h"
#include "formajoutcontact.h"
#include "gestionait.h"
#include <QtDebug>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QMessageBox>
#include <QBitmap>
#include <QDate>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implémentation du constructeur
 * Initialise les différents widgets
 * Récupère les contacts de la bdd
 * Connecte les différents bouttons à des slots
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString chemin = QFileDialog::getOpenFileName(this, tr("Ajouter une base de donées"), QString(), "Sqlite (*.sqlite)");

    if(!chemin.isEmpty()){
        //Initialisation de la bdd et recuperation des contacts
        BDD::getInstance()->setDb(chemin.toStdString());
		
		if(BDD::getInstance()->getDb().isOpen()){
			f = new FormAjoutContact();
			edit = new EditContact();
			rInteractions = new RechercheInteractions();
			rTodos = new RechercheTodos();
			
			gestionC = BDD::getInstance()->getContacts();

			remplirTable(gestionC);

			ui->tableContact->setEditTriggers(QAbstractItemView::NoEditTriggers);
			ui->tableContact->verticalHeader()->hide();
			ui->tableContact->setSelectionBehavior(QAbstractItemView::SelectRows);

			ui->menubar->addAction(ui->actionAjouter);
			ui->menubar->addAction(ui->actionInteractions);
			ui->menubar->addAction(ui->actionTodos);
			ui->menubar->addAction(ui->actionExport);

			connect(ui->actionAjouter, SIGNAL(triggered(bool)), this, SLOT(ouvertureForm()));
			connect(ui->actionInteractions, SIGNAL(triggered(bool)), this, SLOT(ouvertureInteractions()));
			connect(ui->actionTodos, SIGNAL(triggered(bool)), this, SLOT(ouvertureTodos()));
			connect(ui->actionExport, SIGNAL(triggered(bool)), this, SLOT(exportJson()));

			connect(f, SIGNAL(ajouterContact(Contact *)), this, SLOT(ajouterContact(Contact *)));

			connect(this, SIGNAL(envoieEdit(Contact *)), edit, SLOT(initialiser(Contact *)));
			connect(this, SIGNAL(envoieInteractions(GestionContact *)), rInteractions, SLOT(initialiser(GestionContact *)));
			connect(this, SIGNAL(envoieTodos()), rTodos, SLOT(ouverture()));

			connect(edit, SIGNAL(contactModifier(Contact *)), this, SLOT(majContact(Contact *)));
			connect(ui->informationButton, SIGNAL(clicked()), this, SLOT(editContact()));
			connect(ui->supprimerButton, SIGNAL(clicked()), this, SLOT(deleteButton()));
			connect(ui->rechercheButton, SIGNAL(clicked()), this, SLOT(rechercheContacts()));
			connect(ui->dCreationCheckBox, SIGNAL(clicked()), this, SLOT(dateCreationCheckB()));
			connect(ui->dIntervalleCheckBox, SIGNAL(clicked()), this, SLOT(dateIntervalleCheckB()));
			connect(ui->supprimerFiltreButton, SIGNAL(clicked()), this, SLOT(resetRecherche()));
			connect(ui->tableContact->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(majInfo(QModelIndex, QModelIndex)));

			ui->dateCreationEdit->setDate(QDate::currentDate());
			ui->dateCreationEdit->setMaximumDate(QDate::currentDate());
			ui->dateDebutI->setDate(QDate::currentDate());
			ui->dateDebutI->setMaximumDate(QDate::currentDate());
			ui->dateFinI->setDate(QDate::currentDate());
			ui->dateFinI->setMaximumDate(QDate::currentDate());
			ui->tableContact->selectRow(0);

			ui->tableContact->setColumnHidden(0, true);
			ui->tableContact->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
			ui->tableContact->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
			ui->tableContact->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
			if(gestionC->getNbContact() == 0){
				ui->informationButton->setEnabled(false);
				ui->supprimerButton->setEnabled(false);
			}

			//On active le trie
			ui->tableContact->setSortingEnabled(true);
			ui->tableContact->horizontalHeader()->sortIndicatorOrder();

			if(gestionC->getDateSuppression())
				ui->lDateS->setText("Dernière date de suppression : " + QString::fromStdString(gestionC->getDateSuppression()->getDateString()));
			else
				ui->lDateS->setText("Pas de contact supprimé");

			// On change la couleur des messages d'erreurs en rouge
			QPalette rouge;
			rouge.setColor(QPalette::WindowText, Qt::red);
			ui->erreurLabel->setPalette(rouge);

			//On fixe la taille de la fenêtre
			this->setFixedSize(this->geometry().width(), this->geometry().height());
		}
			
		
    }
    else{
        qDebug() << "Echec BDD";
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief destructeur
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief slot executé lors du clique du bouton ajouter du menu bar, ouvre le formulaire d'ajout de contact
 */
void MainWindow::ouvertureForm(){
    f->show();
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Envoie un signal au wudget rechercheInteractions qui ouvre la recherche des interactions de tous les contacts
 * Slot executé lors du clique du bouton interactions du menu bar
 * Pas de paramètre
 */
void MainWindow::ouvertureInteractions(){
    emit envoieInteractions(this->gestionC);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief slot executé lors du clique du bouton todos du menu bar, ouvre la recherche des Todos de tous les contacts
 */
void MainWindow::ouvertureTodos(){
    emit envoieTodos();
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief slot executé lors de l'ajout d'un contact, signal envoyé par le formulaire d'ajout de contact
 */
void MainWindow::ajouterContact(Contact * c){
    ui->tableContact->setSortingEnabled(false);

    gestionC->addContact(c);
    int nb = ui->tableContact->rowCount();
    ui->tableContact->insertRow(nb);
    QTableWidgetItem * id = new QTableWidgetItem(QString::number(c->getIdContact()));
    QTableWidgetItem * nom = new QTableWidgetItem(QString::fromStdString(c->getNom()));
    QTableWidgetItem * entreprise = new QTableWidgetItem(QString::fromStdString(c->getEntreprise()));

    QTableWidgetItem * dateCreation = new QTableWidgetItem();
    dateCreation->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(c->getDateCreation()->getDateString()), "dd/MM/yyyy"));

    ui->tableContact->setItem(nb,0,id);
    ui->tableContact->setItem(nb,1,nom);
    ui->tableContact->setItem(nb,2,entreprise);
    ui->tableContact->setItem(nb,3,dateCreation);
    ui->nbContactsInfo->setText(QString::number(ui->tableContact->rowCount()));
    ui->informationButton->setEnabled(true);
    ui->supprimerButton->setEnabled(true);

    ui->tableContact->setSortingEnabled(true);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief remplie la table des contacts
 * @param gestionC : le pointeur de GestionContact (const) qui contient la liste des contacts à ajouter dans la table
 */
void MainWindow::remplirTable(GestionContact * const gestionC){
    ui->tableContact->setSortingEnabled(false);
    disconnect(ui->tableContact->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(majInfo(QModelIndex, QModelIndex)));

    ui->tableContact->clearContents();
    ui->tableContact->setRowCount(0);

    connect(ui->tableContact->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(majInfo(QModelIndex, QModelIndex)));

    if(gestionC->getNbContact() == 0)
        suppressionInfo();
    else{
        for (Contact * c : gestionC->getContacts()){
            int nb = ui->tableContact->rowCount();
            ui->tableContact->insertRow(nb);
            QTableWidgetItem * id = new QTableWidgetItem(QString::number(c->getIdContact()));
            QTableWidgetItem * nom = new QTableWidgetItem(QString::fromStdString(c->getNom()));
            QTableWidgetItem * entreprise = new QTableWidgetItem(QString::fromStdString(c->getEntreprise()));

            QTableWidgetItem * dateCreation = new QTableWidgetItem();
            dateCreation->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(c->getDateCreation()->getDateString()), "dd/MM/yyyy"));

            ui->tableContact->setItem(nb,0,id);
            ui->tableContact->setItem(nb,1,nom);
            ui->tableContact->setItem(nb,2,entreprise);
            ui->tableContact->setItem(nb,3,dateCreation);
            ui->informationButton->setEnabled(true);
            ui->supprimerButton->setEnabled(true);
        }
    }


    ui->nbContactsInfo->setText(QString::number(ui->tableContact->rowCount()));


    ui->tableContact->selectRow(0);
    ui->tableContact->setSortingEnabled(true);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Supprime les informations des labels et désactive les bouttons
 */
void MainWindow::suppressionInfo()
{
    ui->idInfo->setText("");
    ui->nomInfo->setText("");
    ui->prenomInfo->setText("");
    ui->mailInfo->setText("");
    ui->telephoneInfo->setText("");
    ui->dCreationInfo->setText("");
    ui->entrepriseInfo->setText("");
    ui->dModifInfo->setText("");
    ui->PhotoLabel->clear();
    ui->informationButton->setEnabled(false);
    ui->supprimerButton->setEnabled(false);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief slot executé lors du clique du boutton information d'un contact
 * envoie le signal envoieEdit au editContact qui permet de modifier un contact et accéder à ses interactions
 * Enregistre la ligne en cours de modification pour éviter
 */
void MainWindow::editContact(){
    int row = ui->tableContact->currentRow();
    this->ligne = row;
    unsigned id = ui->tableContact->model()->index(row,0).data().toUInt();
    Contact * c = this->gestionC->getById(id);
    emit envoieEdit(c);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief slot executé lors du clique du boutton supprimer
 * Supprime un contact de la bdd
 */
void MainWindow::deleteButton(){
    QMessageBox::StandardButton reponse;
    reponse = QMessageBox::question(this, "Suppression", "Etes-vous sûr de supprimer ce contact", QMessageBox::Yes|QMessageBox::No);
    if (reponse == QMessageBox::Yes) {
        ui->tableContact->setSortingEnabled(false);
        disconnect(ui->tableContact->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(majInfo(QModelIndex, QModelIndex)));

        int row = ui->tableContact->currentRow();
        unsigned id = ui->tableContact->model()->index(row,0).data().toUInt();
        GestionAIT::getInstance()->removeAssociationsByInteractions(this->gestionC->getById(id)->getGestionI()->getInteractions());
        this->gestionC->removeByIdContact(id);
        ui->tableContact->removeRow(row);
        BDD::getInstance()->supprimerContact(id);
        ui->nbContactsInfo->setText(QString::number(ui->tableContact->rowCount()));

        ui->lDateS->setText("Dernière date de suppression : " + QString::fromStdString(this->gestionC->getDateSuppression()->getDateString()));

        connect(ui->tableContact->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(majInfo(QModelIndex, QModelIndex)));

        if(ui->tableContact->rowCount() == 0)
            suppressionInfo();
        else
            ui->tableContact->selectRow(0);
        ui->tableContact->setSortingEnabled(true);
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief slot executé lors de la modification d'un contact lors du clique du boutton modifier du widget editContact
 * Met à jour la ligne du tableau correspondant au contact modifié
 * @param c : le pointeur du contact (const) que l'on souhaite modifier
 */
void MainWindow::majContact(Contact * c){
    QTableWidgetItem * nom = new QTableWidgetItem(QString::fromStdString(c->getNom()));
    QTableWidgetItem * entreprise = new QTableWidgetItem(QString::fromStdString(c->getEntreprise()));
    ui->tableContact->setItem(this->ligne,1,nom);
    ui->tableContact->setItem(this->ligne,2,entreprise);
    if(ui->idInfo->text().toUInt() == c->getIdContact()){
        ui->nomInfo->setText(QString::fromStdString(c->getNom()));
        ui->prenomInfo->setText(QString::fromStdString(c->getPrenom()));
        ui->mailInfo->setText(QString::fromStdString(c->getMail()));
        ui->telephoneInfo->setText(QString::fromStdString(c->getTelephoneString()));
        ui->entrepriseInfo->setText(QString::fromStdString(c->getEntreprise()));
        ui->dModifInfo->setText(QString::fromStdString(c->getDateModification()->getDateString()));

        QPixmap * pixmap = new QPixmap(QString::fromStdString(c->getPhoto()));
        if(pixmap->isNull()){
            pixmap = new QPixmap(":/images/photoDefaut");
        }
        ui->PhotoLabel->setPixmap(pixmap->scaled(ui->PhotoLabel->width(), ui->PhotoLabel->height()));
        ui->PhotoLabel->setMask(pixmap->mask());
    }

}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Met à jour les informations des labels contenant les informations d'un contact
 * Slots lancé après un clique sur la une ligne de la table
 * @param row : l'instance QModelIndex contenant l'index de la ligne cliqué
 * @param col : l'instance QModelIndex contenant l'index de la colonne cliqué
 */
void MainWindow::majInfo(QModelIndex row, QModelIndex col){
    if(ui->tableContact->rowCount() != 0){
        unsigned idContact = ui->tableContact->model()->index(row.row(),0).data().toUInt();
        Contact * c = this->gestionC->getById(idContact);

        ui->idInfo->setText(QString::number(c->getIdContact()));
        ui->nomInfo->setText(QString::fromStdString(c->getNom()));
        ui->prenomInfo->setText(QString::fromStdString(c->getPrenom()));
        ui->mailInfo->setText(QString::fromStdString(c->getMail()));
        ui->telephoneInfo->setText(QString::fromStdString(c->getTelephoneString()));
        ui->dCreationInfo->setText(QString::fromStdString(c->getDateCreation()->getDateString()));
        ui->entrepriseInfo->setText(QString::fromStdString(c->getEntreprise()));
        if(c->getDateModification())
            ui->dModifInfo->setText(QString::fromStdString(c->getDateModification()->getDateString()));
        else
            ui->dModifInfo->setText(QString("Pas de date de modification"));

        QPixmap * pixmap = new QPixmap(QString::fromStdString(c->getPhoto()));

        if(pixmap->isNull()){
            pixmap = new QPixmap(":/images/photoDefaut");
        }
        ui->PhotoLabel->setPixmap(pixmap->scaled(ui->PhotoLabel->width(), ui->PhotoLabel->height()));
        ui->PhotoLabel->setMask(pixmap->mask());
    }

}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Récupère et met à jour la table des contacts avec les contacts correspondant aux critères de recherche
 * Slot executé lors du clique du boutton recherche
 * Vérifie également les critères de recherches
 */
void MainWindow::rechercheContacts(){
    GestionContact * res = new GestionContact(nullptr);
    bool lancerRecherche = true;
    res->setContacts(this->gestionC->getContacts());
    if(!ui->nomCheckBox->isChecked() && !ui->entrepriseCheckBox->isChecked() && !ui->dCreationCheckBox->isChecked() && !ui->dIntervalleCheckBox->isChecked())
        ui->erreurLabel->setText("Vous devez au moins saisir un critère de recherche");
    else{
        ui->erreurLabel->setText("");
        if(ui->nomCheckBox->isChecked()){
            if(ui->nomSaisie->text() == ""){
                ui->erreurLabel->setText("Veuillez saisir un nom");
                lancerRecherche = false;
            }
            else{
                res->setContacts(res->getAllByName(ui->nomSaisie->text().toStdString()));
                ui->erreurLabel->setText("");
            }
        }
        if(ui->entrepriseCheckBox->isChecked()){
            if(ui->entrepriseSaisie->text() == ""){
                ui->erreurLabel->setText("Veuillez saisir une entreprise");
                lancerRecherche = false;
            }
            else{
                res->setContacts(res->getAllByEntreprise(ui->entrepriseSaisie->text().toStdString()));
                ui->erreurLabel->setText("");
            }
        }
        if(ui->dCreationCheckBox->isChecked()){
                GestionDate * dateC = new GestionDate(ui->dateCreationEdit->date().day(), ui->dateCreationEdit->date().month(), ui->dateCreationEdit->date().year());
                res->setContacts(res->getByDate(dateC));
        }
        if(ui->dIntervalleCheckBox->isChecked()){
                GestionDate * dateD = new GestionDate(ui->dateDebutI->date().day(), ui->dateDebutI->date().month(), ui->dateDebutI->date().year());
                GestionDate * dateF = new GestionDate(ui->dateFinI->date().day(), ui->dateFinI->date().month(), ui->dateFinI->date().year());

                if(dateD->getDate() > dateF->getDate()){
                    ui->erreurLabel->setText("La date de début doit être inférieur à la date de fin");
                    lancerRecherche = false;
                }
                else{
                    res->setContacts(res->getByDatesIntervalle(dateD, dateF));
                    ui->erreurLabel->setText("");
                }
        }
        if(lancerRecherche)
            remplirTable(res);
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date intervalle s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date de création
 */
void MainWindow::dateCreationCheckB(){
    if(ui->dIntervalleCheckBox->isChecked())
        ui->dIntervalleCheckBox->setChecked(false);
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date de création s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date intervalle
 */
void MainWindow::dateIntervalleCheckB(){
    if(ui->dCreationCheckBox->isChecked())
        ui->dCreationCheckBox->setChecked(false);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Supprime les critères de recherche
 * Slot executé lors du clique du boutton de suppression de filtres
 */
void MainWindow::resetRecherche(){
    ui->nomCheckBox->setChecked(false);
    ui->nomSaisie->setText("");

    ui->entrepriseCheckBox->setChecked(false);
    ui->entrepriseSaisie->setText("");

    ui->dCreationCheckBox->setChecked(false);
    ui->dateCreationEdit->setDate(QDate::currentDate());

    ui->dIntervalleCheckBox->setChecked(false);
    ui->dateFinI->setDate(QDate::currentDate());
    ui->dateDebutI->setDate(QDate::currentDate());

    ui->erreurLabel->setText("");

    remplirTable(this->gestionC);
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Export en format Json la bdd
 * Slots executé lors du clique du boutton exporter du menu bar
 */
void MainWindow::exportJson(){
    QString filename= QFileDialog::getExistingDirectory(this,"Choisir un dossier");
    if(!filename.isEmpty()){
        QJsonDocument documentC;
        documentC.setArray(BDD::getInstance()->contactsToJson());
        QFile jsonFileC(filename + "/contacts");
        jsonFileC.open(QFile::WriteOnly);
        jsonFileC.write(documentC.toJson());

        QJsonDocument documentT;
        documentT.setArray(BDD::getInstance()->todosToJson());
        QFile jsonFileT(filename + "/todos");
        jsonFileT.open(QFile::WriteOnly);
        jsonFileT.write(documentT.toJson());

        QJsonDocument documentA;
        documentA.setArray(BDD::getInstance()->associationsToJson());
        QFile jsonFileA(filename + "/associations");
        jsonFileA.open(QFile::WriteOnly);
        jsonFileA.write(documentA.toJson());

        QJsonDocument documentI;
        documentI.setArray(BDD::getInstance()->interactionsToJson());
        QFile jsonFileI(filename + "/interactions");
        jsonFileI.open(QFile::WriteOnly);
        jsonFileI.write(documentI.toJson());

    }

}
