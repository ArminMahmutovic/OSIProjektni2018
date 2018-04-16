#include "Login.h"



int login::menu(int option)
{
	std::ifstream currency("currency.txt");
	if (option == 0)
		return 0;
	if (option == 2)
	{
		char i;
		std::cout << "Unesite broj sa opcijom koju zelite koristiti:" << endl;
		std::cout << "1.Napravi novi racun" << endl << "2.Obrisi postojeci racun" << endl << "3.Promjeni valutu sistema" << endl << "4.Izloguj se" << endl;
		do {
			std::cin >> i;
			if (i != '1' && i != '2' && i != '3' && i != '4')
				std::cout << "Nevazeca opcija, molimo vas da unesete 1, 2, 3 or 4:" << endl;
		} while (i != '1' && i != '2' && i != '3' && i != '4');
		if (i == '1')
		{
			this->addUser();
			menu(option);
			return 0;
		}

		if (i == '2')
		{
			this->deleteUser();
			menu(option);
			return 0;
		}
		if (i == '3')
		{
			this->changeCurr();
			menu(option);
			return 0;
		}
		if (i == '4') {
			std::cout << "Uspjesno ste se izlogovali!" << endl;
			menu(this->welcome());
			return 0;
		}
	}
	if (option == 1)
	{
		
		UcitajRacun racun(currency);
		path p = "C:\\Users\\armin\\Desktop\\Racuni";
		for (auto& f : directory_iterator(p))
		{
			std::ifstream file(f);
			std::string nazivRacuna = f.path().filename().string();
			racun.ucitajRacun(file, nazivRacuna);
		}
		racun.upisiValidne();
		char i;
		std::cout << "Unesite broj sa opcijom koju zelite koristiti:" << endl;
		std::cout << "1.Pregled podataka za odredjenog kupca" << endl << "2.Pregled podataka za odredjeni mjesec" << endl << "3.Pregled podataka za odredjeni proizvod" << endl << "4.Izloguj se" << endl;
		do {
		std::cin >> i;
		if (i != '1' && i != '2' && i != '3' && i != '4')
		std::cout << "Nevazeca opcija, molimo vas da unesete 1, 2, 3 or 4:" << endl;
		} while (i != '1' && i != '2' && i != '3' && i != '4');
		if (i == '1')
		{
		std::string imeKupca;
		std::cout << "Unesite ime kupca: ";
		std::cin >> imeKupca;
		std::cout << endl;
		racun.pregledPodatakaZaKupca(imeKupca);
		menu(option);
		return 0;
		}

		if (i == '2')
		{
		int mjesec, godina;
		std::cout << "Unesite mjesec i godinu" << std::endl;
		std::cout << "Mjesec: ";
		std::cin >> mjesec;
		std::cout << "Godina: ";
		std::cin >> godina;
		std::cout << endl;
		racun.pregledUkupneProdajeZaOdredjeniMjesec(mjesec, godina);
		menu(option);
		return 0;
		}
		if (i == '3')
		{
		std::string nazivProizvoda;
		std::cout << "Unesite naziv proizvoda: ";
		std::cin >> nazivProizvoda;
		std::cout << endl;
		racun.pregledPodatakaZaProizvod(nazivProizvoda);
		return 0;
		}
		if (i == '4') {
		std::cout << "Uspjesno ste se izlogovali!" << endl;
		menu(this->welcome());
		return 0;
		}
		
	}
	return 0;
}

int login::welcome() {
	std::string acc;
	readUsers();
	do {
		std::cout << "Dobrodosli, imate li racun?" << endl;
		std::cin >> acc;
		if (acc != "da" && acc != "ne")
			std::cout << "Nevazeci odgovor, molimo unesite da ili ne!" << endl;
	} while (acc != "da" && acc != "ne");
	if (acc == "ne")
	{

		std::cout << "Pitajte administratora da Vam napravi racun" << endl;
		return 0;

	}

	return signIn();
}

void login::addUser()
{
	user temp;
	user* ptr;
	std::string check;
	do
	{
		std::cout << "Kako se zove novi korisnik?" << endl;
		std::cin >> temp.name;
		std::cout << "Kako se preziva novi korisnik?" << endl;
		std::cin >> temp.lastname;
		if (ptr = searchByName(temp.name, temp.lastname))
			std::cout << "Ovaj korisnik vec ima racun" << endl;
	} while (ptr);
	do {
		std::cout << "Molimo unesite korisnikov pin:" << endl;
		std::cin >> temp.pin;
		if (!isPin(temp.pin))
			std::cout << "Nevazeci pin, molimo koristite cetiri cifre:" << endl;
	} while (!isPin(temp.pin));
	do {
		std::cout << "Da li je novi korisnik admin?" << endl;
		std::cin >> check;
		if (check != "da" && check != "ne")
			std::cout << "Nevazeci odgovor, molimo unesite da ili ne!" << endl;
	} while (check != "da" && check != "ne");

	if (check == "da")
		temp.admin = true;
	else
		temp.admin = false;
	users.push_back(temp);
	std::ofstream file("users.txt", std::ios::app);
	if (file.is_open())
	{
		file << temp.name << endl;
		file << temp.lastname << endl;
		file << temp.pin << endl;
		if (temp.admin == true)
			file << "admin" << endl;
		else file << "analyst" << endl;
		file.close();
		std::cout << "Nalog uspjesno kreiran!" << endl;
		return;
	}
	else std::cout << "Datoteka nije otvorena" << endl;
	return;
}

int login::signIn()
{
	std::string name;
	std::string lastname;
	user* temp;
	do {
		std::cout << "Unesite svoje ime:" << endl;
		std::cin >> name;
		std::cout << "Unesite svoje prezime:" << endl;
		std::cin >> lastname;
		temp = searchByName(name, lastname);
		if (temp == nullptr)
			std::cout << "Nepostojeci nalog!" << endl;
	} while (temp == nullptr);
	std::string pin;
	do {
		std::cout << "Unesite pin:" << endl;
		std::cin >> pin;
		if (pin != temp->pin)
			std::cout << "Nevazeci pin!" << endl;
	} while (pin != temp->pin);
	std::cout << "Uspjesno ste se ulogovali!" << endl;
	if (temp->admin == true) return 2;
	else return 1;
}

user * login::searchByName(std::string name, std::string lastname)
{
	user temp;
	temp.name = name;
	temp.lastname = lastname;
	temp.pin = "0000";
	temp.admin = false;
	std::list<user>::iterator findIter;
	for (findIter = users.begin(); !(findIter == users.end()); ++findIter) {
		if (temp == *findIter)
			return &*findIter;
	}
	return nullptr;
}

void login::readUsers()
{
	std::ifstream file;
	file.open("users.txt");
	std::string temp;
	user temp1;
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, temp);
			temp1.name = temp;
			getline(file, temp);
			temp1.lastname = temp;
			getline(file, temp);
			temp1.pin = temp;
			getline(file, temp);
			if (temp == "admin")
				temp1.admin = true;
			else temp1.admin = false;
			if(temp1.name != "")
				users.push_back(temp1);
		};
		file.close();
	}
	else std::cout << "Datoteka nije otvorena!" << endl;
}

int login::deleteUser()
{
	user temp;
	std::cout << "Unesite ime korisnika ciji nalog zelite obrisati:" << endl;
	std::cin >> temp.name;
	std::cout << "Unesite prezime korisnika ciji nalog zelite obrisati:" << endl;
	std::cin >> temp.lastname;
	temp.pin = "0000";
	temp.admin = false;
	int i = 0;
	std::list<user> temp1;
	for (auto& x : users)
	{
		if (x != temp)
			temp1.push_back(x);
	}
	if (users.size() != temp1.size())
		i = 1;
	if (i == 0) {
		std::cout << "Nalog koji pokusavate obrisati ne postoji!" << endl;
		return i;
	}
	else 
	{
		users = temp1;
		remove("users.txt");
		std::ofstream file1;
		file1.open("users.txt");
		if (file1.is_open())
		{
			for (auto& x : users)
			{
				file1 << x.name << endl << x.lastname << endl << x.pin << endl;
				if (x.admin == true)
					file1 << "admin" << endl;
				else
					file1 << "analyst" << endl;
			}
			file1.close();
			std::cout << "nalog uspjesno uklonjen!" << endl;
			return i;
		}
		else std::cout << "Datoteka nije otvorena!" << endl;
		return 0;
	}
}

void login::changeCurr()
{
	std::string temp;
	do {
		std::cout << "Unesite novu skracenicu valute:" << endl;
		std::cin >> temp;
		if (!isCurr(temp))
			std::cout << "String je predug ili nije napisan velikim slovima!" << endl;
	} while (!isCurr(temp));

	remove("currency.txt");
	std::ofstream file;
	file.open("currency.txt");
	if (file.is_open())
	{
		file << temp;
		file.close();
		std::cout << "Valuta uspjesno promijenjena!" << endl;
	}
	else std::cout << "Datoteka nije otvorena!" << endl;
	return;
}


bool user::operator==(const user & b) const
{
	if (b.name == this->name && b.lastname == this->lastname)
		return true;
	return false;
}

bool user::operator==(user & b)
{
	if (b.name == this->name && b.lastname == this->lastname)
		return true;
	return false;
}

user & user::operator=(user &ref)
{
	admin = ref.admin;
	name = ref.name;
	lastname = ref.lastname;
	pin = ref.pin;
	return *this;
}

bool user::operator!=(const user & b) const
{
	if (b.name != this->name && b.lastname != this->lastname)
		return true;
	return false;
}

bool user::operator!=(user & b)
{
	if (b.name != this->name && b.lastname != this->lastname)
		return true;
	return false;
}

int isPin(std::string pin) {
	if (pin.length() != 4)
		return 0;
	for (int i = 0; i < 4; ++i)
	{
		if (pin[i] > '9' || pin[i] < '0')return 0;
	}
	return 1;
}



int isCurr(std::string temp) {
	if (temp.length() > 3)
		return 0;
	for (int i = 0; i < 3; ++i) {
		if (temp[i]<'A' || temp[i]>'Z')
			return 0;
	}
	return 1;
}