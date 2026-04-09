#include "Radnik.h"
#include <iostream>
#include <string>

Radnik::Radnik(std::string pozicija, int id, std::string ime, float plata) 
	: Zaposleni(id, ime, plata) {
	this->pozicija = pozicija;
}

void Radnik::setPozicija(std::string pozicija) { this->pozicija = pozicija; }

std::string Radnik::getPozicija() const { return this->pozicija; }

void Radnik::prikaziInfo() const {
	Zaposleni::prikaziInfo();
	std::cout << " | Pozicija: " << this->pozicija;
}

void Radnik::povecajPlatu(float iznos) {
	float porez = iznos * 0.15f;
	plata += (iznos - porez);
}

