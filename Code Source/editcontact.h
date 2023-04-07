#ifndef EDITCONTACT_H
#define EDITCONTACT_H

#include <QWidget>
#include "contact.h"

/**
 * @brief Widget qui permet d'afficher/modifier les informations d'un contact, ses interactions/todos
 */
namespace Ui {
class EditContact;
}

class EditContact : public QWidget
{
    Q_OBJECT

public:
    explicit EditContact(QWidget *parent = nullptr);
    ~EditContact();

private:
    Ui::EditContact *ui;
    Contact * c;
    void remplirTableTodos(const std::list<Todo *> &);
    void remplirTableInteractions(const std::list<Interaction> &);

private slots:
    void initialiser(Contact *);
    void modifierContact();
    void ajoutInteraction();
    void ajouterDate();

    void rechercheInteractions();
    void resetRechercheInter();

    void rechercheTodos();
    void resetRechercheTodos();

    void dateAjoutCheckB();
    void dateIntervalleInterCheckB();

    void dateRealisationCheckB();
    void dateIntervalleTodoCheckB();

    void modifierImage();


signals:
    void contactModifier(Contact *);
};

#endif // EDITCONTACT_H
