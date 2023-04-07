#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gestioncontact.h"
#include "formajoutcontact.h"
#include "editcontact.h"
#include "rechercheinteractions.h"
#include "recherchetodos.h"
#include <QModelIndex>

/**
 * @brief Fenêtre principale de notre applicaation, affiche tous les contacts de l'application
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GestionContact * gestionC;
    FormAjoutContact * f;
    EditContact * edit;
    RechercheInteractions * rInteractions;
    RechercheTodos * rTodos;
    unsigned ligne;                                        //Ligne du tableau en cours de modification
    void remplirTable(GestionContact * const);
    void suppressionInfo();

private slots:
    void ouvertureForm();
    void ouvertureInteractions();
    void ouvertureTodos();

    void ajouterContact(Contact *);
    void editContact();
    void deleteButton();
    void majContact(Contact *);
    void majInfo(QModelIndex, QModelIndex);

    void rechercheContacts();
    void dateCreationCheckB();
    void dateIntervalleCheckB();
    void resetRecherche();
    void exportJson();

signals:
    void envoieEdit(Contact *);
    void envoieInteractions(GestionContact *);
    void envoieTodos();

};
#endif // MAINWINDOW_H
