#include "Saloon.h"



Saloon::Saloon()
{
}


Saloon::~Saloon()
{
}

void Saloon::Enter(Agent * a)
{
	std::cout << "Enter Saloon" << std::endl;
}

void Saloon::Update(Agent * a)
{
	a->water++;

	if (a->water >= a->maxWater)
	{
		// Ja te aigua, torna a la mina
		a->thirsty = false;
		Exit(a, a->mine);
	}
}

void Saloon::Exit(Agent * a, State * s)
{
	std::cout << "money: " << a->money << std::endl;
	std::cout << "water: " << a->water << std::endl;
	std::cout << "stamina: " << a->stamina << std::endl;
	std::cout << "wealth: " << a->wealth << std::endl;
	a->changeTo(s);
}
