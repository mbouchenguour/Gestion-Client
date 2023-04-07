#include "gestionait.h"
#include <iostream>
#include <QJsonArray>

GestionAIT *GestionAIT::instance = nullptr;

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Contrsuteur par défaut
 * Constructeur privé, utilisé lors de l'appel de gestInstance
 */
GestionAIT::GestionAIT()
{

}
/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "instance", l'initialise s'il ne l'est pas
 * @return l'instance du singleton qui stocke la liste des AssociationInteractionTodo
 */
GestionAIT* GestionAIT::getInstance(){
    if (instance == nullptr)
    {
        instance = new GestionAIT();
    }
    return instance;
}
/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "associations" 
 * pas de paramètre, fonction const
 * @return la liste std::list contenant les AssociationsInteractionTodo

 */
std::list<AssociationInteractionTodo> GestionAIT::getAssociations() const
{
    return this->associations;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et récupère tous les Todo de l'application
 * @return std::list<Todo *> : list contenant les pointeurs des Todo
 */
std::list<Todo *> GestionAIT::getAllTodos() const
{
    std::list<Todo *> todos;
    for(AssociationInteractionTodo a : this->associations){
        todos.push_back(a.getTodo());
    }
    todos.sort([](const Todo *t1, const Todo *t2){return t1->getDateRealisation()->getDate() < t2->getDateRealisation()->getDate();});
    return todos;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et récupère toutes les Interaction de l'application sans avoir de doublons
 * @return std::list<Interaction *> : list contenant les pointeurs des Interaction
 */
std::list<Interaction *> GestionAIT::getAllInteractions() const
{
    std::list<Interaction *> interactions;
    for(AssociationInteractionTodo a : this->associations)
        interactions.push_back(a.getInteraction());
    auto last = std::unique(interactions.begin(), interactions.end()); //Suppression des doublons
    interactions.erase(last, interactions.end()); //Suppression des doublons
    interactions.sort([](const Interaction *i1, const Interaction *i2){return i1->getDateAjout()->getDate() < i2->getDateAjout()->getDate();});
    return interactions;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et récupère les todos correspondant a une interaction
 * @param interaction : l'id de l'instance Interaction que l'on souhaite récuperer les todos associés
 * @return std::list<Todo *> : list contenant les pointeurs des Todo
 */
std::list<Todo *> GestionAIT::getTodosByIdInteraction(const unsigned idInteraction)
{
    std::list<Todo *> todos;
    for(AssociationInteractionTodo a : this->associations){
        if(a.getInteraction()->getIdInteraction() == idInteraction)
            todos.push_back(a.getTodo());
    }
    todos.sort([](const Todo *t1, const Todo *t2){return t1->getDateRealisation()->getDate() < t2->getDateRealisation()->getDate();});
    return todos;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et récupère l'interaction correspondant a un Todo, renvoie nullptr si l'interaction n'est pas trouvé
 * @param todo : l'id de l'instance Todo que l'on souhaite récuperer l'interaction associé
 * @return Interaction * : pointeur de l'Interaction associée au Todo
 */
Interaction * GestionAIT::getInteractionByIdTodo(const unsigned idTodo)
{
    Interaction * interaction = nullptr;
    auto it = associations.begin();
    bool found = false ;
    while (( it != associations.end () ) && ( found == false ) ) {
        if ( it->getTodo()->getIdTodo() == idTodo)
        {
            found = true;
            interaction = it->getInteraction();
        }
        else
            ++it ;
        }
    return interaction;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et récupère tous les todos correspondant à une liste d'interactions
 * Nécessaire pour récuperer la liste des Todo d'un contact
 * @param interactions : list contenant les Interactions qu'on souhaite récuperer les Todos associés
 * @return std::list<Todo *> : list contenant les adresses des Todo associés aux Interactions
 */
std::list<Todo *> GestionAIT::getTodosByInteractions(const std::list<Interaction> & interactions) const
{
    std::list<Todo *> todos;
    for(Interaction interactionC : interactions){
        for(AssociationInteractionTodo a : this->associations){
            if(interactionC.getIdInteraction()== a.getInteraction()->getIdInteraction())
                todos.push_back(a.getTodo());
        }
    }
    todos.sort([](const Todo *t1, const Todo *t2){return t1->getDateRealisation()->getDate() < t2->getDateRealisation()->getDate();});
    return todos;
}


/**
 * @author Mohamed
 * @date decembre 2022
 * @brief Cherche et récupère toutes les Interaction correspondant à une liste de Todo
 * @param todos : list contenant les Todos (const) qu'on souhaite récuperer les Interaction associés
 * @return std::list<Interaction *> : list contenant les adresses des Interactions associés aux Todos
 */
std::list<Interaction *> GestionAIT::getInteractionsByTodos(const std::list<Todo *> & todos) const
{
    std::list<Interaction *> interactions;
    for(Todo * t : todos){
        for(AssociationInteractionTodo a : this->associations){
            if(t->getIdTodo()== a.getTodo()->getIdTodo())
                interactions.push_back(a.getInteraction());
        }
    }
    auto last = std::unique(interactions.begin(), interactions.end()); //Suppression des doublons
    interactions.erase(last, interactions.end()); //Suppression des doublons
    interactions.sort([](const Interaction *i1, const Interaction *i2){return i1->getDateAjout()->getDate() < i2->getDateAjout()->getDate();}); //trie par date d'ajout de l'interaction
    return interactions;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief ajoute une association à la liste des associations
 * @param association : l'instance Association que l'on souhaite ajouter
 */
void GestionAIT::addAssociation(const AssociationInteractionTodo & association)
{
    this->associations.push_back(association);
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief supprime les associations contenant l'idInteraction passé en paramètre
 * Une interaction peut avoir plusieurs todo, donc être dans plusieurs AssociationInteractionTodo, on parcours ainsi toute la liste
 * @param idInteraction : l'id de l'Interaction que l'on souhaite supprimer
 */
void GestionAIT::removeAssociationsByIdInteraction(const unsigned idInteraction)
{
    auto it = associations.begin();
    while ( it != associations.end() ) {
        if ( it->getInteraction()->getIdInteraction() == idInteraction)
        {
            it = associations.erase(it);
        }
        else
            ++it ;
        }
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Supprime toute les AssociationInteractionTodo (de std::list<AssociationIteractionTodo> associations) correspondant à une liste d'Interaction
 * Nécessaire pour supprimer les AssociationsInteractionTodo correspondant aux Interaction d'un contact lors de la suppression de ce dernier
 * @param interactions : list contenant les Interactions que l'on souhaite supprimer
 */
void GestionAIT::removeAssociationsByInteractions(const std::list<Interaction> & interactions)
{
    for(Interaction interactionC : interactions){
        this->removeAssociationsByIdInteraction(interactionC.getIdInteraction());
    }
}



/**
 * @author Mohamed
 * @date octobre 2022
 * @brief supprime l'association contenant l'idTodo passé en paramètre
 * Un Todo correspond à une seul Interaction, donc être dans une seul AssociationInteractionTodo
 * on arrête ainsi le parcours de la liste lorsqu'on a trouvé l'AssociationInteractionTodo en question
 * @param idTodo : l'id du Todo que l'on souhaite supprimer
 */
void GestionAIT::removeAssociationByIdTodo(const unsigned idTodo)
{
    auto it = associations.begin();
    bool found = false ;
    while (( it != associations.end () ) && ( found == false ) ) {
        if ( it->getTodo()->getIdTodo() == idTodo)
        {
            found = true;
            it = associations.erase(it);
        }
        else
            ++it ;
        }
}





/**
 * @author Mohamed
 * @date octobre 2022
 * @brief affiche la liste des AssociantionInteractionTodo
 */
std::ostream& operator<<(std::ostream &os, const GestionAIT& gestion)
{
    for(AssociationInteractionTodo a : gestion.getAssociations()){
        os << a << std::endl;
    }

    return os;
}


