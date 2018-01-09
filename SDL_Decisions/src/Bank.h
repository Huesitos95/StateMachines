#pragma once
#include "State.h"
class Bank :
	public State
{
public:
	Bank();
	~Bank();
	void Enter(Agent* a);
	void Update(Agent* a);
	void Exit(Agent* a, State* s);
};

