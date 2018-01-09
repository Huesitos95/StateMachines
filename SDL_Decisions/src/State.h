#pragma once
#include "../src/utils.h"
#include <iostream>
#include "Agent.h"

class State
{
private:
	
public:
	State();
	~State();
	virtual void Enter(Agent*);
	virtual void Update(Agent*);
	virtual void Exit(Agent*,State*);
};

