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
	a->Behavior()->Seek(a, { 3,5 }, 0);
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

	std::cout << "stamina: " << a->stamina << std::endl;
	std::cout << "Water: " << a->water << std::endl;
	a->changeTo(s);
}
