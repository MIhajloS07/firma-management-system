#include "Menadzer.h"
#include <iostream>
#include <string>

Menadzer::Menadzer(int id, std::string ime, float plata, float bonus)
	: Zaposleni(id, ime, plata) {
	this->bonus = bonus;
}

void Menadzer::setBonus(float bonus) { this->bonus = bonus; }

float Menadzer::getBonus() const { return this->bonus; }

void Menadzer::prikaziInfo() const{
	Zaposleni::prikaziInfo();
	std::cout << " | Bonus: " << this->bonus;
}

void Menadzer::povecajPlatu(float iznos) {
	plata += (iznos + this->bonus);
}
