#ifndef TODO_H
#define TODO_H
#include <string>
#include "gestiondate.h"
#include <QJsonObject>

/**
 * @brief Classe correspondant à un Todo
 */
class Todo
{

private:

    /**
     * @brief id du Todo
     */
    unsigned idTodo;

    /**
     * @brief contenu du Todo
     */
    std::string contenuTodo;


	/**
     * @brief la date de réalisation du Todo
     */
    GestionDate * dateRealisation;


public:

    Todo(const unsigned, const std::string &, GestionDate * const);

    // GETTER
    unsigned getIdTodo() const;
    std::string getContenuTodo() const;
    GestionDate * getDateRealisation() const;

    // SETTER
    void setIdTodo(const unsigned);
    void setContenuTodo(const std::string &);
    void setDateRealisation(GestionDate * const);

    friend std::ostream& operator<<(std::ostream &, const Todo &);


};

#endif // TODO_H
