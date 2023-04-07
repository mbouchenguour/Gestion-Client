#include "editcontact.h"
#include "ui_editcontact.h"
#include "bdd.h"
#include "gestionait.h"
#include <QDebug>
#include <QDate>
#include <QTextBlock>
#include <QFileDialog>
#include <QMessageBox>


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implémentation du constructeur
 * Initialise les différents widgets
 * Connecte les différents bouttons/checkbox à des slots
 */

EditContact::EditContact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditContact)
{
    ui->setupUi(this);

    connect(ui->modifierButton, SIGNAL(clicked()), this, SLOT(modifierContact()));
    connect(ui->ajouterButton, SIGNAL(clicked()), this, SLOT(ajoutInteraction()));
    connect(ui->dateButton, SIGNAL(clicked()), this, SLOT(ajouterDate()));
    connect(ui->imageButton, SIGNAL(clicked()), this, SLOT(modifierImage()));

    ui->calendarWidget->setMinimumDate(QDate::currentDate());

    ui->dateAjoutEdit->setMaximumDate(QDate::currentDate());
    ui->dDebutInteraction->setMaximumDate(QDate::currentDate());
    ui->dFinInteraction->setMaximumDate(QDate::currentDate());

    QRegExp telValidateur("[0-9]{10}");
    ui->telephoneLineEdit->setValidator(new QRegExpValidator(telValidateur,this));


    ui->dateAjoutEdit->setDate(QDate::currentDate());
    ui->dFinInteraction->setDate(QDate::currentDate());
    ui->dDebutInteraction->setDate(QDate::currentDate());

    ui->dateRealisationEdit->setDate(QDate::currentDate());
    ui->dDebutTodo->setDate(QDate::currentDate());
    ui->dFinTodo->setDate(QDate::currentDate());

    connect(ui->dAjoutCheckBox, SIGNAL(clicked()), this, SLOT(dateAjoutCheckB()));
    connect(ui->dIntervalleInterCheckBox, SIGNAL(clicked()), this, SLOT(dateIntervalleInterCheckB()));

    connect(ui->dRealisationCheckBox, SIGNAL(clicked()), this, SLOT(dateRealisationCheckB()));
    connect(ui->dIntervalleTodoCheckBox, SIGNAL(clicked()), this, SLOT(dateIntervalleTodoCheckB()));

    connect(ui->suppressionFiltreInterButton, SIGNAL(clicked()), this, SLOT(resetRechercheInter()));
    connect(ui->rechercheFiltreInterButton, SIGNAL(clicked()), this, SLOT(rechercheInteractions()));
    connect(ui->rechercheFiltreTodoButton, SIGNAL(clicked()), this, SLOT(rechercheTodos()));
    connect(ui->suppressionFiltreTodoButton, SIGNAL(clicked()), this, SLOT(resetRechercheTodos()));


    ui->interactionsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->interactionsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->interactionsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->todosTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->todosTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->todosTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->todosTableWidget->setSortingEnabled(true);
    ui->interactionsTableWidget->setSortingEnabled(true);
    ui->todosTableWidget->horizontalHeader()->sortIndicatorOrder();
    ui->interactionsTableWidget->horizontalHeader()->sortIndicatorOrder();

    // On change la couleur des messages d'erreurs en rouge
    QPalette rouge;
    rouge.setColor(QPalette::WindowText, Qt::red);
    ui->erreurAjoutLabel->setPalette(rouge);
    ui->erreurModifLabel->setPalette(rouge);
    ui->erreurRechercheInter->setPalette(rouge);
    ui->erreurRechercheTodo->setPalette(rouge);

    this->setFixedSize(this->geometry().width(), this->geometry().height());

}

EditContact::~EditContact()
{
    delete ui;
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Vide et remplie la table QTableWidget des todos
 * @param lTodos : la liste std::list (const) contenant les pointeurs des todos
 */
void EditContact::remplirTableTodos(const std::list<Todo *> & lTodos)
{
    ui->todosTableWidget->setSortingEnabled(false);
    ui->interactionsTableWidget->setSortingEnabled(false);
    ui->todosTableWidget->clearContents();
    ui->todosTableWidget->setRowCount(0);
    for(Todo * t : lTodos){
        int nb = ui->todosTableWidget->rowCount();
        ui->todosTableWidget->insertRow(nb);
        QTableWidgetItem * contenuTodo = new QTableWidgetItem(QString::fromStdString(t->getContenuTodo()));

        QTableWidgetItem * dateRealisation = new QTableWidgetItem();
        dateRealisation->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(t->getDateRealisation()->getDateString()), "dd/MM/yyyy"));

        ui->todosTableWidget->setItem(nb,0,contenuTodo);
        ui->todosTableWidget->setItem(nb,1,dateRealisation);
    }
    ui->todosTableWidget->setSortingEnabled(true);
    ui->interactionsTableWidget->setSortingEnabled(true);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Vide et remplie la table QTableWidget des interactions
 * @param lInteractions : la liste std::list (const) contenant les instances Interaction
 */
void EditContact::remplirTableInteractions(const std::list<Interaction> & lInteractions)
{
    ui->todosTableWidget->setSortingEnabled(false);
    ui->interactionsTableWidget->setSortingEnabled(false);
    ui->interactionsTableWidget->clearContents();
    ui->interactionsTableWidget->setRowCount(0);

    for(Interaction i : lInteractions){
        int nb = ui->interactionsTableWidget->rowCount();
        ui->interactionsTableWidget->insertRow(nb);
        QTableWidgetItem * contenuInteraction = new QTableWidgetItem(QString::fromStdString(i.getContenuInteraction()));

        QTableWidgetItem * dateAjout = new QTableWidgetItem();
        dateAjout->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(i.getDateAjout()->getDateString()), "dd/MM/yyyy"));

        ui->interactionsTableWidget->setItem(nb,0,contenuInteraction);
        ui->interactionsTableWidget->setItem(nb,1,dateAjout);
    }
    ui->todosTableWidget->setSortingEnabled(true);
    ui->interactionsTableWidget->setSortingEnabled(true);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Initialise les QLineEdit avec les informations du contact
 * Remplie les tables todos et interactions avec les todos et interactions du contacts
 * Affiche le widget
 * Slot executé lors du clique du boutton information du mainwindow
 */
void EditContact::initialiser(Contact * c){
    this->c = c;
    ui->nomLineEdit->setText(QString::fromStdString(c->getNom()));
    ui->prenomLineEdit->setText(QString::fromStdString(c->getPrenom()));
    ui->entrepriseLineEdit->setText(QString::fromStdString(c->getEntreprise()));
    ui->mailLineEdit->setText(QString::fromStdString(c->getMail()));
    ui->telephoneLineEdit->setText(QString::fromStdString(c->getTelephoneString()));
    ui->imageLineEdit->setText(QString::fromStdString(c->getPhoto()));

    remplirTableInteractions(this->c->getGestionI()->getInteractions());
    remplirTableTodos(this->c->getGestionI()->getTodos());

    this->show();
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Modifie le contact avec les valeurs des QEditLine
 * Demande au gestionnaire de bdd à modifier le contact dans la bdd
 * Slot executé lors du clique du boutton modifier
 */
void EditContact::modifierContact(){

    //Validateur mail
    QRegExp mailValidateur("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", Qt::CaseInsensitive);
    mailValidateur.setPatternSyntax(QRegExp::RegExp);

    //Validateur telephone
    QRegExp telValidateur("[0-9]{10}");
    telValidateur.setPatternSyntax(QRegExp::RegExp);


    if(ui->nomLineEdit->text().isEmpty() || ui->prenomLineEdit->text().isEmpty() || ui->entrepriseLineEdit->text().isEmpty() || ui->mailLineEdit->text().isEmpty()|| ui->imageLineEdit->text().isEmpty())
        ui->erreurModifLabel->setText("Veulliez remplir tous les champs");
    else
    {
        if(!mailValidateur.exactMatch(ui->mailLineEdit->text()))
                ui->erreurModifLabel->setText("Format du mail incorrect");
        else{
            if(!telValidateur.exactMatch(ui->telephoneLineEdit->text()))
                ui->erreurModifLabel->setText("Téléphone incorrect");
            else{

                std::list<unsigned> temp;
                for(char i : ui->telephoneLineEdit->text().toStdString())
                    temp.push_back(int(i) - 48);

                ui->erreurModifLabel->setText("");
                c->setNom(ui->nomLineEdit->text().toStdString());
                c->setPrenom(ui->prenomLineEdit->text().toStdString());
                c->setEntreprise(ui->entrepriseLineEdit->text().toStdString());
                c->setMail(ui->mailLineEdit->text().toStdString());
                c->setTelephone(temp);
                c->setPhoto(ui->imageLineEdit->text().toStdString());
                BDD::getInstance()->modifierContact(c);
                emit contactModifier(c);
                QMessageBox msgBox;
                msgBox.setText("Contact modifié");
                msgBox.exec();
            }

        }
    }



}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Ajoute une interaction et les todos correspondant, au contact à partir du champ de saisie
 * Analyse le champ de saisie afin d'extraire les tags todo et date
 * Slot executé lors du clique du boutton ajouter interaction
 * Pas de paramètre
 */
void EditContact::ajoutInteraction(){
    QTextDocument * qdoc = ui->ajoutInteractionEdit->document();
    QTextCursor cur(qdoc);
    bool dateCorrectB = true;
    QTextDocument :: FindFlag fcs = ( QTextDocument :: FindFlag ) 0;
    QRegularExpression qregTodo ("^@todo .*$");
    QStringList ltodos;
    while (!cur.isNull())
    {
        cur = qdoc->find(qregTodo, cur.position(), fcs);
        if (!cur.isNull())
        {
            QTextBlock qtb = cur.block();
            ltodos.append(qtb.text());
        }
    }
    GestionDate * dateRealisationTodo = new GestionDate();

    for(QString s : ltodos){
        //Verification des dates des todo

        QRegularExpression qregDate (" @date ");
        bool tagDate = s.contains(qregDate);
        if(tagDate){
            QStringList temp = s.split(" ");
            if(temp.at(temp.size()-2) != QString("@date")){
                ui->erreurAjoutLabel->setText(QString("Date du todo incorrect, veuillez saisir la date à la fin de la ligne"));
                dateCorrectB = false;
            }
            else
            {
                QRegularExpression dateCorrect("^(\\d\\d)/(\\d\\d)/(\\d\\d\\d\\d)$");
                QRegularExpressionMatch match = dateCorrect.match(temp.at(temp.size()-1));
                if (match.hasMatch()) {
                    GestionDate * dateDuJour = new GestionDate();
                    dateRealisationTodo = new GestionDate(match.captured(1).toUInt(), match.captured(2).toUInt(), match.captured(3).toUInt());
                    if(dateRealisationTodo->getDate() < dateDuJour->getDate()){
                        ui->erreurAjoutLabel->setText(QString("La date du todo ne peux pas être avant la date du jour"));
                        dateCorrectB = false;
                    }
                }
                else{
                    ui->erreurAjoutLabel->setText(QString("Le format de la date doit être ""JJ/MM/AAAA"""));
                    dateCorrectB = false;
                }
            }
        }
    }

    if(dateCorrectB && !ui->ajoutInteractionEdit->toPlainText().isEmpty()){

        //Création de l'interaction
        Interaction * i = new Interaction(0, ui->ajoutInteractionEdit->toPlainText().toStdString(), new GestionDate());
        unsigned idI = BDD::getInstance()->ajouterInteraction(i, c->getIdContact());
        i->setIdInteraction(idI);


        //Insertion dans la table des interractions
        ui->interactionsTableWidget->setSortingEnabled(false);
        int nb = ui->interactionsTableWidget->rowCount();
        ui->interactionsTableWidget->insertRow(nb);
        QTableWidgetItem * contenuInteraction = new QTableWidgetItem(QString::fromStdString(i->getContenuInteraction()));


        QTableWidgetItem * dateAjout = new QTableWidgetItem();
        dateAjout->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(i->getDateAjout()->getDateString()), "dd/MM/yyyy"));

        ui->interactionsTableWidget->setItem(nb,0,contenuInteraction);
        ui->interactionsTableWidget->setItem(nb,1,dateAjout);
        ui->interactionsTableWidget->setSortingEnabled(true);
        //ui->interactionsTableWidget->update();

        c->getGestionI()->addInteraction(*i);

        ui->todosTableWidget->setSortingEnabled(false);
        for(QString s : ltodos){
            QRegularExpression qregDate (" @date ");
            bool tagDate = s.contains(qregDate);
            if(tagDate){
                QStringList temp = s.split(" ");
                QString dateString = temp.at(temp.size()-1);
                dateRealisationTodo = new GestionDate(dateString.split("/").at(0).toUInt(), dateString.split("/").at(1).toUInt(), dateString.split("/").at(2).toUInt());
                s = s.left(s.lastIndexOf('@'));
            }
            else
                dateRealisationTodo = new GestionDate();

            Todo * t = new Todo(0, s.remove(0,6).toStdString(), dateRealisationTodo);

            //Ajout du todo dans la bdd
            unsigned idT = BDD::getInstance()->ajouterTodo(t);
            t->setIdTodo(idT);

            //Création de l'association et ajout dans la BDD
            AssociationInteractionTodo * a = new AssociationInteractionTodo(0, t, i);
            unsigned idA = BDD::getInstance()->ajouterAssociation(a);
            a->setIdAssociation(idA);

            //Ajout dans le gestionnaire d'associations
            GestionAIT::getInstance()->addAssociation(*a);

            //Insertion dans le tableau de todo

            int nb = ui->todosTableWidget->rowCount();
            ui->todosTableWidget->insertRow(nb);
            QTableWidgetItem * contenuTodo = new QTableWidgetItem(QString::fromStdString(t->getContenuTodo()));

            QTableWidgetItem * dateRealisation = new QTableWidgetItem();
            dateRealisation->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(t->getDateRealisation()->getDateString()), "dd/MM/yyyy"));

            ui->todosTableWidget->setItem(nb,0,contenuTodo);
            ui->todosTableWidget->setItem(nb,1,dateRealisation);
        }
        ui->todosTableWidget->setSortingEnabled(true);

        ui->ajoutInteractionEdit->clear();
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Ajoute un tag date au QTextEdit contenant la nouvelle Interaction
 * Slot executé lors du clique du boutton ajouter date
 */
void EditContact::ajouterDate(){
    QTextCursor cur = ui->ajoutInteractionEdit->textCursor();
    QRegularExpression qregTodo ("^@todo .*$");

    bool tagTodo = cur.block().text().contains(qregTodo);
    if (tagTodo){
        QRegularExpression qregDate (" @date ");
        bool tagDate = cur.block().text().contains(qregDate);
        if(tagDate){
            ui->erreurAjoutLabel->setText(QString("Il y a deja une date pour ce todo"));
        }
        else{
            QString dateSaisie = (" @date ") + ui->calendarWidget->selectedDate().toString("dd/MM/yyyy");
            ui->ajoutInteractionEdit->insertPlainText(dateSaisie);
            ui->erreurAjoutLabel->setText(QString(""));
        }

    }
    else{
        ui->erreurAjoutLabel->setText(QString("Vous ne pouvez pas ajouter de date s'il n'y a pas de @todo"));

    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Lance la recherche des Interaction et des Todo correspondant à l'aide des critères de recherche
 * Vérifie les critères de recherche
 * Remplie les tables contenant les Todos et les interactions
 * Slot executé lors du clique du boutton de lancement de recherche des Interaction
 */
void EditContact::rechercheInteractions()
{
    bool lancerRecherche = true;
    GestionInteraction * res = new GestionInteraction();
    ui->erreurRechercheTodo->setText("");
    if(!ui->dIntervalleInterCheckBox->isChecked() && !ui->dAjoutCheckBox->isChecked())
        ui->erreurRechercheInter->setText("Veuillez choisir un critère");
    else
    {
        ui->erreurRechercheInter->setText("");
        if(ui->dAjoutCheckBox->isChecked())
        {
            ui->erreurRechercheInter->setText("");
            GestionDate * dateA = new GestionDate(ui->dateAjoutEdit->date().day(), ui->dateAjoutEdit->date().month(), ui->dateAjoutEdit->date().year());
            res->setInteractions(this->c->getGestionI()->getInteractionsByDate(dateA));
        }
        else
        {
            GestionDate * dateD = new GestionDate(ui->dDebutInteraction->date().day(), ui->dDebutInteraction->date().month(), ui->dDebutInteraction->date().year());
            GestionDate * dateF = new GestionDate(ui->dFinInteraction->date().day(), ui->dFinInteraction->date().month(), ui->dFinInteraction->date().year());
            if(dateD->getDate() > dateF->getDate()){
                lancerRecherche = false;
                ui->erreurRechercheInter->setText("La date de début doit être inférieur à la date de fin");
            }
            else{
                ui->erreurRechercheInter->setText("");
                res->setInteractions(this->c->getGestionI()->getInteractionsByIntervalle(dateD, dateF));
            }

        }

        if(lancerRecherche){
            remplirTableInteractions(res->getInteractions());
            remplirTableTodos(res->getTodos());
        }

    }

}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date intervalle du Todo s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date de réalisation des todos
 */
void EditContact::dateRealisationCheckB()
{
    if(ui->dIntervalleTodoCheckBox->isChecked())
        ui->dIntervalleTodoCheckBox->setChecked(false);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date de réalisation des Todo s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date intervalle des Todos
 */
void EditContact::dateIntervalleTodoCheckB()
{
    if(ui->dRealisationCheckBox->isChecked())
        ui->dRealisationCheckBox->setChecked(false);
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date intervalle de l'interaction s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date de création des interactions
 */
void EditContact::dateAjoutCheckB()
{
    if(ui->dIntervalleInterCheckBox->isChecked())
        ui->dIntervalleInterCheckBox->setChecked(false);
}



/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date d'ajout de l'interaction s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date intervalle des interactions
 */
void EditContact::dateIntervalleInterCheckB()
{
    if(ui->dAjoutCheckBox->isChecked())
        ui->dAjoutCheckBox->setChecked(false);
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Supprime les critères de recherche
 * Slot executé lors du clique du boutton de suppression de filtres des Interaction
 */
void EditContact::resetRechercheInter()
{
    ui->dIntervalleInterCheckBox->setChecked(false);
    ui->dAjoutCheckBox->setChecked(false);

    ui->dateAjoutEdit->setDate(QDate::currentDate());
    ui->dFinInteraction->setDate(QDate::currentDate());
    ui->dDebutInteraction->setDate(QDate::currentDate());

    ui->erreurRechercheInter->setText("");
    ui->erreurRechercheTodo->setText("");

    remplirTableInteractions(this->c->getGestionI()->getInteractions());
    remplirTableTodos(this->c->getGestionI()->getTodos());

}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Lance la recherche des Todo et des Interactions correspondant à l'aide des critères de recherche
 * Vérifie les critères de recherche
 * Remplie les tables contenant les Todos et les interactions
 * Slot executé lors du clique du boutton de lancement de recherche des Todos
 */
void EditContact::rechercheTodos()
{
    bool lancerRecherche = true;
    std::list<Todo *> lTodos;
    ui->erreurRechercheInter->setText("");

    if(!ui->dRealisationCheckBox->isChecked() && !ui->dIntervalleTodoCheckBox->isChecked()){
        ui->erreurRechercheTodo->setText("Veuillez choisir un critère");
    }
    else
    {
        ui->erreurRechercheTodo->setText("");
        if(ui->dRealisationCheckBox->isChecked())
        {
            ui->erreurRechercheTodo->setText("");
            GestionDate * dateA = new GestionDate(ui->dateRealisationEdit->date().day(), ui->dateRealisationEdit->date().month(), ui->dateRealisationEdit->date().year());
            for(Todo * t : this->c->getGestionI()->getTodos()){
                if(t->getDateRealisation()->getDate() == dateA->getDate()){
                    lTodos.push_back(t);
                }
            }
        }
        else
        {
            GestionDate * dateD = new GestionDate(ui->dDebutTodo->date().day(), ui->dDebutTodo->date().month(), ui->dDebutTodo->date().year());
            GestionDate * dateF = new GestionDate(ui->dFinTodo->date().day(), ui->dFinTodo->date().month(), ui->dFinTodo->date().year());
            if(dateD->getDate() > dateF->getDate()){
                lancerRecherche = false;
                ui->erreurRechercheTodo->setText("La date de début doit être inférieur à la date de fin");
            }
            else{
                ui->erreurRechercheTodo->setText("");
                for(Todo * t : this->c->getGestionI()->getTodos()){
                    if(t->getDateRealisation()->getDate() >= dateD->getDate() && t->getDateRealisation()->getDate() <= dateF->getDate()){
                        lTodos.push_back(t);
                    }
                }
            }
        }

        if(lancerRecherche){
            ui->interactionsTableWidget->setSortingEnabled(false);
            ui->interactionsTableWidget->clearContents();
            ui->interactionsTableWidget->setRowCount(0);

            for(Interaction * i : GestionAIT::getInstance()->getInteractionsByTodos(lTodos)){
                int nb = ui->interactionsTableWidget->rowCount();
                ui->interactionsTableWidget->insertRow(nb);
                QTableWidgetItem * contenuInteraction = new QTableWidgetItem(QString::fromStdString(i->getContenuInteraction()));
                QTableWidgetItem * dateAjout = new QTableWidgetItem();
                dateAjout->setData(Qt::DisplayRole, QDate::fromString(QString::fromStdString(i->getDateAjout()->getDateString()), "dd/MM/yyyy"));

                ui->interactionsTableWidget->setItem(nb,0,contenuInteraction);
                ui->interactionsTableWidget->setItem(nb,1,dateAjout);

            }
            ui->interactionsTableWidget->setSortingEnabled(true);
            remplirTableTodos(lTodos);
        }

    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche les checkbox et affecte la date du jour au QDateEdit des critères de recherches des todos
 * Vide et remplie les tables QTableWidget avec les interactions et todos du contact
 * Slot executé lors du clique du boutton de Supprimer filtre des todos
 */
void EditContact::resetRechercheTodos()
{
    ui->dRealisationCheckBox->setChecked(false);
    ui->dIntervalleTodoCheckBox->setChecked(false);
    ui->dateAjoutEdit->setDate(QDate::currentDate());
    ui->dDebutTodo->setDate(QDate::currentDate());
    ui->dFinTodo->setDate(QDate::currentDate());

    ui->erreurRechercheTodo->setText("");
    ui->erreurRechercheInter->setText("");

    remplirTableInteractions(this->c->getGestionI()->getInteractions());
    remplirTableTodos(this->c->getGestionI()->getTodos());
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Ouvre une fenêtre afin de chercher une image de format png et jpg
 * Slot executé lors du clique du boutton parcourir
 */
void EditContact::modifierImage(){
    ui->imageLineEdit->setText(QFileDialog::getOpenFileName(this, tr("Ajouter une image "), QString(), "Images (*.png *.jpg )"));
}

