#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "Racun.h"

class UcitajRacun : Racun
{
	std::vector<Racun> validniRacuni;
	std::ifstream& currency;
	void format1(std::ifstream&, std::string); // funkcija za ucitavanje racuna koji ima format1
	void format2(std::ifstream&, std::string); // funckija za ucitavanje racuna koji ima format2
	int provjeraFormata(std::string);
	bool provjeraJedinstvenosti(Racun&, Racun&); // provjera da li je racun prethodno obradjen
	void ucitajIzFajla(); // ucitavanje vec obradjenih racuna iz fajla validniRacuni
public:
	UcitajRacun(std::ifstream&); // konstruktor prihvata fajl u kome se nalazi valuta sistema
	void ucitajRacun(std::ifstream&, std::string); // prihvata fajl sa valutom sistema i naziv racuna kao argumente
	void upisiValidne(); // upisivanje obradjenih i validnih racuna u fajl validniRacuni
	void pregledPodatakaZaProizvod(std::string); // funkcija prihvata naziv zeljenog proizvoda kao argument
	~UcitajRacun();
};

