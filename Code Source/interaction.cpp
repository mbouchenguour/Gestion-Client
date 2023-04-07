#include "interaction.h"
#include <iostream>

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implémentation du constructeur
 * @param idInteraction : l'entier unsigned (const) correspondant à l'id de l'interaction
 * @param contenuInteraction : la chaine de caractères std::string(const) correspondant au contenu de l'interaction
 * @param dateAjout : le pointeur de l'instance GestionDate(const), correspondant à la date d'ajout de l'interaction
 */
Interaction::Interaction(const unsigned idInteraction, const std::string & contenuInteraction, GestionDate * const dateAjout)
{
    this->idInteraction = idInteraction;
    this->contenuInteraction = contenuInteraction;
    this->dateAjout = dateAjout;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "idInteraction" 
 * pas de paramètre, fonction const
 * @return l'unsigned qui correspond à l'id de l'interaction
 */
unsigned Interaction::getIdInteraction() const
{
    return this->idInteraction;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "contenuInteraction"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond au contenu de l'interaction
 */

std::string Interaction::getContenuInteraction() const
{
    return this->contenuInteraction;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief accesseur "dateAjout" 
 * pas de paramètre, fonction const
 * @return le pointeur de l'instance GestionDate qui correspond à la date d'ajout de l'interaction
 */
GestionDate * Interaction::getDateAjout() const
{
    return this->dateAjout;
}


/**
 * @author Mohamed
 * @date décembre 2022
 * @brief implementaion du mutateur "idInteraction", utiliser après l'insertion dans la bdd pour avoir le bon idInteraction
 * @param id : l'entier non signé (const), il remplacera l'"idAssociation" courant
 */
void Interaction::setIdInteraction(const unsigned id){
    this->idInteraction = id;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implementaion du mutateur "contenuInteraction",
 * @param contenuInteraction : la chaine de caractères std::string(const), elle remplacera le contenu de l'interaction courant
 */
void Interaction::setContenuInteraction(const std::string & contenuInteraction)
{
    this->contenuInteraction = contenuInteraction;
}

/**
 * @author Mehdi
 * @date octobre 2022
 * @brief implementaion du mutateur "dateAjout",
 * @param dateAjout : le pointeur de l'instance GestionDate contenant une date, elle remplacera la date d'ajout
 */
void Interaction::setDateAjout(GestionDate * const dateAjout)
{
    this->dateAjout->setDate(dateAjout->getDate());
}



/**
 * @author Mehdi
 * @date octobre 2022
 * @brief affiche les infos de l'interaction: id, contenu, date.
 * @param os
 * @param Interaction
 * @return toutes les infos de l'interaction
 */
std::ostream& operator<<(std::ostream &os, const Interaction& interaction)
{
    os << "Id interaction: " << interaction.getIdInteraction() << std::endl;
    os << "Contenu interaction: " << interaction.getContenuInteraction() << std::endl;
    os << "Date interaction: " << *interaction.getDateAjout() << std::endl;
    return os;
}
