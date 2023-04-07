#ifndef GESTIONDATE_H
#define GESTIONDATE_H

#include <chrono>
#include <iostream>

#include "date/date.h"
#include "date/chrono_io.h"

using namespace date;
using namespace std::chrono;

using day_point = std::chrono::time_point<std::chrono::system_clock, days>;

/**
 * @brief Classe qui permet de stocker une date et de la manipuler
 */
class GestionDate
{

private :
    day_point date;

public:
    GestionDate();
    GestionDate(const unsigned, const unsigned, const unsigned);
    day_point getDate() const;
    std::string getDateString() const;
    std::string getDateBDD() const;
    void setDateDefault();
    void setDate(const day_point &);
    void setDateInt(const unsigned, const unsigned, const unsigned);

    bool operator==(const GestionDate &) const;
    bool operator==(const std::string &) const;
    friend std::ostream& operator<<(std::ostream &, const GestionDate &);

};

#endif // GESTIONDATE_H
