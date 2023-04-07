#include "associationinteractiontodo.h"
#include <iostream>

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implémentation du constructeur
 * @param idAssociation : l'unsigned (const) correspondant à l'id de l'AssociationInteractionTodo
 * @param t : le pointeur (const) d'un Todo
 * @param i : le pointeur (const) d'une Interaction
 */
AssociationInteractionTodo::AssociationInteractionTodo(const unsigned idAssociation, Todo * const t, Interaction * const i) 
{
    this->idAssociation = idAssociation; //l'affectaion de id association 
    this->todo = t; //l'affectaion du todo
    this->interaction = i; //l'affectaion de l'interaction
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief destructeur par defaut
 */
AssociationInteractionTodo::~AssociationInteractionTodo()
{

}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implementaion du mutateur "idAssociation", utiliser après l'insertion dans la bdd pour avoir le bon idAssociation
 * @param idA : l'entier non signé (const), il remplacera l'idContact courant
 */
void AssociationInteractionTodo::setIdAssociation(const unsigned idA)
{
    this->idAssociation = idA;
}


/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "idAssociation"
 * pas de paramètre, fonction const
 * @return l'unsigned qui correspond à l'id de l'association
 */
unsigned AssociationInteractionTodo::getIdAssociation() const
{
    return this->idAssociation;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "todo"
 * pas de paramètre, fonction const
 * @return Le pointeur du Todo
 */
Todo * AssociationInteractionTodo::getTodo() const {
    return this->todo;

}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "interaction"
 * pas de paramètre, fonction const
 * @return Le pointeur de l'interaction
 */
Interaction * AssociationInteractionTodo::getInteraction()const {
    return this->interaction;
}


/**
 * @author Mehdi
 * @date octobre 2022
 * @brief redéfini l'opérateur <<, affiche les informations du Todo et de l'Interaction d'une AssociationInteractionTodo
 * @param association : l'AssociationInteractionTodo que l'on souhaite afficher
 */
std::ostream& operator<<(std::ostream &os, const AssociationInteractionTodo& association)
{
    os << "Id de l'association : " << association.getIdAssociation() << std::endl;
    os << "TODO" << std::endl;
    os << *(association.getTodo()) << std::endl;
    os << std::endl;
    os << "INTERACTION" << std::endl;
    os << *(association.getInteraction()) << std::endl;
    return os;
}
