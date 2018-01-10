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
	// MINA 3-5
	/*a->Behavior()->Seek(a, { 3,5 }, 0);*/
}

void Mine::Update(Agent * a)
{
	a->Behavior()->Seek(a, { 3,5 }, 0);
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
	std::cout << "stamina: " << a->stamina << std::endl;

	a->changeTo(s);
}

