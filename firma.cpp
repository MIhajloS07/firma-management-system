#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "SQLiteBaza.h"
#include "Radnik.h"
#include "Menadzer.h"
#include "Projekat.h"
#include "Zaposleni.h"

// ANSI escape kodovi za bojanje teksta u terminalu
namespace Color {
	const std::string C_RESET =	  "\033[0m";
	const std::string C_RED =     "\033[31m";
	const std::string C_GREEN =   "\033[32m";
	const std::string C_YELLOW =  "\033[33m";
	const std::string C_BLUE =    "\033[34m";
}

int validacijaUnosa(std::string poruka) {
	int broj; std::cout << poruka;	
	while (!(std::cin >> broj)) {
		std::cout << Color::C_RED << "Neispravan unos. Pokusaj ponovo: " << Color::C_RESET;
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	} return broj;
}

float validacijaUnosaFloat(std::string poruka) {
	float n; std::cout << Color::C_GREEN << poruka << Color::C_RESET;
	while (!(std::cin >> n) || n < 0) {
		std::cout << Color::C_RED << "Neispravan unos. Pokusaj ponovo: " << Color::C_RESET;
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	} return n;
}

bool postojiId(const std::vector<std::unique_ptr<Zaposleni>>& zaposleni, int id) {
	for (auto& z : zaposleni) { 
		if (z->getId() == id) { return true; }
	} return false;
}

void Menu() {
	std::cout << std::endl;
	std::cout << "+-------------------------------+" << std::endl;
	std::cout << "|" << Color::C_BLUE << "   --- INTERAKTIVNI MENI ---  " << Color::C_RESET << " |" <<std::endl;
	std::cout << "|  0.Izlaz                      |" << std::endl;
	std::cout << "|  1.Dodaj radnika              |" << std::endl;
	std::cout << "|  2.Dodaj menadzera            |" << std::endl;
	std::cout << "|  3.Dodaj projekat             |" << std::endl;
	std::cout << "|  4.Povecaj platu              |" << std::endl;
	std::cout << "|  5.Prikazi info               |" << std::endl;
	std::cout << "|  6.Ukloni zaposlenog          |" << std::endl;
	std::cout << "|  7.Ukloni projekat            |" << std::endl;
	std::cout << "|  8.Dodaj zaposleni(projekat)  |" << std::endl;
	std::cout << "|  9.Pretrazi zaposlenog(ime)   |" << std::endl;
	std::cout << "|  10.Pretrazi zaposlenog(id)   |" << std::endl;
	std::cout << "|  11.Pretrazi poziciju radnika |" << std::endl;
	std::cout << "|  12.Prosek plate zaposlenih   |" << std::endl;
	std::cout << "|  13.Prosek plate radnika      |" << std::endl;
	std::cout << "|  14.Prosek plate menadzera    |" << std::endl;
	std::cout << "+-------------------------------+" << std::endl;
	std::cout << std::endl;
}

int main() {
	std::vector<std::unique_ptr<Zaposleni>> zaposleni; // Dinamicki vektor za sve zaposlene 
	std::vector<std::unique_ptr<Projekat>> projekti; // Dinamicki vektor za sve projekte
	SQLiteBaza baza("firma.db"); // Objekat baze i povezivanje se sa fajlom "firma.db" (ako fajl ne postoji, bice kreiran)
	int izbor, brojRadnika = 0, brojMenadzera = 0, brojProjekta = 0;
	bool zauzet; zaposleni.clear(); /*ucitajIzFajla(zaposleni);*/

	zaposleni = baza.ucitajZaposlene(); // Ucitaj zaposlene iz baze
	projekti = baza.ucitajProjekte(); // Ucitaj projekte iz baze

	#pragma region Options
	do {
		Menu();
		std::cout << "------------------------------" << std::endl;
		izbor = validacijaUnosa("Unesi izbor: ");
		std::cout << "------------------------------" << std::endl;
		try {
			switch (izbor) {
			case 0:
				for (int i = 3; i > 0; i--) {
					std::cout << Color::C_YELLOW<< "Izlazak za " << i << "..." << Color::C_RESET << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
				break;
			case 1: {
				// Dodaj radnika
				int id;
				std::string ime, pozicija;
				float plata;
				while (true) {
					id = validacijaUnosa("ID: ");
					if (postojiId(zaposleni, id)) {
						std::cout << Color::C_RED
							<< "ID vec postoji! Pokusaj ponovo.\n"
							<< Color::C_RESET;
					} else {
						break;
					}
				} 
				std::cin.ignore(); // ciscenje buffera 
				std::cout << Color::C_GREEN << "Ime: " << Color::C_RESET; std::getline(std::cin, ime);
				plata = validacijaUnosaFloat("Plata (din): ");
				std::cin.ignore();
				std::cout << Color::C_GREEN << "Pozicija: " << Color::C_RESET; std::getline(std::cin, pozicija);				
				zaposleni.push_back(std::make_unique<Radnik>(pozicija, id, ime, plata));
				baza.dodajRadnika(Radnik(pozicija, id, ime, plata)); // Dodavanje radnika u bazu
				std::cout << "Radnik " << Color::C_YELLOW << ime << Color::C_RESET << " dodat!" << std::endl;
				break;
			}

			case 2: {
				// Dodaj menadzera
				int idM;
				std::string imeM;
				float plataM, bonus;
				while (true) {
					idM = validacijaUnosa("ID: ");
					if (postojiId(zaposleni, idM)) {
						std::cout << Color::C_RED
							<< "ID vec postoji! Pokusaj ponovo.\n"
							<< Color::C_RESET;
					}
					else {
						break;
					}
				} 
				std::cin.ignore();
				std::cout << Color::C_GREEN << "Ime: " << Color::C_RESET; std::getline(std::cin, imeM);
				plataM = validacijaUnosaFloat("Plata (din): ");
				std::cin.ignore();
				bonus = validacijaUnosaFloat("Bonus (din): ");
				std::cin.ignore();

				zaposleni.push_back(std::make_unique<Menadzer>(idM, imeM, plataM, bonus));
				baza.dodajMenadzera(Menadzer(idM, imeM, plataM, bonus)); // Dodavanje menadzera u bazu
				std::cout << "Menadzer " << Color::C_YELLOW  << imeM << Color::C_RESET << " dodat!" << std::endl;
				break;
			}
			case 3: {
				// Dodaj projekat
				std::string naziv, rok;
				std::cout << Color::C_BLUE << "Naziv projekta: " << Color::C_RESET; std::cin.ignore(); std::getline(std::cin, naziv);
				std::cout << Color::C_BLUE << "Rok projekta: " << Color::C_RESET; std::getline(std::cin, rok);
				projekti.push_back(std::make_unique<Projekat>(naziv, rok));
				baza.dodajProjekat(Projekat(naziv, rok)); // Dodavanje projekta u bazu	
				break;
			}
			case 4: {
				// Povecaj platu
				int idZaPovecanje;
				float iznos;
				std::cout << Color::C_GREEN << "Unesite ID zaposlenog: " << Color::C_RESET;
				std::cin >> idZaPovecanje;
				std::cout << Color::C_GREEN << "Unesite iznos za povecanje: " << Color::C_RESET;
				std::cin >> iznos;
				bool nadjen = false;
				for (auto& z : zaposleni) {
					if (z->getId() == idZaPovecanje) {
						z->povecajPlatu(iznos);
						baza.azurirajPlatu(idZaPovecanje, z->getPlata()); // Azuriranje plate zaposlenog u bazi
						std::cout << Color::C_GREEN << "Plata povecana!" << Color::C_RESET << std::endl;
						nadjen = true;
						break;
					}
				}
				if (!nadjen) {
					throw std::runtime_error("ZAPOSLENI SA TIM ID-jem NIJE PRONADJEN. Pokusaj ponovo.");
				}
				break;
			}
			case 5: {
				// Prikazi info
				std::cout << std::endl;
				std::cout << Color::C_YELLOW << "=== ZAPOSLENI ===" << Color::C_RESET << std::endl;
				for (auto& z : zaposleni) { z->prikaziInfo(); }
				std::cout << std::endl; std::cout << std::endl;
				std::cout << Color::C_YELLOW << "=== PROJEKTI ===" << Color::C_RESET << std::endl;
				for (auto& p : projekti) { p->prikaziProjekat(); }
				std::cout << std::endl; std::cout << std::endl;
				break;
			}
			case 6: {
				// Ukloni zaposlenog
				int idZaUklanjanje;
				std::cout << Color::C_GREEN << "Unesi ID zaposlenog koga zelis da uklonis: " << Color::C_RESET; std::cin >> idZaUklanjanje;
				bool pronadjen = false;
				for (size_t i = 0; i < zaposleni.size(); i++) {
					if (zaposleni[i]->getId() == idZaUklanjanje) {
						for (auto& p : projekti) { p->ukloniZaposlenog(idZaUklanjanje); }
						zaposleni.erase(zaposleni.begin() + i);
						pronadjen = true;
						baza.ukloniZaposlenog(idZaUklanjanje); // Uklanjanje zaposlenog iz baze
						std::cout << Color::C_GREEN << "------------------------------" << Color::C_RESET << std::endl;
						std::cout << Color::C_GREEN << "Zaposleni sa ID " << idZaUklanjanje << " uklonjen." << Color::C_RESET << std::endl;
						std::cout << Color::C_GREEN << "------------------------------" << Color::C_RESET << std::endl;
						break;
					}
				}
				break;
			}
			case 7: {
				// Ukloni projekat
				std::string nazivUnos; std::cout << Color::C_GREEN << "Unesi naziv projekta koga zelis da uklonis: " << Color::C_RESET;	
				std::cin.ignore(); std::getline(std::cin, nazivUnos);
				bool nadjenP = false;
				for (size_t i = 0; i < projekti.size(); i++) {
					if (projekti[i]->getNaziv() == nazivUnos) {
						projekti.erase(projekti.begin() + i);
						nadjenP = true;
						baza.ukloniProjekat(i + 1); // Uklanjanje projekta iz baze (id projekta je i+1 zbog AUTOINCREMENT)
						std::cout << std::endl;
						std::cout << Color::C_GREEN << "------------------------------" << Color::C_RESET << std::endl;
						std::cout << Color::C_GREEN << "Projekat /" << nazivUnos << "/ uklonjen." << Color::C_RESET << std::endl;
						break;
					}
				}
				break;
			}
			case 8: {
				// Dodaj zaposleni(projekat)
				int idZaposlenog, indexProjekta;
				std::cout << Color::C_GREEN << "Unesi ID zaposlenog: " << Color::C_RESET; std::cin >> idZaposlenog;
				std::cout << Color::C_GREEN << "Unesi redni broj projekta (0 - " << projekti.size() - 1 << "): " << Color::C_RESET; std::cin >> indexProjekta;
				if (indexProjekta >= 0 && indexProjekta < projekti.size()) {
					Zaposleni* z = nullptr;
					for (auto& zap : zaposleni) {
						if (zap->getId() == idZaposlenog) { z = zap.get(); break; }
					}
					if (z != nullptr) {
						projekti[indexProjekta]->dodajZaposlenog(z);
						std::cout << Color::C_GREEN << "------------------------------" << Color::C_RESET << std::endl;
					}
					else {
						std::cout << Color::C_RED << "Zaposleni nije pronadjen." << Color::C_RESET << std::endl;
					}
				}
				else {
					throw std::out_of_range("Neispravan indeks projekta. Pokusaj ponovo.");
				}
				break;
			}
			case 9: {
				// Pretrazi zaposlenog(ime)
				std::string trazenoIme; std::cout << Color::C_GREEN << "Unesi ime zaposlenog: " << Color::C_RESET;	
				std::cin.ignore(); std::getline(std::cin, trazenoIme);
				bool nadjenoIme = false;
				std::cout << Color::C_YELLOW << "------------------------------" << Color::C_RESET << std::endl;
				std::cout << Color::C_YELLOW << "Rezultati pretrage:" << Color::C_RESET << std::endl;
				for (auto& z : zaposleni) {
					if (z->getIme() == trazenoIme) {
						z->prikaziInfo();
						nadjenoIme = true;
					}
				}
				if (!nadjenoIme) {
					throw std::runtime_error("ZAPOSLENI SA IMENOM " + trazenoIme + " NIJE PRONADJEN. Pokusaj ponovo.");
				}
				break;
			}
			case 10: {
				// Pretrazi zaposlenog(id)
				int trazeniId; std::cout << Color::C_GREEN << "Unesi trazeni ID: " << Color::C_RESET; std::cin >> trazeniId;
				bool nadjeniID = false;
				std::cout << Color::C_YELLOW << "------------------------------" << Color::C_RESET << std::endl;
				std::cout << Color::C_YELLOW << "Rezultati pretrage:" << Color::C_RESET << std::endl;
				for (auto& z : zaposleni) {
					if (z->getId() == trazeniId) {
						z->prikaziInfo();
						nadjeniID = true;
					}
				}
				if (!nadjeniID) {
					throw std::runtime_error("ZAPOSLENI SA TIM ID-jem NIJE PRONADJEN. Pokusaj ponovo.");
				}
				break;
			}
			case 11: {
				// Pretrazi poziciju radnika
				std::string pozicija; std::cout << Color::C_GREEN << "Unesi trazenu poziciju radnika: " << Color::C_RESET;
				std::cin.ignore(); std::getline(std::cin, pozicija);
				bool nadjenaPoz = false;
				std::cout << Color::C_YELLOW << "------------------------------" << Color::C_RESET << std::endl;
				std::cout << Color::C_YELLOW << "Rezultati pretrage:" << Color::C_RESET << std::endl;
				for (auto& z : zaposleni) {
					Radnik* r = dynamic_cast<Radnik*>(z.get());
					if (r != nullptr && r->getPozicija() == pozicija) {
						r->prikaziInfo();
						nadjenaPoz = true;
					}
				}
				if (!nadjenaPoz) {
					throw std::runtime_error("NEMA RADNIKA SA POZICIJOM " + pozicija + ". Pokusaj ponovo.");
				}
				break;
			}
			case 12: {
				// Prosek plate zaposlenih
				float sumaPlata = 0;
				int ukupanBrojZaposlenih = zaposleni.size();
				for (auto& z : zaposleni) { sumaPlata += z->getPlata(); }
				if (ukupanBrojZaposlenih > 0) {
					float prosecnaPlata = sumaPlata / ukupanBrojZaposlenih;
					std::cout << Color::C_YELLOW << "Prosecna plata svih zaposlenih: " << prosecnaPlata << " din." << Color::C_RESET << std::endl;
				}
				else {
					std::cout << Color::C_YELLOW << "------------------------------" << Color::C_RESET << std::endl;
					std::cout << Color::C_YELLOW << "Nema zaposlenih trenutno..." << Color::C_RESET << std::endl;
				}
				break;
			}
			case 13: {
				// Prosek plate radnika
				float sumaRadnika = 0;
				int brojRadnika = 0;
				for (auto& z : zaposleni) {
					Radnik* r = dynamic_cast<Radnik*>(z.get());
					if (r != nullptr) {
						sumaRadnika += r->getPlata();
						brojRadnika++;
					}
				}
				if (brojRadnika > 0) {
					float prosekRadnici = sumaRadnika / brojRadnika;
					std::cout << Color::C_YELLOW << "Prosecna plata radnika: " << prosekRadnici << " din." << Color::C_RESET << std::endl;
				}
				else {
					std::cout << Color::C_YELLOW << "------------------------------" << Color::C_RESET << std::endl;
					std::cout << Color::C_YELLOW << "Trenutno nema radnika..." << Color::C_RESET << std::endl;
				}
				break;
			}
			case 14: {
				// Prosek plate menadzera
				float sumaMenadzera = 0;
				int brojMenadzera = 0;
				for (auto& z : zaposleni) {
					Menadzer* m = dynamic_cast<Menadzer*>(z.get());
					if (m != nullptr) {
						sumaMenadzera += m->getPlata();
						brojMenadzera++;
					}
				}
				if (brojMenadzera > 0) {
					float prosekMenadzeri = sumaMenadzera / brojMenadzera;
					std::cout << Color::C_YELLOW << "Prosecna plata menadzera: " << prosekMenadzeri << " din." << Color::C_RESET << std::endl;
				}
				else {
					std::cout << Color::C_YELLOW << "------------------------------" << Color::C_RESET << std::endl;
					std::cout << Color::C_YELLOW << "Trenutno nema menadzera..." << Color::C_RESET << std::endl;
				}
				break;
			}
			default:
				std::cout << Color::C_YELLOW << "Unesi izbor (broj) u opsegu od 0 do 15" << Color::C_RESET << std::endl;
				std::cout << Color::C_YELLOW << "-------------------------------------" << Color::C_RESET << std::endl;
				break;
			}
		}
		catch (std::runtime_error& e) {
			std::cout << Color::C_RED << e.what() << Color::C_RESET << std::endl;
		}
		catch (const std::out_of_range& e) {
			std::cout << Color::C_RED << e.what() << Color::C_RESET << std::endl;
		}
		catch (const std::invalid_argument& e) {
			std::cout << Color::C_RED << e.what() << Color::C_RESET << std::endl;
		}
		catch (const std::exception& e) {
			std::cout << Color::C_RED << e.what() << Color::C_RESET << std::endl;
		}
	} while (izbor != 0);
	#pragma endregion
	return 0;
}