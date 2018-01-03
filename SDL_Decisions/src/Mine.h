#pragma once
#include "State.h"
#include <iostream>

class Mine :
	public State
{
public:
	Mine();
	~Mine();
	void Enter();
	void Update();
	void Exit();
};

