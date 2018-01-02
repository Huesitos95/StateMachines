#pragma once
#include "../src/utils.h"

class State
{
private:
	
public:
	State();
	~State();
	void Enter();
	void Update();
	void Exit();
	PlayerState getStatus();
	PlayerState setStatus(PlayerState state);
};

