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

void Racun::setImeKupca(std::string imeKupca)
{
	this->imeKupca = imeKupca;
}

void Racun::upisiGresku(std::string poruka)
{
	std::fstream errorFile("Error.txt", std::ios::app);
	if (errorFile.is_open())
	{
		errorFile << poruka << std::endl;
		errorFile.close();
	}
	else
		std::cout << "Fajl nije otvoren!" << std::endl;
}

bool Racun::isLeap(int year)
{
	// Vrati true ako je godina djeljiva sa 4
	// i nije djeljiva sa 100
	// ili je godina djeljiva sa 400
	return (((year % 4 == 0) && (year % 100 != 0)) ||
		(year % 400 == 0));
}

bool Racun::isDateValid(int dan, int mjesec, int godina)
{
	if (godina < 0 || godina > 2018)
		return false;
	if (mjesec < 1 || mjesec > 12)
		return false;
	if (dan < 1 || dan > 31)
		return false;

	// Provjerimo prvo da li je godina prestupna ili ne
	if (mjesec == 2)
	{
		if (isLeap(godina))
			return (dan <= 29);
		else
			return (dan <= 28);
	}


	if (mjesec == 4 || mjesec == 6 || mjesec == 9 || mjesec == 11)
		return (dan <= 30);

	return true;
}


bool Racun::setDatumKupovine(std::string datum)
{
	int dan, mjesec, godina;
	dan = mjesec = godina = 0;
	int i = 0;
	while (datum[i] != '/')
	{
		dan = dan * 10 + ((int)datum[i] - 48);
		i++;
	}
	i++;
	while (datum[i] != '/')
	{
		mjesec = mjesec * 10 + ((int)datum[i] - 48);
		i++;
	}
	i++;
	while (i < datum.size())
	{
		godina = godina * 10 + ((int)datum[i] - 48);
		i++;
	}
	if (isDateValid(dan, mjesec, godina))
	{
		datumKupovine.dan = dan;
		datumKupovine.mjesec = mjesec;
		datumKupovine.godina = godina;
		return true;
	}
	else
	{
		upisiGresku("Datum nije validan!");
		return false;
	}
}

bool Racun::setProizvod(std::string nazivProizvoda, std::string sifraProizvoda, double kolicinaProizvoda, double cijenaProizvoda, double ukupno)
{
	if (cijenaProizvoda <= 0)
	{
		upisiGresku("Cijena ne moze biti negativna!");
		return false;
	}
	else if (kolicinaProizvoda < 0)
	{
		upisiGresku("Kolicina ne moze biti negativna!");
		return false;
	}
	else if (ukupno != (cijenaProizvoda*kolicinaProizvoda))
	{
		upisiGresku("Ukupno cijena za proizvod nije tacna!");
		return false;
	}
	else
	{
		proizvodi.push_back(Proizvod(nazivProizvoda, sifraProizvoda, cijenaProizvoda, kolicinaProizvoda, ukupno));
		return true;
	}
}

bool Racun::setUkupno(double ukupno, double PDVvrijednost, double ukupnoZaPlacanje)
{
	if (isRacunValidan(ukupno, PDVvrijednost, ukupnoZaPlacanje))
	{
		this->ukupno = ukupno;
		this->PDVvrijednost = PDVvrijednost;
		this->ukupnoZaPlacanje = ukupnoZaPlacanje;
		return true;
	}
	else
	{
		upisiGresku("Ukupna cijena za placanje nije tacna!");
		return false;
	}
}

bool Racun::isRacunValidan(double ukupno, double PDVvrijednost, double ukupnoZaPlacanje) const
{
	double sum = 0;
	for (auto& x : proizvodi)
		sum += (x.cijena * x.kolicina);
	if (sum != ukupno)
		return false;
	else if (ukupnoZaPlacanje != PDVvrijednost + ukupno)
		return false;
	return true;
}


Racun::~Racun()
{}