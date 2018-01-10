#include "Home.h"



Home::Home()
{
}


Home::~Home()
{
}

void Home::Enter(Agent * a)
{
	std::cout << "Enter Home" << std::endl;

	//Reset bank per poder tornar
	a->maxWealth = 0;
	a->wealthy = false;
	
}

void Home::Update(Agent * a)
{
	if (a->stamina <= a->maxStamina)
	{
		//No esta recuperat i NO torna a la mina
		a->stamina++;
	}

	if (a->stamina >= a->maxStamina)
	{
		// Esta recuperat i torna a la mina
		a->rested = true;
		Exit(a, a->mine);
	}
}

void Home::Exit(Agent * a, State * s)
{
	std::cout << "money: " << a->money << std::endl;
	std::cout << "water: " << a->water << std::endl;
	std::cout << "stamina: " << a->stamina << std::endl;
	std::cout << "wealth: " << a->wealth << std::endl;
	a->changeTo(s);
}
