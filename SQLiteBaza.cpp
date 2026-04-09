#include <iostream>
#include "SQLiteBaza.h"

SQLiteBaza::SQLiteBaza(const std::string& dbName) {
	try {
		// Pokusaj povezivanja sa bazom podataka, ako baza ne postoji, kreirace se nova
		db = std::make_unique<SQLite::Database>(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
		KreirajTabele();
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom povezivanja sa bazom: " << e.what() << std::endl;
	}
}	

void SQLiteBaza::KreirajTabele() {
	try {
		//  tabela Zaposleni sa dodatim poljem "tip" koje ce odredjivati da li je zaposleni radnik ili menadzer, i poljima "pozicija" i "bonus" koja ce se koristiti zavisno od tipa zaposlenog
		db->exec("CREATE TABLE IF NOT EXISTS Zaposleni ("
				 "id INTEGER PRIMARY KEY,"
				 "ime TEXT NOT NULL,"
				 "plata REAL NOT NULL,"
				 "tip TEXT NOT NULL,"
				 "pozicija TEXT,"
				 "bonus REAL);");
		// tabela Projekti sa automatski generisanim ID-em
		db->exec("CREATE TABLE IF NOT EXISTS Projekti ("
				 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
				 "naziv TEXT NOT NULL,"
				 "rok TEXT NOT NULL);");
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom kreiranja tabela: " << e.what() << std::endl;
	}
}

void SQLiteBaza::dodajRadnika(const Radnik& r) {
	try {
		SQLite::Statement query(*db, "INSERT INTO Zaposleni (id, ime, plata, tip, pozicija) VALUES (?, ?, ?, 'RADNIK', ?);"); // bonus se ne unosi za radnike, pozicija se unosi umesto bonusa
		query.bind(1, r.getId());
		query.bind(2, r.getIme());
		query.bind(3, r.getPlata());
		query.bind(4, r.getPozicija());
		query.exec(); // Izvrsi upit za dodavanje radnika u bazu
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom dodavanja radnika: " << e.what() << std::endl;
	}
}

void SQLiteBaza::dodajMenadzera(const Menadzer& m) {
	try {
		SQLite::Statement query(*db, "INSERT INTO Zaposleni (id, ime, plata, tip, bonus) VALUES (?, ?, ?, 'MENADZER', ?);"); // pozicija se ne unosi za menadzere, bonus se unosi umesto pozicije
		query.bind(1, m.getId());
		query.bind(2, m.getIme());
		query.bind(3, m.getPlata());
		query.bind(4, m.getBonus());
		query.exec(); // Izvrsi upit za dodavanje menadzera u bazu
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom dodavanja menadzera: " << e.what() << std::endl;
	}
}

void SQLiteBaza::dodajProjekat(const Projekat& p) {
	try {
		SQLite::Statement query(*db, "INSERT INTO Projekti (naziv, rok) VALUES (?, ?);"); // id se automatski generise zbog AUTOINCREMENT
		query.bind(1, p.getNaziv());
		query.bind(2, p.getRok()); 
		query.exec(); // Izvrsi upit za dodavanje projekta u bazu
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom dodavanja projekta: " << e.what() << std::endl;
	}
}

void SQLiteBaza::ukloniZaposlenog(int id) {
	try {
		SQLite::Statement query(*db, "DELETE FROM Zaposleni WHERE id = ?;"); // Ukloni zaposlenog sa zadatim ID-em
		query.bind(1, id);
		query.exec(); // Izvrsi upit za uklanjanje zaposlenog iz baze
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom uklanjanja zaposlenog: " << e.what() << std::endl;
	}
}

void SQLiteBaza::ukloniProjekat(int id) {
	try {
		SQLite::Statement query(*db, "DELETE FROM Projekti WHERE id = ?;"); // Ukloni projekat sa zadatim ID-em
		query.bind(1, id);
		query.exec(); // Izvrsi upit za uklanjanje projekta iz baze
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom uklanjanja projekta: " << e.what() << std::endl;
	}
}	

void SQLiteBaza::azurirajPlatu(int id, float novaPlata) {
	try {
		SQLite::Statement query(*db, "UPDATE Zaposleni SET plata = ? WHERE id = ?;"); // Azuriraj platu zaposlenog sa zadatim ID-em
		query.bind(1, novaPlata);
		query.bind(2, id);
		query.exec(); // Izvrsi upit za azuriranje plate zaposlenog u bazi
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom azuriranja plate: " << e.what() << std::endl;
	}
}

std::vector<std::unique_ptr<Zaposleni>> SQLiteBaza::ucitajZaposlene() {
	std::vector<std::unique_ptr<Zaposleni>> zaposleni; // Vektor pametnih pokazivaca na Zaposleni objekte
	try {
		SQLite::Statement query(*db, "SELECT * FROM Zaposleni;"); // Izvuci sve zaposlene iz baze
		while (query.executeStep()) { // Prolazi kroz svaki red rezultata
			int id = query.getColumn("id").getInt(); // Dobij ID zaposlenog
			std::string ime = query.getColumn("ime").getString(); // Dobij ime zaposlenog
			float plata = query.getColumn("plata").getDouble(); // Dobij platu zaposlenog
			std::string tip = query.getColumn("tip").getString(); // Dobij tip zaposlenog (RADNIK ili MENADZER)
			if (tip == "RADNIK") { // Ako je tip RADNIK, dobij poziciju i kreiraj Radnik objekat
				std::string pozicija = query.getColumn("pozicija").getString();
				zaposleni.push_back(std::make_unique<Radnik>(pozicija, id, ime, plata));
			}
			else if (tip == "MENADZER") { // Ako je tip MENADZER, dobij bonus i kreiraj Menadzer objekat
				float bonus = query.getColumn("bonus").getDouble();
				zaposleni.push_back(std::make_unique<Menadzer>(id, ime, plata, bonus));
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom ucitavanja zaposlenih: " << e.what() << std::endl;
	}
	return zaposleni; // Vrati vektor sa svim zaposlenima ucitanim iz baze
}

std::vector<std::unique_ptr<Projekat>> SQLiteBaza::ucitajProjekte() {
	std::vector<std::unique_ptr<Projekat>> projekti; // Vektor pametnih pokazivaca na Projekat objekte
	try {
		SQLite::Statement query(*db, "SELECT * FROM Projekti;"); // Izvuci sve projekte iz baze
		while (query.executeStep()) { // Prolazi kroz svaki red rezultata
			std::string naziv = query.getColumn("naziv").getString(); // Dobij naziv projekta
			std::string rok = query.getColumn("rok").getString(); // Dobij rok projekta
			projekti.push_back(std::make_unique<Projekat>(naziv, rok)); // Kreiraj Projekat objekat i dodaj ga u vektor
		}
	} catch (const std::exception& e) {
		std::cerr << "Greska prilikom ucitavanja projekata: " << e.what() << std::endl;
	}
	return projekti; // Vrati vektor sa svim projektima ucitanim iz baze
}