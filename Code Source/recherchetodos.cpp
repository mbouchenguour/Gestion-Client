#include "recherchetodos.h"
#include "ui_recherchetodos.h"
#include "gestionait.h"


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implémentation du constructeur
 * Initialise les différents widgets
 * Connecte les différents bouttons à des slots
 */
RechercheTodos::RechercheTodos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RechercheTodos)
{
    ui->setupUi(this);

    ui->dateRealisationEdit->setDate(QDate::currentDate());
    ui->dDebutTodo->setDate(QDate::currentDate());
    ui->dFinTodo->setDate(QDate::currentDate());

    connect(ui->dRealisationCheckBox, SIGNAL(clicked()), this, SLOT(dateRealisationCheckB()));
    connect(ui->dIntervalleTodoCheckBox, SIGNAL(clicked()), this, SLOT(dateIntervalleCheckB()));
    connect(ui->rechercheFiltreTodoButton, SIGNAL(clicked()), this, SLOT(lancerRecherche()));
    connect(ui->suppressionFiltreTodoButton, SIGNAL(clicked()), this, SLOT(resetRecherche()));

    ui->todosTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->todosTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    ui->todosTableWidget->setSortingEnabled(true);
    ui->todosTableWidget->horizontalHeader()->sortIndicatorOrder();


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
RechercheTodos::~RechercheTodos()
{
    delete ui;
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Vide et remplie la table QTableWidget des Todos
 * @param lTodos : la liste std::list (const) contenant les pointeurs des Todos
 */
void RechercheTodos::remplirTable(const std::list<Todo *> & lTodos)
{
    ui->todosTableWidget->setSortingEnabled(false);
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

}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Lance la recherche des Todos à l'aide des critères de recherche
 * Vérifie les critères de recherche
 * Remplie la table contenant les todos
 * Slot executé lors du clique du boutton de lancement de recherche des Todos
 */
void RechercheTodos::lancerRecherche()
{
    bool lancerRecherche = true;
    std::list<Todo *> lTodos;
    if(!ui->dRealisationCheckBox->isChecked() && !ui->dIntervalleTodoCheckBox->isChecked()){
        ui->erreurRechercheLabel->setText("Veuillez choisir un critère");
    }
    else
    {
        ui->erreurRechercheLabel->setText("");
        if(ui->dRealisationCheckBox->isChecked())
        {
            ui->erreurRechercheLabel->setText("");
            GestionDate * dateA = new GestionDate(ui->dateRealisationEdit->date().day(), ui->dateRealisationEdit->date().month(), ui->dateRealisationEdit->date().year());
            for(Todo * t : GestionAIT::getInstance()->getAllTodos()){
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
                ui->erreurRechercheLabel->setText("La date de début doit être inférieur à la date de fin");
            }
            else{
                ui->erreurRechercheLabel->setText("");
                for(Todo * t : GestionAIT::getInstance()->getAllTodos()){
                    if(t->getDateRealisation()->getDate() >= dateD->getDate() && t->getDateRealisation()->getDate() <= dateF->getDate()){
                        lTodos.push_back(t);
                    }
                }
            }

        }

        if(lancerRecherche){
            remplirTable(lTodos);
        }

    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Supprime les critères de recherche
 * Remplie la table des Todos avec tous les Todos de l'application
 * Slot executé lors du clique du boutton suppression
 */
void RechercheTodos::resetRecherche()
{
    ui->dRealisationCheckBox->setChecked(false);
    ui->dIntervalleTodoCheckBox->setChecked(false);

    ui->dateRealisationEdit->setDate(QDate::currentDate());
    ui->dDebutTodo->setDate(QDate::currentDate());
    ui->dFinTodo->setDate(QDate::currentDate());

    remplirTable(GestionAIT::getInstance()->getAllTodos());
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date intervalle s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date de realisation des Todos
 */
void RechercheTodos::dateRealisationCheckB()
{
    if(ui->dIntervalleTodoCheckBox->isChecked()){
        ui->dIntervalleTodoCheckBox->setChecked(false);
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Décoche le checkBox du critère de recherche date de realisation s'il est coché
 * Slot executé lors du clique du checkBox pour le critère de recherche date intervalles des Todos
 */
void RechercheTodos::dateIntervalleCheckB()
{
    if(ui->dRealisationCheckBox->isChecked()){
        ui->dRealisationCheckBox->setChecked(false);
    }
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Récupère tous les todos de l'application et remplie la table des Todo avec
 * Affiche le widget
 * Slot executé lors de la reception du signal envoieTodos du mainwindow
 */
void RechercheTodos::ouverture(){
    remplirTable(GestionAIT::getInstance()->getAllTodos());
    this->show();
}



