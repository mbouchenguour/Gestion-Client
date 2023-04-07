#ifndef RECHERCHETODOS_H
#define RECHERCHETODOS_H

#include "todo.h"
#include <QWidget>

/**
 * @brief Widget qui affiche tous les todos de l'application
 */
namespace Ui {
class RechercheTodos;
}

class RechercheTodos : public QWidget
{
    Q_OBJECT

public:
    explicit RechercheTodos(QWidget *parent = nullptr);
    ~RechercheTodos();

private:
    Ui::RechercheTodos *ui;
    void remplirTable(const std::list<Todo *> &);

private slots:
    void lancerRecherche();
    void resetRecherche();
    void dateRealisationCheckB();
    void dateIntervalleCheckB();
    void ouverture();
};

#endif // RECHERCHETODOS_H
