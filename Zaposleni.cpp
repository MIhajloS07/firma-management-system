#include <iostream>
#include "Zaposleni.h"
#include <string>

Zaposleni::Zaposleni(int id, std::string ime, float plata) {
	this->id = id;
	this->ime = ime;
	this->plata = plata;
}

Zaposleni::~Zaposleni() = default;

void Zaposleni::setId(int id) { this->id = id; }

void Zaposleni::setIme(std::string ime) { this->ime = ime; }

void Zaposleni::setPlata(float plata) { this->plata = plata; }

int Zaposleni::getId() const { return this->id; }

std::string Zaposleni::getIme() const { return this->ime; }

float Zaposleni::getPlata() const { return this->plata; }

void Zaposleni::prikaziInfo() const {
	std::cout << "\n == Osnovne informacije ==" << std::endl;
	std::cout << "ID: " << this->id << " | Name: " << this->ime << " | Plata: " << this->plata << "din ";
}

void Zaposleni::povecajPlatu(float iznos) { this->plata += iznos; }

