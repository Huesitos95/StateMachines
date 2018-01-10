#pragma once
#include "../src/utils.h"
#include <iostream>
#include "Agent.h"

class State
{

public:
	State();
	~State();
	virtual void Enter(Agent* a) = 0;
	virtual void Update(Agent* a) = 0;
	virtual void Exit(Agent* a,State* s) = 0;
};

