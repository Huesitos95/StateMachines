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
	a->wealth = 0;
	a->wealthy = false;
	
}

void Home::Update(Agent * a)
{

	state = STATE::HOME;

	a->Behavior()->Seek(a, { 3,5 }, 0);
	if (a->stamina <= a->maxStamina)
	{
		//No esta recuperat i NO torna a la mina
		a->stamina++;
	}

	if (a->stamina >= a->maxStamina)
	{
		a->stamina = a->maxStamina;
		// Esta recuperat i torna a la mina
		a->rested = true;
		Exit(a, a->mine);
	}
}

void Home::Exit(Agent * a, State * s)
{

	std::cout << "stamina: " << a->stamina << std::endl;

	a->changeTo(s);
}
