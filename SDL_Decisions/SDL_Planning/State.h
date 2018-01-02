#pragma once
#include "../src/utils.h"

class State
{
private:
	PlayerState *Status;
public:
	State();
	~State();
	void Enter();
	void Update();
	void Exit();
};

