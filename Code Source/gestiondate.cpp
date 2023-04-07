#include "gestiondate.h"
#include <sstream>



/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Constructeur par defaut, initialise dp avec la date du jour
 */

GestionDate::GestionDate()
{
    this->date = floor<days>(system_clock::now());
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Constructeur de la date avec le jour, le mois et l'année voulu
 * @param jour : le jour de la date
 * @param mois : le mois de la date
 * @param annee : l'année de la date
 */
GestionDate::GestionDate(const unsigned jour, const unsigned mois, const unsigned annee)
{
    this->date = year(annee)/month(mois)/day(jour);
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "date"
 * pas de paramètre, fonction const
 * @return la date en format day_point
 */

day_point GestionDate::getDate() const
{
    return this->date;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "date" en String
 * pas de paramètre, fonction const
 * @return le std::string qui correspond à la date en format "JJ/MM/AAAA"
 */
std::string GestionDate::getDateString() const
{
    std::stringstream s;
    auto ymd = year_month_day{date};
    if( 0 < static_cast<unsigned>(ymd.day()) && static_cast<unsigned>(ymd.day()) < 10)
        s << "0" << static_cast<unsigned>(ymd.day());
    else
        s << static_cast<unsigned>(ymd.day());

    s << "/";

    if( 0 < static_cast<unsigned>(ymd.month()) && static_cast<unsigned>(ymd.month()) < 10)
        s << "0" << static_cast<unsigned>(ymd.month());
    else
        s << static_cast<unsigned>(ymd.month());

    s << "/";

    s << static_cast<int>(ymd.year());

    return s.str();

}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief accesseur "date" en String pour la bdd
 * pas de paramètre, fonction const
 * @return le std::string qui correspond à la date en format "AAAA-MM-JJ"
 */
std::string GestionDate::getDateBDD() const
{
    std::stringstream s;
    auto ymd = year_month_day{date};
    s << static_cast<int>(ymd.year());

    s << "-";

    if( 0 < static_cast<unsigned>(ymd.month()) && static_cast<unsigned>(ymd.month()) < 10)
        s << "0" << static_cast<unsigned>(ymd.month());
    else
        s << static_cast<unsigned>(ymd.month());

    s << "-";

    if( 0 < static_cast<unsigned>(ymd.day()) && static_cast<unsigned>(ymd.day()) < 10)
        s << "0" << static_cast<unsigned>(ymd.day());
    else
        s << static_cast<unsigned>(ymd.day());

    return s.str();
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "date", affecte à "date" la date du jour
 * pas de paramètre
 */

void GestionDate::setDateDefault()
{
    this->date = floor<days>(system_clock::now());
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "date"
 * @param dp : le day_point (const), il remplacera l'attribut "date" courant
 */
void GestionDate::setDate(const day_point & dp)
{
    this->date = dp;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief implementaion du mutateur "date" avec le jour, le mois et l'année voulu
 * @param jour : l'unsigned qui remplacera le jour courant
 * @param mois : l'unsigned qui remplacera le mois courant
 * @param annee : l'unsigned qui remplacera l'année courante
 */

void GestionDate::setDateInt(const unsigned jour, const unsigned mois, const unsigned annee)
{
    auto ymd2 = day(jour)/month(mois)/year(annee);
    this->date = ymd2;
}

/**
 * @brief Permet de comparer 2 dates
 * @param date : la date que l'on souhaite comparer
 * @return un Booleen Vrai si les 2 dates sont identiques
 */
bool GestionDate::operator==(const GestionDate & date) const
{
    bool temp = false;
    if(date.getDateString().compare(getDateString()) == 0)
        temp = true;
    return temp;
}

/**
 * @author Mohamed
 * @date octobre 2022
 * @brief Permet de comparer un date avec un std::string
 * @param stringDate : le std::string correspondant à la date
 * @return un Booleen Vrai si la date et le string sont identiques
 */
bool GestionDate::operator==(const std::string & stringDate) const
{
    bool temp = false;
    if(getDateString().compare(stringDate) == 0)
        temp = true;
    return temp;
}


/**
 * @author Mohamed
 * @date octobre 2022
 * @brief affiche la date
 * @param os
 * @param date
 * @return la date
 */

std::ostream& operator<<(std::ostream & os, const GestionDate & date)
{
    os << date.getDateString();

    return os;
}
