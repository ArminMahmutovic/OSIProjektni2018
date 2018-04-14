#include "Racun.h"
#include <iostream>
#include <iomanip>



Racun::Racun(std::ifstream& currency) : currency(currency)
{
	if (currency.is_open())
		currency >> valuta;
	else
		std::cout << std::endl << "Fajl sa valutom nije otvorena!" << std::endl;
}


Racun::~Racun()
{}