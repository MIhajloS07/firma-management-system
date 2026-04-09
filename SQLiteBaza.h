#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include "Zaposleni.h"
#include "Projekat.h"
#include "Radnik.h"
#include "Menadzer.h"

class SQLiteBaza {
private:
	std::unique_ptr<SQLite::Database> db;
public:
	SQLiteBaza(const std::string& dbName);
	
	void KreirajTabele();	

	void dodajRadnika(const Radnik& r);
	void dodajMenadzera(const Menadzer& m);	
	void dodajProjekat(const Projekat& p);	

	void ukloniZaposlenog(int id); // Uklanja zaposlenog bez obzira na tip (radnik ili menadzer)
	void ukloniProjekat(int id);

	void azurirajPlatu(int id, float novaPlata); // Azurira platu zaposlenog sa zadatim ID-em

	std::vector<std::unique_ptr<Zaposleni>> ucitajZaposlene();
	std::vector<std::unique_ptr<Projekat>> ucitajProjekte();
};