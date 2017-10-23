#pragma once

#include "d3Util.h"

class GameTimer {
public:
	GameTimer(); //Constructor 
	
	float TotalTime() const; 
	float DeltaTime() const; 

	void Tick(); 
	void Start(); 
	void Pause();
	void Reset(); 
	
private:
	double _SecondsPerCount; 
	double _DeltaTime; 

	__int64 _CurrentTime; 
	__int64 _BaseTime; 
	__int64 _PauseTime; 
	__int64 _StopTime; 
	__int64 _PrevTime; 

	bool _Stop; 
};