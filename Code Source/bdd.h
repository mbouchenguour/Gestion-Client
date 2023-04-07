#ifndef BDD_H
#define BDD_H

#include <QtSql/QSqlDatabase>
#include "gestioncontact.h"
#include "associationinteractiontodo.h"

/**
 * @brief Classe qui permet d'accéder et manipuler la base de donées
 */
class BDD
{

private:
    static BDD* instance;
    QSqlDatabase db;
    BDD();

public:
    static BDD* getInstance();
    GestionContact* getContacts();
    unsigned ajoutContact(Contact * const) const;
    void modifierContact(Contact * const);
    void supprimerContact(const unsigned);
    unsigned ajouterInteraction(Interaction * const, const unsigned) const;
    unsigned ajouterTodo(Todo * const) const;
    unsigned ajouterAssociation(AssociationInteractionTodo * const) const;
    void setDb(const std::string &);
    QSqlDatabase getDb() const;

    QJsonArray contactsToJson() const;
    QJsonArray interactionsToJson() const;
    QJsonArray todosToJson() const;
    QJsonArray associationsToJson() const;


};

#endif // BDD_H
