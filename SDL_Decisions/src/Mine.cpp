#include "Mine.h"

Mine::Mine()
{
	
}

Mine::~Mine()
{

}

void Mine::Enter(Agent * a)
{
	std::cout << "Enter Mine" << std::endl;
	
}

void Mine::Update(Agent * a)
{
	a->money++;
	a->stamina--;
	a->water--;

	if (a->money >= a->maxMoney)
	{
		// Depositar al Banc
		a->full = true;
		Exit(a, a->bank);
	}

	if (a->water <= 0)
	{
		// No te aigua, TE SED, va al SALOON
		a->thirsty = true;
		Exit(a, a->saloon);
	}
}

void Mine::Exit(Agent * a, State * s)
{
	std::cout << "money: " << a->money << std::endl;
	std::cout << "water: " << a->water << std::endl;
	std::cout << "stamina: " << a->stamina << std::endl;
	std::cout << "wealth: " << a->wealth << std::endl;
	a->changeTo(s);
}

