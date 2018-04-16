#pragma once
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "UcitajRacun.h"
#include "Racun.h"
#include <filesystem>
using namespace std::experimental::filesystem;
using std::endl;

struct user
{
	bool admin;
	std::string name;
	std::string lastname;
	std::string pin;
	bool operator==(const user& b) const;
	bool operator==(user& b);
	user& operator=(user&);
	bool operator!=(const user& b) const;
	bool operator!=(user& b);
};
struct login
{
	std::list<user> users;
	int menu(int);
	int welcome();
	void addUser();
	int signIn();
	user* searchByName(std::string, std::string);
	void readUsers();
	int deleteUser();
	void changeCurr();
};

int isPin(std::string);
int isCurr(std::string);