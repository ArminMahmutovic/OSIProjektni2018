#include "UcitajRacun.h"



UcitajRacun::UcitajRacun(std::ifstream& currency) : Racun(currency), currency(currency)
{}


UcitajRacun::~UcitajRacun()
{
}
