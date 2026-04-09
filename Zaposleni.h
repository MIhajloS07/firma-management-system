#pragma once
#include <string>
#include <iostream>

class Zaposleni {
private:
    int id;

protected:
    std::string ime;
    float plata;

public:
    Zaposleni(int id, std::string ime, float plata);
    virtual ~Zaposleni();

    void setId(int id);
    void setIme(std::string ime);
    void setPlata(float plata);

    int getId() const;
    std::string getIme() const;
    float getPlata() const;

    virtual void prikaziInfo() const;
    virtual void povecajPlatu(float iznos);
};