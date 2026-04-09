#pragma once
#include "Zaposleni.h"
class Menadzer : public Zaposleni {
private:
    float bonus;
public:
    Menadzer(int id, std::string ime, float plata, float bonus);
    void setBonus(float bonus);
    float getBonus() const;
    void prikaziInfo() const override;
    void povecajPlatu(float iznos) override;
};