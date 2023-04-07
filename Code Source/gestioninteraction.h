#ifndef GESTIONINTERACTION_H
#define GESTIONINTERACTION_H
#include <list>
#include "interaction.h"
#include "todo.h"

/**
 * @brief Classe qui permet de stocker et manipuler les interactions d'un contact
 */
class GestionInteraction
{
private:

	/**
     * @brief la list contenant les Interaction
     */
    std::list<Interaction> interactions;

public:

    GestionInteraction();
    std::list<Interaction> getInteractions() const;
    void setInteractions(const std::list<Interaction> &);
    Interaction * getInteractionById(const unsigned);
	
    void addInteraction(const Interaction &);
    void removeInteractionById(const unsigned);
    std::list<Interaction> getInteractionsByDate(GestionDate * const) const;
    std::list<Interaction> getInteractionsByIntervalle(GestionDate * const, GestionDate * const) const;
	
	std::list<Todo *> getTodos() const;

    friend std::ostream& operator<<(std::ostream &, const GestionInteraction &);
};

#endif // GESTIONINTERACTION_H
