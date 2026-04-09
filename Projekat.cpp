#include "Projekat.h"
#include <iostream>
#include <string>
#include <vector>

Projekat::Projekat(std::string naziv, std::string rok) {
	this->naziv = naziv;
	this->rok = rok;
}

void Projekat::dodajZaposlenog(Zaposleni* z) {
	//provera da li zaposleni sa tim ID postoji u projektu
	for (size_t i = 0; i < zaposleniNaProjektu.size(); i++) {
		if (zaposleniNaProjektu[i]->getId() == z->getId()) {
			std::cout << "Zaposleni sa ID " << z->getId() << " vec postoji u projektu!" << std::endl;
			return;
		}
	}
	zaposleniNaProjektu.push_back(z);
	std::cout << "Zaposleni sa ID " << z->getId() << " dodat u projekat." << std::endl;
}

bool Projekat::ukloniZaposlenog(int id) {
	for (size_t i = 0; i < zaposleniNaProjektu.size(); i++) {
		if (zaposleniNaProjektu[i]->getId() == id) {
			zaposleniNaProjektu.erase(zaposleniNaProjektu.begin() + i);
			return true;
		}
	}
	return false; // nije pronadjen zaposleni sa tim id-jem
}

void Projekat::prikaziProjekat() const {
	std::cout << "Projekat: " << this->naziv << ", rok: " << this->rok << std::endl;
	std::cout << "Zaposleni na projektu: " << zaposleniNaProjektu.size() << std::endl;
	for (size_t i = 0; i < zaposleniNaProjektu.size(); i++) { zaposleniNaProjektu[i]->prikaziInfo(); }
}

std::string Projekat::getNaziv() const { return this->naziv; }

std::string Projekat::getRok() const { return this->rok; }