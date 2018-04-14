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
public:
	UcitajRacun(std::ifstream&); // konstruktor prihvata fajl u kome se nalazi valuta sistema
	~UcitajRacun();
};

