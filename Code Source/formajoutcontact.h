#ifndef FORMAJOUTCONTACT_H
#define FORMAJOUTCONTACT_H

#include <QWidget>
#include "contact.h"

/**
 * @brief Widget qui permet d'afficher un formulaire d'ajout de contact
 */
namespace Ui {
class FormAjoutContact;
}

class FormAjoutContact : public QWidget
{
    Q_OBJECT

public:
    explicit FormAjoutContact(QWidget *parent = nullptr);
    ~FormAjoutContact();

private:
    Ui::FormAjoutContact *ui;

private slots:
    void ajoutContact();
    void annulerAjout();
    void ajouterImage();

signals:
    void ajouterContact(Contact *);
};

#endif // FORMAJOUTCONTACT_H
