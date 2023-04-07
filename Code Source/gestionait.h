#ifndef GESTIONAIT_H
#define GESTIONAIT_H
#include "associationinteractiontodo.h"
#include "todo.h"
#include "interaction.h"
#include <list>

/**
 * @brief Singleton qui stocke toute les interactions de l'application
 */
class GestionAIT
{

private:
    static GestionAIT* instance;

    /**
     * @brief GestionAIT
     */
    GestionAIT();

    /**
     * @brief la list des AssociationInteractionTodo de l'application
     */
    std::list<AssociationInteractionTodo> associations;


public:
    static GestionAIT* getInstance();
    std::list<AssociationInteractionTodo> getAssociations() const;
    std::list<Todo *> getAllTodos() const;
    std::list<Interaction *> getAllInteractions() const;
	
    std::list<Todo *> getTodosByIdInteraction(const unsigned);
    Interaction * getInteractionByIdTodo(const unsigned);
    std::list<Todo *> getTodosByInteractions(const std::list<Interaction> &) const;
    std::list<Interaction *> getInteractionsByTodos(const std::list<Todo *> &) const;
	
	void addAssociation(const AssociationInteractionTodo &);
    void removeAssociationsByIdInteraction(const unsigned);
    void removeAssociationsByInteractions(const std::list<Interaction> & );
    void removeAssociationByIdTodo(const unsigned);


    friend std::ostream& operator<<(std::ostream &, const GestionAIT &);

};

#endif // GESTIONAIT_H
