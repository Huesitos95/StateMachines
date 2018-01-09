#pragma once
#include "State.h"
#include <iostream>

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

