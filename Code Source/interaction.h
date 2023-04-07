#ifndef INTERACTION_H
#define INTERACTION_H
#include <string>
#include "gestiondate.h"
#include <QJsonObject>

/**
 * @brief Classe correspondant à une interaction
 */
class Interaction
{

private:
    /**
     * @brief idInteraction
     */
    unsigned idInteraction;

    /**
     * @brief contenu de l'interaction
     */
    std::string contenuInteraction;

    /**
     * @brief dateAjout
     */
    GestionDate * dateAjout;

public:

    Interaction(const unsigned, const std::string &, GestionDate * const);

    //GETTER
    unsigned getIdInteraction() const;
    std::string getContenuInteraction() const;
    GestionDate * getDateAjout() const;


    //SETTER
    void setIdInteraction(const unsigned);
    void setContenuInteraction(const std::string &);
    void setDateAjout(GestionDate * const);


    friend std::ostream& operator<<(std::ostream &, const Interaction &);



};

#endif // INTERACTION_H
