#ifndef PROJEKAT_H
#define PROJEKAT_H

#include "Zaposleni.h"
#include <string>
#include <vector>

class Projekat {
private:
    std::string naziv;
    std::string rok;
    std::vector<Zaposleni*> zaposleniNaProjektu; // Dinamicki vektor za fleksibilno upravljanje zaposlenima
public:
    Projekat(std::string naziv, std::string rok);
    void dodajZaposlenog(Zaposleni* z);
    bool ukloniZaposlenog(int id);
    void prikaziProjekat() const;
    std::string getNaziv() const;
	std::string getRok() const;
};

#endif // !PROJEKAT_H
