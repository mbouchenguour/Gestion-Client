#include "todo.h"
#include <iostream>

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implémentation du constructeur
 * @param idTodo : l'entier unsigned (const) correspondant à l'id du todo
 * @param contenuTodo : la chaine de caractères std::string(const) correspondant au contenu du todo
 * @param dateRealisation : le pointeur de l'instance GestionDate(const), correspondant à la date de réalisation du todo
 */
Todo::Todo(const unsigned idTodo, const std::string & contenuTodo, GestionDate * const dateRealisation)
{
    this->idTodo = idTodo;
    this->contenuTodo = contenuTodo;
    if(dateRealisation != nullptr)
        this->dateRealisation = dateRealisation;
    else
        this->dateRealisation = new GestionDate();
}



/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "idTodo"
 * pas de paramètre, fonction const
 * @return l'unsigned qui correspond à l'id du todo
 */
unsigned Todo::getIdTodo() const
{
    return this->idTodo;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "contenuTodo"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond au contenu du todo
 */
std::string Todo::getContenuTodo() const
{
    return this->contenuTodo;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "dateRealisation"
 * pas de paramètre, fonction const
 * @return le pointeur de l'instance GestionDate qui correspond à la date de realisation
 */
GestionDate * Todo::getDateRealisation() const
{
    return this->dateRealisation;
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implementaion du mutateur "idTodo", utiliser après l'insertion dans la bdd pour avoir le bon idTodo
 * @param id : l'entier non signé (const), il remplacera l'"idTodo" courant
 */
void Todo::setIdTodo(const unsigned id)
{
    this->idTodo = id;
}


/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implementaion du mutateur "contenuTodo",
 * @param contenuTodo : la chaine de caractères std::string(const), elle remplacera le contenu du todo courant
 */
void Todo::setContenuTodo(const std::string & contenuTodo)
{
    this->contenuTodo = contenuTodo;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implementaion du mutateur "dateRealisation",
 * @param dateRealisation : le pointeur de l'instance GestionDate contenant une date, elle remplacera la date de réalisation
 */
void Todo::setDateRealisation(GestionDate * const dateRealisation)
{
    this->dateRealisation->setDate(dateRealisation->getDate());
}



/**
 * @author Mehdi
 * @date octobre 2022
 * @brief affiche les informations du todo
 * @param os
 * @param todo : Todo que l'on souhaite afficher
 * @return toutes les informations du todo
 */
std::ostream& operator<<(std::ostream &os, const Todo& todo)
{
    os << "Id todo: " << todo.getIdTodo() << std::endl;
    os << "Contenu todo: " << todo.getContenuTodo() << std::endl;
    os << "Date todo: " << *todo.getDateRealisation() << std::endl;
    return os;
}
