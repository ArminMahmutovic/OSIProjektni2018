#include "UcitajRacun.h"




UcitajRacun::UcitajRacun(std::ifstream& currency) : Racun(currency), currency(currency)
{
	 ucitajIzFajla(); // pri konstrukciji objekta klase UcitajRacun odmah se ucitavaju validni racuni koji su ranije obradjeni
}

void UcitajRacun::ucitajRacun(std::ifstream& file, std::string nazivRacuna)
{
	if (file.is_open())
	{
		std::string provjera;
		file >> provjera;
		if (provjeraFormata(provjera) == 1)
			format1(file, nazivRacuna);
		else if (provjeraFormata(provjera) == 2)
			format2(file, nazivRacuna);
		else
			std::cout << "Nepoznat format!" << std::endl;
	}
	else
		std::cout << "Fajl nije otvoren!" << std::endl;
}

void UcitajRacun::upisiValidne()
{
	upisiRacun(validniRacuni);
}

void UcitajRacun::format1(std::ifstream& file, std::string nazivRacuna)
{
	bool provjera;
	if (file.is_open())
	{
		Racun racun(currency);
		racun.setNazivRacuna(nazivRacuna);
		std::vector<std::string> racuni;
		while (!file.eof())
		{
			std::string pomocna;
			file >> pomocna;
			if (pomocna != "Kupac:" && pomocna != "Datum:" && pomocna != "Racun" && pomocna != "Proizvod" && pomocna != "kolicina" && pomocna != "cijena" && pomocna != "ukupno" && pomocna != "Ukupno" && pomocna != "Ukupno:" && pomocna != "za" && pomocna != "placanje:" && pomocna != "PDV:" && pomocna != "-")
				racuni.push_back(pomocna);
		}
		file.close();
		racuni.erase(racuni.end() - 4);
		racun.setImeKupca(racuni[0]);
		provjera = racun.setDatumKupovine(racuni[1]);
		if (provjera == false)
			return;
		for (int i = 2; i < racuni.size() - 3; i += 5)
		{
			provjera = racun.setProizvod(racuni[i], racuni[i + 1], stod(racuni[i + 2]), stod(racuni[i + 3]), stod(racuni[i + 4]));
			if (provjera == false)
				return;
		}
		provjera = racun.setUkupno(stod(racuni[racuni.size() - 3]), stod(racuni[racuni.size() - 2]), stod(racuni[racuni.size() - 1]));
		if (provjera == false)
			return;
		if (validniRacuni.size() > 0)
		{
			int brojac = 0;
			for (auto& x : validniRacuni)
				if (provjeraJedinstvenosti(x, racun))
					brojac++;
			if (brojac == 0)
				validniRacuni.push_back(racun);
		}
		else
			validniRacuni.push_back(racun);
	}
	else
		std::cout << "Racun nije otvoren";
}

void UcitajRacun::format2(std::ifstream& file, std::string nazivRacuna)
{
	bool provjera;
	if (file.is_open())
	{
		Racun racun(currency);
		racun.setNazivRacuna(nazivRacuna);
		std::vector<std::string> racuni;
		std::string temp;
		getline(file, temp);
		getline(file, temp);
		getline(file, temp);
		while (!file.eof())
		{
			std::string pomocna;
			file >> pomocna;
			if (pomocna != "Kupac:" && pomocna != "Datum:" && pomocna != "racun" && pomocna != "Proizvod" && pomocna != "kolicina" && pomocna != "cijena" && pomocna != "ukupno" && pomocna != "Ukupno" && pomocna != "Ukupno:" && pomocna != "za" && pomocna != "placanje:" && pomocna != "PDV:" && pomocna != "-" && pomocna != "Maloprodajni")
				racuni.push_back(pomocna);
		}
		file.close();
		racuni.erase(racuni.begin() + 1);
		racuni.erase(racuni.end() - 5);
		racun.setImeKupca(racuni[0]);
		for (int i = 1; i < racuni.size() - 4; i += 5)
		{
			provjera = racun.setProizvod(racuni[i], racuni[i + 1], stod(racuni[i + 2]), stod(racuni[i + 3]), stod(racuni[i + 4]));
			if (provjera == false)
				return;
		}
		provjera = racun.setUkupno(stod(racuni[racuni.size() - 4]), stod(racuni[racuni.size() - 3]), stod(racuni[racuni.size() - 2]));
		if (provjera == false)
			return;
		provjera = racun.setDatumKupovine(racuni[racuni.size() - 1]);
		if (provjera == false)
			return;
		if (validniRacuni.size() > 0)
		{
			int brojac = 0;
			for (auto& x : validniRacuni)
				if (provjeraJedinstvenosti(x, racun))
					brojac++;
			if (brojac == 0)
				validniRacuni.push_back(racun);
		}
		else
			validniRacuni.push_back(racun);
	}
	else
		std::cout << "Racun nije otvoren";
}

int UcitajRacun::provjeraFormata(std::string rijec)
{
	if (rijec == "Kupac:")
		return 1;
	else if (rijec == "OSI")
		return 2;
	else
		return 0;
}

bool UcitajRacun::provjeraJedinstvenosti(Racun& racun1, Racun& racun2)
{
	return (racun1 == racun2) ? true : false;
}


void UcitajRacun::ucitajIzFajla()
{
	std::ifstream file("ValidniRacuni.txt");
	if (file.is_open())
	{
		std::vector<std::string> racuni;
		while (!file.eof())
		{
			std::string pomocna;
			file >> pomocna;
			if (pomocna != "Ukupan" && pomocna != "broj" && pomocna != "validnih" && pomocna != "racuna:" && pomocna != "Datum" && pomocna != "kupovine:" && pomocna != "Naziv" && pomocna != "Sifra" && pomocna != "Kolicina" && pomocna != "Cijena" && pomocna != "Ukupna" && pomocna != "cijena" && pomocna != "proizvoda:" && pomocna != "Ukupno:" && pomocna != "Ukupno" && pomocna != "za" && pomocna != "placanje:" && pomocna != "PDV:")
				racuni.push_back(pomocna);
		}
		file.close();
		int j = 2;
		for (int i = 0; i < stoi(racuni[0]); i++)
		{
			Racun racun(currency);
			std::vector<std::string> pomocna;
			while (racuni[j] != "Kupac:" && j < racuni.size() - 1)
			{
				pomocna.push_back(racuni[j]);
				j++;
			}
			racun.setImeKupca(pomocna[0]);
			racun.setDatumKupovine(pomocna[1]);
			for (int k = 2; k < pomocna.size() - 3; k += 5)
				racun.setProizvod(pomocna[k], pomocna[k + 1], stod(pomocna[k + 2]), stod(pomocna[k + 3]), stod(pomocna[k + 4]));
			racun.setUkupno(stod(pomocna[pomocna.size() - 3]), stod(pomocna[pomocna.size() - 2]), stod(pomocna[pomocna.size() - 1]));
			j++;
			validniRacuni.push_back(racun);
		}
	}
	else
		std::cout << "Fajl nije otvoren!" << std::endl;
}


void UcitajRacun::pregledPodatakaZaProizvod(std::string nazivProizvoda)
{
	int brojac = 0; // sluzi za provjeru da li je pronadjen zeljeni proizvod
	std::cout << std::endl << "Naziv proizvoda: " << nazivProizvoda << std::endl;
	for (auto& racun : validniRacuni) // prolazimo kroz niz racuna
		if (ispisiPodatkeZaProizvod(racun, nazivProizvoda))
			brojac++;
	if (brojac == 0)
		std::cout << std::endl << "Nema podataka za ovaj proizvod" << std::endl << std::endl;
}


UcitajRacun::~UcitajRacun()
{}
