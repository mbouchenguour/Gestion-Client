#include "formajoutcontact.h"
#include "ui_formajoutcontact.h"
#include "contact.h"
#include "bdd.h"
#include <QDebug>
#include <QFileDialog>
#include <QRegularExpressionValidator>
#include <QRegularExpression>


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implémentation du constructeur
 * Initialise les différents widgets
 * Connecte les différents bouttons à des slots
 */
FormAjoutContact::FormAjoutContact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAjoutContact)
{
    ui->setupUi(this);
    connect(ui->buttonAjout, SIGNAL(clicked()), this, SLOT(ajoutContact()));
    connect(ui->buttonAnnuler, SIGNAL(clicked()), this, SLOT(annulerAjout()));
    connect(ui->imageButton, SIGNAL(clicked()), this, SLOT(ajouterImage()));

    QRegExp telValidateur("[0-9]{10}");
    ui->telephoneLineEdit->setValidator(new QRegExpValidator(telValidateur,this));

    QPalette rouge;
    rouge.setColor(QPalette::WindowText, Qt::red);
    ui->erreurLabel->setPalette(rouge);

    this->setFixedSize(this->geometry().width(), this->geometry().height());


}

FormAjoutContact::~FormAjoutContact()
{
    delete ui;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Crée une instance Contact à partir des champs et demande au gestionnaire de la bdd d'ajouter le contact
 * Vérifie les saisies de l'utilisateur
 * Envoie un signal au mainwindow afin qu'il ajoute le contact dans la table
 * Slot executé lors du clique du boutton ajouter
 */
void FormAjoutContact::ajoutContact(){
    QRegExp mailValidateur("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", Qt::CaseInsensitive);
    mailValidateur.setPatternSyntax(QRegExp::RegExp);

    QRegExp telValidateur("[0-9]{10}");
    telValidateur.setPatternSyntax(QRegExp::RegExp);


    if(ui->nomLigneEdit->text().isEmpty() || ui->prenomLineEdit->text().isEmpty() || ui->entrepriseLineEdit->text().isEmpty() || ui->mailLineEdit->text().isEmpty()|| ui->imageLineEdit->text().isEmpty()){
        ui->erreurLabel->setText("Veulliez remplir tous les champs");
    }
    else
    {
        if(!mailValidateur.exactMatch(ui->mailLineEdit->text())){
                ui->erreurLabel->setText("Format du mail incorrect");
        }
        else{
            if(!telValidateur.exactMatch(ui->telephoneLineEdit->text())){
                ui->erreurLabel->setText("Téléphone incorrect");
            }
            else{

                std::list<unsigned> temp;
                for(char i : ui->telephoneLineEdit->text().toStdString()){
                    temp.push_back(int(i) - 48);
                }
                Contact * contact = new Contact(0, ui->nomLigneEdit->text().toStdString(), ui->prenomLineEdit->text().toStdString(), ui->entrepriseLineEdit->text().toStdString(), ui->mailLineEdit->text().toStdString(), temp, ui->imageLineEdit->text().toStdString(), nullptr, nullptr);
                contact->setIdContact(BDD::getInstance()->ajoutContact(contact));
                emit ajouterContact(contact);

                ui->erreurLabel->setText("");
                ui->nomLigneEdit->clear();
                ui->prenomLineEdit->clear();
                ui->entrepriseLineEdit->clear();
                ui->mailLineEdit->clear();
                ui->telephoneLineEdit->clear();
                ui->imageLineEdit->clear();
                this->close();
            }

        }
    }

}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Supprime les champs du forme et ferme le widget
 * Slot executé lors du clique du boutton annuler
 */
void FormAjoutContact::annulerAjout(){
    ui->erreurLabel->setText("");
    ui->nomLigneEdit->clear();
    ui->prenomLineEdit->clear();
    ui->entrepriseLineEdit->clear();
    ui->mailLineEdit->clear();
    ui->telephoneLineEdit->clear();
    this->close();
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Ouvre une fenêtre afin de chercher une image de format png et jpg
 * Slot executé lors du clique du boutton parcourir
 */
void FormAjoutContact::ajouterImage(){
    ui->imageLineEdit->setText(QFileDialog::getOpenFileName(this, tr("Ajouter une image "), QString(), "Images (*.png *.jpg )"));
}
