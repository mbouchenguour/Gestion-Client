#include "contact.h"
#include "gestionait.h"
#include <iostream>
#include <QJsonObject>


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implémentation du constructeur
 * @param idContact : l'entier unsigned (const) correspondant à l'id du contact
 * @param nom : la chaine de caractères std::string(const), correspondant au nom du contact
 * @param prenom : la chaine de caractères std::string(const), correspondant au prénom du contact
 * @param entreprise : la chaine de caractères std::string(const), correspondant à l'entreprise du contact
 * @param mail : la chaine de caractères std::string(const), correspondant au mail du contact
 * @param telephone : la liste std::lisdt(const), correspondant au numéro de téléphone du contact
 * @param photo : la chaine de caractères std::string(const), correspondant à l'URI de la photo du contact
 * @param dateCreation : le pointeur de l'instance GestionDate(const), correspondant à la date de création du contact
 * @param dateModification : le pointeur de l'instance GestionDate(const), correspondant à la date de la dernière modification du contact
 */
Contact::Contact(const unsigned idContact, const std::string & nom, const std::string & prenom, const std::string & entreprise, const std::string & mail, const std::list<unsigned> & telephone, const std::string & photo, GestionDate * const dateCreation, GestionDate  * const dateModification)
{
    this->idContact = idContact;
    this->nom = nom;
    this->prenom = prenom;
    this->entreprise = entreprise;
    this->mail = mail;
    this->telephone = telephone;
    this->photo = photo;

    if(dateCreation != nullptr)
        this->dateCreation = dateCreation;
    else
        this->dateCreation = new GestionDate(); //Si aucune date n'est passé en paramètre, on initiliase dateCreation avec la date du jour

    this->dateModification = dateModification;

    this->gestionI = new GestionInteraction();
}


Contact::~Contact()
{

}


//GETTER
/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "idContact" 
 * pas de paramètre, fonction const
 * @return l'unsigned qui correspond à l'id du contact
 */
unsigned Contact::getIdContact() const
{
    return this->idContact;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "nom"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond au nom
 */
std::string Contact::getNom() const
{
    return this->nom;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "prenom"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond au prenom
 */
std::string Contact::getPrenom() const
{
    return this->prenom;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "entreprise"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond à l'entreprise
 */
std::string Contact::getEntreprise() const
{
    return this->entreprise;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "mail"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond au mail
 */
std::string Contact::getMail() const
{
    return this->mail;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "telephone"
 * pas de paramètre, fonction const
 * @return la list std::list qui correspond au telephone
 */
std::list<unsigned> Contact::getTelephone() const
{
    return this->telephone;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "telephone" en format std::string
 * pas de paramètre, fonction const
 * @return le std::string qui correspond au telephone
 */
std::string Contact::getTelephoneString() const
{
    std::string s = "";
    for(unsigned i : this->telephone) {
        s += std::to_string(i);
    }
    return s;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "photo"
 * pas de paramètre, fonction const
 * @return le std::string qui correspond à l'URI de la photo
 */
std::string Contact::getPhoto() const
{
    return this->photo;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "DateCreation"
 * pas de paramètre, fonction const
 * @return le pointeur d'une instance GestionDate qui correspond à la date de création
 */
GestionDate * Contact::getDateCreation() const
{
    return this->dateCreation;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "DateModification"
 * pas de paramètre, fonction const
 * @return le pointeur d'une instance GestionDate qui correspond à la date de la dernière modification
 */
GestionDate * Contact::getDateModification() const
{
    return this->dateModification;
}



/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "getGestionI"
 * pas de paramètre, fonction const
 * @return le pointeur d'une instance GestionInteraction qui stocke et gère les interactions du contact
 */
GestionInteraction * Contact::getGestionI() const
{
    return this->gestionI;
}


//SETTER


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "idContact", utiliser après la l'insertion de la bdd pour avoir le bon id Contact
 * @param id : l'entier non signé (const), il remplacera l'idContact courant
 */
void Contact::setIdContact(const unsigned id)
{
    this->idContact = id;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "nom", modifie dateModification à la date du jour
 * @param nom : la chaîne de caractères std::string (const), elle remplacera le nom courant
 */
void Contact::setNom(const std::string & nom)
{
    this->nom = nom;
    this->setDateModification();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "prenom", modifie dateModification à la date du jour
 * @param prenom : la chaîne de caractères std::string (const), elle remplacera le prenom courant
 */
void Contact::setPrenom(const std::string & prenom)
{
    this->prenom = prenom;
    this->setDateModification();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "entreprise", modifie dateModification à la date du jour
 * @param enreprise : la chaîne de caractères std::string (const), elle remplacera l'entreprise courant
 */
void Contact::setEntreprise(const std::string & entreprise)
{
    this->entreprise = entreprise;
    this->setDateModification();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "mail", modifie dateModification à la date du jour
 * @param mail : la chaîne de caractères std::string (const), elle remplacera le mail courant
 */
void Contact::setMail(const std::string & mail)
{
    this->mail = mail;
    this->setDateModification();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "telephone", modifie dateModification à la date du jour
 * @param telephone : la liste std::list (const), elle remplacera le telephone courant
 */
void Contact::setTelephone(const std::list<unsigned> & telephone)
{
    this->telephone.assign(telephone.begin(), telephone.end());
    this->setDateModification();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "photo", modifie dateModification à la date du jour
 * @param photo : la chaîne de caractères std::string (const), elle remplacera l'URI de la photo courante
 */
void Contact::setPhoto(const std::string & photo)
{
    this->photo = photo;
    this->setDateModification();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "dateModification", Affecte à dateModification la date du jour si l'instance est initialisé, sinon l'initialise
 * Méthode privé car on modifie la date de modification seulement si un setter est appelé
 */
void Contact::setDateModification(){
        if(dateModification == nullptr)
            dateModification = new GestionDate();
        else
            dateModification->setDateDefault();
}



/**
 * @author Mohamed
 * @date octobre 2022
 * @brief permet d'afficher les informations d'un contact
 * @param contact : l'instance Contact à afficher
 */
std::ostream& operator<<(std::ostream &os, const Contact& contact)
{
    os << "Id contact: " << contact.getIdContact() << std::endl;
    os << "Nom: " << contact.getNom() << std::endl;
    os << "Prenom: " << contact.getPrenom() << std::endl;
    os << "Entreprise: " << contact.getEntreprise() << std::endl;
    os << "Mail: " << contact.getMail() << std::endl;
    os << "Tel: " << contact.getTelephoneString() << std::endl;
    os << "Photo: " << contact.getPhoto() << std::endl;
    os << "Date Création: " << *(contact.getDateCreation()) << std::endl;
    if(contact.getDateModification() != nullptr)
        os << "Date Modification: " << *(contact.getDateModification()) << std::endl;
    else
        os << "Il n'y a pas de date de modification" << std::endl;

    return os;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Envoie true si les informations des 2 contacts sont identiques
 * @param contact : l'instance Contact a comparer
 * @return un booléen VRAI si égalité
 */
bool Contact::operator==(const Contact & c) const
{
    bool temp = false;
    if(getIdContact() == c.getIdContact())
        if ( getNom() == c.getNom())
            if ( getPrenom() == c.getPrenom() )
                if ( getEntreprise() == c.getEntreprise() )
                    if ( getMail() == c.getMail() )
                        if ( getTelephoneString() == c.getTelephoneString() )
                            if(getPhoto() == c.getPhoto())
                                if(getDateCreation()->getDateString() == c.getDateCreation()->getDateString())
                                    if(getDateModification()->getDateString() == c.getDateModification()->getDateString())
                                        temp = true;
    return temp;
}

