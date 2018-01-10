#pragma once

#include <iostream>
#include "Agent.h"

class Mine :
	public State
{
public:
	Mine();
	~Mine();
	void Enter(Agent* a);
	void Update(Agent* a);
	void Exit(Agent* a, State* s);
};

