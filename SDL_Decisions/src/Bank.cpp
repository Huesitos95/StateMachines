#include "Bank.h"



Bank::Bank()
{
	
}


Bank::~Bank()
{
}

void Bank::Enter(Agent * a)
{
	std::cout << "Enter Bank" << std::endl;
	a->full = false;
	/*a->Behavior()->Seek(a, { 3,5 }, 0);*/
}

void Bank::Update(Agent * a)
{
	state = STATE::BANK;


	a->Behavior()->Seek(a, { 3,5 }, 0);
	a->wealth += a->money;
	a->money = 0;
	

	if (a->wealth >= a->maxWealth)
	{
		a->wealth = a->maxWealth;
		//Ja te prou diner al banc
		a->wealthy = true;
		// Anar a casa
		Exit(a, a->home);
	}
	
	if (a->wealth < a->maxWealth)
	{
		// No te prou diner
		a->wealthy = false;
		
		// Anar a la mina
		Exit(a, a->mine);
	}

	if (a->stamina <= 0)
	{
		// Si esta cansat, va a casa.
		a->rested = false;
		Exit(a, a->home);
	}
}

void Bank::Exit(Agent * a, State * s)
{
	std::cout << "stamina: " << a->stamina << std::endl;
	std::cout << "water: " << a->water << std::endl;
	std::cout << "money: " << a->money << std::endl;
	std::cout << "wealth: " << a->wealth << std::endl;

	a->changeTo(s);
}
