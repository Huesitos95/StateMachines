#pragma once
#include "State.h"
class Saloon :
	public State
{
public:
	Saloon();
	~Saloon();
	void Enter(Agent* a);
	void Update(Agent* a);
	void Exit(Agent* a, State* s);
};

