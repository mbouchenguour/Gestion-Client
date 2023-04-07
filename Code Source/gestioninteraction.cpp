#include "gestioninteraction.h"
#include "gestionait.h"

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Constructeur par defaut
 */
GestionInteraction::GestionInteraction()
{

}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "interactions"
 * pas de paramètre, fonction const
 * @return la liste std::list contenant les Interactions
 */
std::list<Interaction> GestionInteraction::getInteractions() const
{
    return this->interactions;
}

/**
 * @author Mohamed
 * @date decembre 2022
 * @brief mutateur "interactions"
 * @param lInteractions : la liste std::list contenant les Interactions
 */
void GestionInteraction::setInteractions(const std::list<Interaction> & lInteractions)
{
    this->interactions = lInteractions;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Récupère les Todo correspondant à la liste d'Interaction "interactions"
 * Récupère la liste des Todo à l'aide de la fonction getTodosByInteractions du singleton GestionAIT
 * @return la liste std::list contenant les pointeurs des Todo
 */

std::list<Todo *> GestionInteraction::getTodos() const
{
    return GestionAIT::getInstance()->getTodosByInteractions(this->interactions);
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et retourne une Interaction à l'aide de son id
 * Renvoie nullptr si l'interaction n'est pas trouvé
 * @param idInteraction : l'unsigned de l'Interaction que l'on souhaite récupérer
 */

Interaction * GestionInteraction::getInteractionById(const unsigned idInteraction){
    Interaction * interaction = nullptr;
    auto it = this->interactions.begin();
    bool found = false ;
    while (( it != this->interactions.end () ) && ( found == false ) ) {
        if ( it->getIdInteraction() == idInteraction)
        {
            found = true;
            interaction = &(*it);
        }
        else
            ++it ;
    }
    return interaction;
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Cherche et retourne les intéractions ajoutées à une date spécific
 * @param dateAjout : pointeur d'un GestionDate (const), contenant la date d'ajout voulu
 * @return la liste std::list des instances des Interaction ajoutées à la date passé en paramètre
 */
std::list<Interaction> GestionInteraction::getInteractionsByDate(GestionDate * const dateAjout) const{
    std::list<Interaction> lInteraction;
    for(Interaction i : this->interactions)
    {
        if(i.getDateAjout()->getDate() == dateAjout->getDate())
            lInteraction.push_back(i);
    }
    return lInteraction;
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief Cherche et retourne les interactions ajoutées dans une intervalle de date
 * @param dateDebut : pointeur d'une GestionDate GestionDate (const), date de début de l'intervalle
 * @param dateFin : pointeur d'une GestionDate  (const), date de fin de l'intervalle
 * @return la liste des pointeurs des Interaction crée dans l'intervalle de date passé en paramètre
 */
std::list<Interaction> GestionInteraction::getInteractionsByIntervalle(GestionDate * const dateDebut, GestionDate * const dateFin) const
{
    std::list<Interaction> linteraction;
    for(Interaction i : this->interactions)
    {
        if(i.getDateAjout()->getDate() >= dateDebut->getDate() && i.getDateAjout()->getDate() <= dateFin->getDate())
            linteraction.push_back(i);
    }
    return linteraction;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief ajoute une interaction à la liste d'Interaction (std::list<Interaction> interactions)
 * @param interaction : l'instance Interaction que l'on souhaite ajouter à la liste

 */
void GestionInteraction::addInteraction(const Interaction & interaction)
{
    this->interactions.push_back(interaction);
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief supprime une interaction de la liste d'Interaction (std::list<Interaction> interactions)
 * Supprime également les AssociationInteractionTodo contenant cette Interaction de la liste d'AssociationInteractionTodo du singleton GestionAIT
 * @param interaction : l'instance Interaction que l'on souhaite supprimer de la liste
 */

void GestionInteraction::removeInteractionById(const unsigned idInteraction){
    auto it = interactions.begin();
    bool found = false ;
    while (( it != interactions.end () ) && ( found == false ) ) {
        if ( it->getIdInteraction() == idInteraction )
        {
            found = true;
            it = interactions.erase(it);
            GestionAIT::getInstance()->removeAssociationsByIdInteraction(idInteraction);
        }
        else
            ++it ;
        }
}




/**
 * @author Mohamed
 * @date octobre 2022
 * @brief affiche la liste des interactions de GestionInteraction
 */
std::ostream& operator<<(std::ostream &os, const GestionInteraction& liste)
{
    for(Interaction i : liste.getInteractions()){
        os << i << std::endl;
    }

    return os;
}
