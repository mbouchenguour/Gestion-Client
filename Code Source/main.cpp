#include "mainwindow.h"
#include "contact.h"
#include "gestioncontact.h"
#include "interaction.h"
#include "associationinteractiontodo.h"
#include "todo.h"
#include "gestionait.h"
#include "bdd.h"


#include <QApplication>
#include <iostream>
#include <QDebug>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //Verifie si la bdd est ouverte
    if(BDD::getInstance()->getDb().isOpen()){
        w.show();
    }

    return a.exec();
}
