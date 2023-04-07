#ifndef RECHERCHEINTERACTIONS_H
#define RECHERCHEINTERACTIONS_H

#include <QWidget>
#include "gestioncontact.h"

/**
 * @brief Classe qui affiche toutes les interactions de l'application
 */
namespace Ui {
class RechercheInteractions;
}

class RechercheInteractions : public QWidget
{
    Q_OBJECT

public:
    explicit RechercheInteractions(QWidget *parent = nullptr);
    ~RechercheInteractions();

private:
    Ui::RechercheInteractions *ui;
    void remplirTable(const std::list<Interaction> &);
    GestionInteraction * gestionI;

private slots:
    void lancerRecherche();
    void resetRecherche();
    void dateAjoutCheckB();
    void dateIntervalleCheckB();
    void initialiser(GestionContact *);

};

#endif // RECHERCHEINTERACTIONS_H
