#include "Racun.h"
#include <iostream>
#include <iomanip>



Racun::Racun(std::ifstream& currency) : currency(currency)
{
	if (currency.is_open())
		currency >> valuta;
	else
		std::cout << std::endl << "Fajl sa valutom nije otvoren!" << std::endl;
}

void Racun::setImeKupca(std::string imeKupca)
{
	this->imeKupca = imeKupca;
}

void Racun::upisiGresku(std::string poruka)
{
	std::ofstream errorFile(this->nazivRacuna);
	if (errorFile.is_open())
	{
		if (poruka.find("Datum") == 0) // ako je u poruci pronadjena rijec Datum na poziciji 0
		{
			errorFile << "Kupac: " << this->imeKupca << std::endl;
			errorFile << poruka << std::endl << std::endl;
		}
		else
		{
			errorFile << "Kupac: " << this->imeKupca << std::endl;
			errorFile << "Datum kupovine: " << this->datumKupovine.dan << "/" << this->datumKupovine.mjesec << "/" << this->datumKupovine.godina << std::endl;
			errorFile << poruka << std::endl << std::endl;
		}
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
		std::string poruka = "Datum " + datum + " nije validan!";
		upisiGresku(poruka);
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
		std::string poruka = "Ukupna cijena za proizvod " + nazivProizvoda + " nije tacna!";
		upisiGresku(poruka);
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
		upisiGresku("Ukupna cijena za placanje na racunu nije tacna!");
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

bool Racun::operator==(const Racun& other) const
{
	if (proizvodi.size() != other.proizvodi.size()) // da li se na racunu nalazi isti broj proizvoda
		return false;
	else
	{
		int c = 0;
		for (int i = 0; i < proizvodi.size(); ++i)
			if (proizvodi[i] == other.proizvodi[i]) // provjerimo da li su svi proizvodi na racunima jednaki
				++c;
		if (datumKupovine == other.datumKupovine && imeKupca == other.imeKupca && ukupno == other.ukupno && PDVvrijednost == other.PDVvrijednost && ukupnoZaPlacanje == other.ukupnoZaPlacanje && c == proizvodi.size())
			return true;
		else
			return false;
	}
}

bool Racun::Proizvod::operator==(const Proizvod& other) const
{
	return (this->naziv == other.naziv && sifra == other.sifra && cijena == other.cijena && kolicina == other.kolicina && ukupno == other.ukupno) ? true : false;
}

bool Racun::Datum::operator==(const Datum& other) const
{
	return (dan == other.dan && mjesec == other.mjesec && godina == other.godina) ? true : false;
}

void Racun::upisiRacun(std::vector<Racun> validniRacuni)
{
	std::ofstream file("ValidniRacuni.txt");
	if (file.is_open())
	{
		file << "Ukupan broj validnih racuna: " << validniRacuni.size() << std::endl;
		file << std::endl;
		for (auto& racun : validniRacuni)
		{
			file << "Kupac: " << racun.imeKupca << std::endl;
			file << "Datum kupovine: " << racun.datumKupovine.dan << "/" << racun.datumKupovine.mjesec << "/" << racun.datumKupovine.godina << std::endl;
			for (auto& x : racun.proizvodi)
				file << "Naziv proizvoda: " << x.naziv << std::endl << "Sifra proizvoda: " << x.sifra << std::endl << "Kolicina proizvoda: " << x.kolicina << std::endl << "Cijena proizvoda: " << x.cijena << std::endl << "Ukupna cijena proizvoda: " << x.ukupno << std::endl;
			file << "Ukupno: " << racun.ukupno << " " << racun.valuta << std::endl;
			file << "PDV: " << racun.PDVvrijednost << " " << racun.valuta << std::endl;
			file << "Ukupno za placanje: " << racun.ukupnoZaPlacanje << " " << racun.valuta << std::endl;
			file << std::endl;
		}
		file.close();
	}
	else
		std::cout << "Fajl nije otvoren!" << std::endl;
}


int Racun::ispisiPodatkeZaProizvod(Racun& racun, std::string nazivProizvoda)
{
	int brojac = 0; // sluzi za provjeru da li na racunu postoji proizvod koji trazimo
	for (auto& x : racun.proizvodi) // prolazimo kroz niz proizvoda
	{
		if (x.naziv == nazivProizvoda) // ako su nazivi proizvoda isti ispisujemo proizvod
		{
			brojac++;
			std::cout << "Datum kupovine: " << racun.datumKupovine.dan << "/" << racun.datumKupovine.mjesec << "/" << racun.datumKupovine.godina << std::endl;
			std::cout << "          Kolicina: " << x.kolicina << std::endl;
			std::cout << "          Cijena: " << x.cijena << " " << valuta << std::endl;
			std::cout << "          Ukupno: " << x.ukupno << std::endl << std::endl;
		}
	}
	return brojac;
}

void Racun::setNazivRacuna(std::string nazivRacuna)
{
	nazivRacuna.erase(nazivRacuna.end() - 4, nazivRacuna.end()); // obrisemo .txt iz naziva racuna
	this->nazivRacuna = nazivRacuna + "_error.txt";
}

void Racun::pregledUkupneProdajeZaMjesec(int mjesec, int godina, std::vector<Racun>& racuni)
{
	std::vector<Proizvod> pomocna; // pomocni niz proizvoda
	double suma = 0; // ukupna cijena prodaje za mjesec bez PDV-a
	for (auto& racun : racuni) // prolazimo kroz niz racuna
	{
		if (racun.datumKupovine.mjesec == mjesec && racun.datumKupovine.godina == godina)
		{
			suma += racun.ukupno; // na sumu dodamo ukupnu cijenu svih proizvoda na racunu bez PDV-a
			for (auto& x : racun.proizvodi) // prolazimo kroz niz proizvoda sa racuna
			{
				if (pomocna.size() > 0) // ako niz "pomocna" nije prazan
				{
					int brojac = 0; // provjera da li smo na vise razlicitih racuna pronasli isti proizvod
					for (auto& y : pomocna) // prolazimo kroz niz proizvoda koje smo vec dodali u niz "pomocna"
					{
						if (y.naziv == x.naziv && y.sifra == x.sifra) // ako se proizvod vec nalazi u nizu
						{											 // povecamo kolicinu, cijenu i ukupno
							y.kolicina += x.kolicina;				// za taj proizvod
							y.cijena += x.cijena;
							y.ukupno += x.ukupno;
							brojac++; // uvecamo brojac
						}
					}
					if (brojac == 0) // ako se proizvod ne nalazi u nizu dodamo ga u niz
						pomocna.push_back(x);
				}
				else
					pomocna.push_back(x);
			}
		}
	}
	if (pomocna.size() > 0) // ako niz pomocna nije prazan ispisemo podatke ukupne prodaje za mjesec
	{
		std::cout << "Naziv" << std::setw(10) << "kolicina" << std::setw(10) << "cijena" << std::setw(10) << "ukupno" << std::endl;
		for (auto& proizvodi : pomocna)
			std::cout << proizvodi.naziv << std::setw(10) << proizvodi.kolicina << std::setw(10) << proizvodi.cijena << std::setw(10) << proizvodi.ukupno << std::endl;
		std::cout << std::endl;
		std::cout << "Ukupno: " << suma << " " << valuta << std::endl;
		std::cout << "PDV: " << suma * PDV << " " << valuta << std::endl;
		std::cout << "Ukupno za placanje: " << suma * PDV + suma << " " << valuta << std::endl;
		std::cout << std::endl;
	}
	else // ako je niz prazan ispisemo odgovarajucu poruku
		std::cout << std::endl << "Nema podataka za ovaj mjesec!" << std::endl << std::endl;

}

void Racun::ispisiRacunZaKupca(Racun& racun)
{
	std::cout << "Datum kupovine: " << racun.datumKupovine.dan << "/" << racun.datumKupovine.mjesec << "/" << racun.datumKupovine.godina << std::endl;
	std::cout << "Proizvod" << std::setw(7) << "sifra" << std::setw(10) << "kolicina" << std::setw(10) << "cijena" << std::setw(10) << "ukupno" << std::endl;
	for (auto& x : racun.proizvodi)
		std::cout << x.naziv << std::setw(10) << x.sifra << std::setw(10) << x.kolicina << std::setw(10) << x.cijena << std::setw(10) << x.ukupno << std::endl;
	std::cout << std::endl << "Ukupno: " << racun.ukupno << " " << valuta << std::endl;
	std::cout << "PDV: " << racun.PDVvrijednost << " " << valuta << std::endl;
	std::cout << "Ukupno za placanje: " << racun.ukupnoZaPlacanje << " " << valuta << std::endl;
	std::cout << std::endl;
}

std::string Racun::getImeKupca()
{
	return imeKupca;
}

Racun::~Racun()
{}