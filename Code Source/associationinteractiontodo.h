#ifndef ASSOCIATIONINTERACTIONTODO_H
#define ASSOCIATIONINTERACTIONTODO_H

#include "todo.h"
#include "interaction.h"

/**
 * @brief Classe qui permet de faire l'association entre un todo et une interaction
 */
class AssociationInteractionTodo
{
private:

    /**
     * @brief l'id de l'association dans la base de données
     */
    unsigned idAssociation;

    /**
     * @brief pointeur d'un Todo
     */
    Todo* todo;

    /**
     * @brief pointeur d'une Interaction
     */
    Interaction* interaction;

public:

    AssociationInteractionTodo(const unsigned,  Todo * const, Interaction * const);
    ~AssociationInteractionTodo();

    //SETTER
    void setIdAssociation(const unsigned);
    //GETTER
    unsigned getIdAssociation() const;
    Todo * getTodo() const;
    Interaction * getInteraction() const;


    friend std::ostream& operator<<(std::ostream &, const AssociationInteractionTodo &);
};

#endif // ASSOCIATIONINTERACTIONTODO_H
