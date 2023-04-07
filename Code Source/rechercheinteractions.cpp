#include "rechercheinteractions.h"
#include "gestionait.h"
#include "ui_rechercheinteractions.h"
#include <QDebug>

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implémentation du constructeur
 * Initialise les différents widgets
 * Connecte les différents bouttons à des slots
 */
RechercheInteractions::RechercheInteractions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RechercheInteractions)
{
    ui->setupUi(this);

    ui->dateAjoutEdit->setMaximumDate(QDate::currentDate());
    ui->dDebutInteraction->setMaximumDate(QDate::currentDate());
    ui->dFinInteraction->setMaximumDate(QDate::currentDate());

    ui->dateAjoutEdit->setDate(QDate::currentDate());
    ui->dFinInteraction->setDate(QDate::currentDate());
    ui->dDebutInteraction->setDate(QDate::currentDate());

    connect(ui->dAjoutCheckBox, SIGNAL(clicked()), this, SLOT(dateAjoutCheckB()));
    connect(ui->dIntervalleInterCheckBox, SIGNAL(clicked()), this, SLOT(dateIntervalleCheckB()));
    connect(ui->suppressionFiltreInterButton, SIGNAL(clicked()), this, SLOT(resetRecherche()));
    connect(ui->rechercheFiltreInterButton, SIGNAL(clicked()), this, SLOT(lancerRecherche()));


    ui->interactionsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->interactionsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->interactionsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->interactionsTableWidget->setSortingEnabled(true);
    ui->interactionsTableWidget->horizontalHeader()->sortIndicatorOrder();

    // On change la couleur des messages d'erreurs en rouge
    QPalette rouge;
    rouge.setColor(QPalette::WindowText, Qt::red);
    ui->erreurRechercheLabel->setPalette(rouge);


    this->setFixedSize(this->geometry().width(), this->geometry().height());
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief destructeur
 */
RechercheInteractions::~RechercheInteractions()
{
    delete ui;
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date intervalle s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date d'ajout des interactions
 */
void RechercheInteractions::dateAjoutCheckB()
{
    if(ui->dIntervalleInterCheckBox->isChecked()){
        ui->dIntervalleInterCheckBox->setChecked(false);
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date ajout s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date intervalles des interactions
 */
void RechercheInteractions::dateIntervalleCheckB()
{
    if(ui->dAjoutCheckBox->isChecked()){
        ui->dAjoutCheckBox->setChecked(false);
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Lance la recherche des Interaction à l'aide des critères de recherche
 * Vérifie les critères de recherche
 * Remplie la table contenant les interactions
 * Slot executé lors du clique du boutton des filtres de recherche des Interaction
 */

void RechercheInteractions::resetRecherche()
{
    ui->dIntervalleInterCheckBox->setChecked(false);
    ui->dAjoutCheckBox->setChecked(false);

    ui->dateAjoutEdit->setDate(QDate::currentDate());
    ui->dFinInteraction->setDate(QDate::currentDate());
    ui->dDebutInteraction->setDate(QDate::currentDate());

    remplirTable(gestionI->getInteractions());
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Lance la recherche des Interaction à l'aide des critères de recherche
 * Vérifie les critères de recherche
 * Remplie la table contenant les interactions
 * Slot executé lors du clique du boutton de lancement de recherche des Interaction
 */
void RechercheInteractions::lancerRecherche(){
    bool lancerRecherche = true;
    GestionInteraction * res = new GestionInteraction();
    if(!ui->dIntervalleInterCheckBox->isChecked() && !ui->dAjoutCheckBox->isChecked()){
        ui->erreurRechercheLabel->setText("Veulliez choisir un critère");
    }
    else
    {
        ui->erreurRechercheLabel->setText("");
        if(ui->dAjoutCheckBox->isChecked())
        {
            ui->erreurRechercheLabel->setText("");
            GestionDate * dateA = new GestionDate(ui->dateAjoutEdit->date().day(), ui->dateAjoutEdit->date().month(), ui->dateAjoutEdit->date().year());
            res->setInteractions(gestionI->getInteractionsByDate(dateA));
        }
        else
        {
            GestionDate * dateD = new GestionDate(ui->dDebutInteraction->date().day(), ui->dDebutInteraction->date().month(), ui->dDebutInteraction->date().year());
            GestionDate * dateF = new GestionDate(ui->dFinInteraction->date().day(), ui->dFinInteraction->date().month(), ui->dFinInteraction->date().year());
            if(dateD->getDate() > dateF->getDate()){
                lancerRecherche = false;
                ui->erreurRechercheLabel->setText("La date de début doit être inférieur à la date de fin");
            }
            else{
                ui->erreurRechercheLabel->setText("");
                res->setInteractions(gestionI->getInteractionsByIntervalle(dateD, dateF));
            }

        }

        if(lancerRecherche){
            remplirTable(res->getInteractions());
        }

    }
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Récupère les interactions de tous les contacts et remplie la table QTableWidget des interactions
 * Affiche le widget
 * Slot executé lors de l'envoie du signal envoieInteractions du mainWindows
 * @param gestionC : le pointeur d'un GestionContact qui contient tous les contacts de l'application
 */
void RechercheInteractions::initialiser(GestionContact * gestionC){

    gestionI = new GestionInteraction();
    resetRecherche();
    for(Contact * c : gestionC->getContacts()){
        for(Interaction i : c->getGestionI()->getInteractions()){
            gestionI->addInteraction(i);
        }
    }
    remplirTable(gestionI->getInteractions());

    this->show();
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Vide et remplie la table QTableWidget des interactions
 * @param lInteractions : la liste std::list (const) contenant les instances Interaction
 */
void RechercheInteractions::remplirTable(const std::list<Interaction> & lInteractions){
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
    ui->interactionsTableWidget->setSortingEnabled(true);
}
