#ifndef RADNIK_H
#define RADNIK_H
#include <string>
#include "Zaposleni.h"
class Radnik : public Zaposleni {
private:
    std::string pozicija;

public:
    Radnik(std::string pozicija, int id, std::string ime, float plata);

    void setPozicija(std::string pozicija);
    std::string getPozicija() const;

    void prikaziInfo() const override;
    void povecajPlatu(float iznos) override;
};
#endif // !RADNIK_H
		
