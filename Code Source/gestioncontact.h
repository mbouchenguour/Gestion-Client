#ifndef GESTIONCONTACT_H
#define GESTIONCONTACT_H
#include <list>
#include "contact.h"

/**
 * @brief Classe qui permet de stocker et manipuler la liste des contacts
 */
class GestionContact
{
private :

    /**
     * @brief la liste des contacts
     */
    std::list<Contact *> contacts;

    /**
     * @brief la date de la dernière suppression d'un contact
     */
    GestionDate * dateSuppression;

    /**
     * @brief Affecte à dateSuppression la date du jour
     * Methode privé car on modifie la date de supression seulement si un setter est appelé
     */
    void setDateSuppression();

public:
    GestionContact(GestionDate * const);
    ~GestionContact();
    void setContacts(const std::list<Contact *> &);
    std::list<Contact *> getContacts() const;
    GestionDate * getDateSuppression() const;
	unsigned int getNbContact() const;
    void addContact(Contact * const);
    Contact * getById(const unsigned);
	std::list<Contact *> getAllByName(const std::string &) const;
    std::list<Contact *> getAllByEntreprise(const std::string &) const;
    std::list<Contact *> getByDate(GestionDate * const) const;
    std::list<Contact *> getByDatesIntervalle(GestionDate * const, GestionDate * const) const;
    
    void removeByIdContact(const unsigned);

    friend std::ostream& operator<<(std::ostream &, const GestionContact &);
};

#endif // GESTIONCONTACT_H
