#include "gestioncontact.h"
#include "gestionait.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implémentation du constructeur avec la date de suppression mis en paramètre
 * @param dateSuppression : pointeur vers une instance GestionDate correspondant à la date de suppression
 */
GestionContact::GestionContact(GestionDate * const dateSuppression)
{
    this->dateSuppression = dateSuppression;
}

/**
 * @brief implémentation du destructeur, vide la liste std::list<contacts>
 */
GestionContact::~GestionContact()
{
    this->contacts.clear();
}

/**
 * @author Mohamed
 * @date décembre 2022
 * @brief mutateur "contacts"
 * @param lContacts : la liste std::list contenant les pointeurs des instances Contacts
 * @return la liste std::list contenant les adresses des instances Contact
 */
void GestionContact::setContacts(const std::list<Contact *> & lContacts){
    this->contacts = lContacts;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "contacts"
 * pas de paramètre, fonction const
 * @return la liste std::list contenant les adresses des instances Contact
 */
std::list<Contact * > GestionContact::getContacts() const
{
    return this->contacts;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "dateSuppression" 
 * pas de paramètre, fonction const
 * @return le pointeur de l'instance GestionDate qui correspond à la dernière date de suppression d'un contact
 */
GestionDate * GestionContact::getDateSuppression() const{
    return this->dateSuppression;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief retourne le nombre de contact
 * @return le nombre de contact
 */
unsigned int GestionContact::getNbContact() const
{
    return this->contacts.size();
}



/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Ajoute un contact à la liste std::list contacts
 * @param contact : l'adresse de l'instance Contact à ajouter
 */
void GestionContact::addContact(Contact * const contact)
{
    this->contacts.push_back(contact);
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et retourne un Contact à l'aide de son id
 * Renvoie nullptr si le contact n'est pas trouvé
 * @return Contact * : le pointeur de l'instance Contact correspondant
 */
Contact * GestionContact::getById(const unsigned idContact)
{
    Contact * contact = nullptr;
    auto it = contacts.begin();
    bool found = false ;
    while (( it != contacts.end () ) && ( found == false ) ) {
        if (  (*it)->getIdContact() == idContact)
        {
            found = true;
            contact = &(**it);
        }
        else
            ++it ;
    }
    return contact;
}







/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Retourne une list contenant tous les contacts appartenant à l'entreprise passé en paramètre
 * @param entreprise : la chaine de caractère std::string (const) qui correspond à l'entreprise
 * @return la liste des pointeurs des instances Contact appartenant à l'entreprise
 */
std::list<Contact * > GestionContact::getAllByName(const std::string & name) const{
    std::list<Contact * > lcontact;
    for(Contact * c : this->contacts)
    {
        if(c->getNom() == name)
            lcontact.push_back(c);
    }
    return lcontact;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et retourne les contacts crées à une date spécific
 * @param dateC : pointeur d'un GestionDate (const), contenant la date de la création
 * @return la liste des pointeurs des instances Contact crée à la date passé en paramètre
 */
std::list<Contact *> GestionContact::getByDate(GestionDate * const dateC) const
{
    std::list<Contact *> lcontact;
    for(Contact * c : this->contacts)
    {
        if(c->getDateCreation()->getDate() == dateC->getDate())
            lcontact.push_back(c);
    }
    return lcontact;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Retourne une list contenant tous les contacts appartenant à l'entreprise passé en paramètre
 * @param entreprise : la chaine de caractère std::string (const) qui correspond à l'entreprise
 * @return la liste des pointeurs des instances Contact appartenant à l'entreprise
 */
std::list<Contact * > GestionContact::getAllByEntreprise(const std::string & entreprise) const{
    std::list<Contact * > lcontact;
    for(Contact * c : this->contacts)
    {
        if(c->getEntreprise() == entreprise)
            lcontact.push_back(c);
    }
    return lcontact;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et retourne les contacts crées dans une intervalle de date
 * @param dateDebut : instance GestionDate, date de début de l'intervalle
 * @param dateFin : instance GestionDate, date de fin de l'intervalle
 * @return la liste des pointeurs des instances Contact crée dans l'intervalle de date passé en paramètre
 */
std::list<Contact *> GestionContact::getByDatesIntervalle(GestionDate * const dateDebut, GestionDate * const dateFin) const
{
    std::list<Contact *> lcontact;
    for(Contact * c : this->contacts)
    {
        if(c->getDateCreation()->getDate() >= dateDebut->getDate() && c->getDateCreation()->getDate() <= dateFin->getDate())
            lcontact.push_back(c);
    }
    return lcontact;
}





/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Cherche et supprime un contact par son id
 * @param idContact : l'unsigned qui correspond à l'id du contact que l'on souhaite supprimer
 */
void GestionContact::removeByIdContact(const unsigned idContact)
{
    auto it = this->contacts.begin();
    bool found = false ;
    while (( it != this->contacts.end () ) && ( found == false ) ) {
        if ( (*it)->getIdContact() == idContact )
        {
            found = true;
            (*it)->getGestionI()->getTodos();
            GestionAIT::getInstance()->removeAssociationsByInteractions((*it)->getGestionI()->getInteractions());
            it = this->contacts.erase(it);
            this->setDateSuppression();
        }
        else
            ++it ;
        }
}








/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "dateModification", modifie dateSuppressions à la date du jour
 * Méthode privé, appelé lorsqu'un contact est supprimé
 * Initialise la date à la date su jour si dateSuppression était null
 */
void GestionContact::setDateSuppression(){
        if(dateSuppression == nullptr)
            dateSuppression = new GestionDate();
        else
            dateSuppression->setDateDefault();
}



/**
 * @author Mohamed
 * @date octobre 2022
 * @brief affiche la liste des contacts
 */
std::ostream& operator<<(std::ostream &os, const GestionContact& liste)
{
    if(liste.getDateSuppression() == nullptr)
        os << "Il n'y a pas de date de suppression" << std::endl;
    else
        os << "Date suppression : " << liste.getDateSuppression()->getDateString() << std::endl;

    for(Contact * c : liste.getContacts()){
        os << *c << std::endl;
    }

    return os;
}
