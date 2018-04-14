#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#define PDV 0.17


class Racun
{
private:
	std::string imeKupca;
	std::string sifraProizvoda;
	int prodanaKolicina;
	struct Proizvod
	{
		std::string naziv;
		std::string sifra;
		double cijena;
		double kolicina;
		double ukupno;
		Proizvod(std::string naziv, std::string sifra, double cijena, double kolicina, double ukupno) : naziv(naziv), sifra(sifra), cijena(cijena), kolicina(kolicina), ukupno(ukupno) {}
	};
	std::vector<Proizvod> proizvodi;
	struct Datum
	{
		int dan, mjesec, godina;
	};
	Datum datumKupovine;
	double ukupno; // ukupna cijena svih proizvoda bez PDV-a
	double ukupnoZaPlacanje; // ukupna cijena za placanje ukljucujuci PDV
	std::ifstream& currency; // fajl u kom se nalazi valuta sistema
	std::string valuta;
	double PDVvrijednost;
public:
	Racun() = default;
	Racun(std::ifstream&); // konstruktor prihvata fajl u kom se nalazi valuta sistema
	~Racun();
};