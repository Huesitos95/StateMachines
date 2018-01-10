#pragma once
#include "Agent.h"


class Home :
	public State
{
public:
	Home();
	~Home();
	void Enter(Agent* a);
	void Update(Agent* a);
	void Exit(Agent* a, State* s);
};

