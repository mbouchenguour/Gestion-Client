#ifndef CONTACT_H
#define CONTACT_H
#include <string>
#include <list>
#include <QJsonObject>
#include <chrono>
#include "interaction.h"
#include "todo.h"
#include "gestioninteraction.h"
#include "gestiondate.h"

#include "date/date.h"
#include "interaction.h"

/**
 * @brief Classe correspondant à un contact
 */
class Contact
{

private:
    /**
     * @brief id du contact dans la base de données
     */
    unsigned idContact;

    /**
     * @brief nom
     */
    std::string nom;

    /**
     * @brief prenom
     */
    std::string prenom;

    /**
     * @brief le nom de l'entreprise
     */
    std::string entreprise;

    /**
     * @brief email
     */
    std::string mail;

    /**
     * @brief telephone du contact stocké dans une list unsigned pour éviter les mauvaises insertions
     */
    std::list<unsigned> telephone;

    /**
     * @brief URI de la photo
     */
    std::string photo;


    /**
     * @brief la date de création de la fiche contact
     */
    GestionDate * dateCreation;

    /**
     * @brief la date de la dernière modification du contact
     */
    GestionDate * dateModification;

    /**
     * @brief Classe qui permet de stocker et manipuler les interactions du contact
     */
    GestionInteraction * gestionI;

    /**
     * @brief Affecte à dateModification la date du jour si l'instance est initialisé, sinon l'initialise
     * Methode privé car on modifie la date de modification seulement si un setter est appelé
     */
    void setDateModification();



public:
    /**
     * @brief Constructeur
     */
    Contact(const unsigned, const std::string &, const std::string &, const std::string &, const std::string &, const std::list<unsigned> &, const std::string &, GestionDate * const, GestionDate * const);
    ~Contact();
    // GETTER
    unsigned getIdContact() const;
    std::string getNom() const;
    std::string getPrenom() const;
    std::string getEntreprise() const;
    std::string getMail() const;
    std::list<unsigned> getTelephone() const;
    std::string getTelephoneString() const;
    std::string getPhoto() const;
    GestionDate * getDateCreation() const;
    GestionDate * getDateModification() const;
    GestionInteraction * getGestionI() const;

    // SETTER
    void setIdContact(const unsigned);
    void setNom(const std::string &);
    void setPrenom(const std::string &);
    void setEntreprise(const std::string &);
    void setMail(const std::string &);
    void setTelephone(const std::list<unsigned> &);
    void setPhoto(const std::string &);





    friend std::ostream& operator<<(std::ostream &, const Contact &);
    bool operator==(const Contact&) const;
};

#endif // CONTACT_H
