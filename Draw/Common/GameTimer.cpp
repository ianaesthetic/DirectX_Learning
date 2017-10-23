#include "GameTimer.h" 

GameTimer::GameTimer():_SecondsPerCount(0.0), _DeltaTime(0.0), _BaseTime(0), 
	_StopTime(0), _PauseTime(0), _PrevTime(0), _CurrentTime(0){
	__int64 countsPerSecond; 
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond); 
	_DeltaTime = 0.0; 
	_SecondsPerCount = 1.0 / countsPerSecond; 
}

float GameTimer::DeltaTime()const {
	return float(_DeltaTime); 
}

float GameTimer::TotalTime()const {
	if (_Stop) {
		return float((_StopTime - _PauseTime - _BaseTime) * _SecondsPerCount);
	}
	else return float((_CurrentTime - _PauseTime - _BaseTime) * _SecondsPerCount);
}

void GameTimer::Tick() {
	if (_Stop) {
		_DeltaTime = 0.0;
	}
	else {
		__int64 currentTime; 
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		_CurrentTime = currentTime; 
		_DeltaTime = (_CurrentTime - _PrevTime) * _SecondsPerCount; 
		_PrevTime = _CurrentTime;
		if (_DeltaTime < 0.0) _DeltaTime = 0.0;
	}
}

void GameTimer::Pause() {
	if (!_Stop) {
		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		_StopTime = currentTime; 
		_Stop = 1;
	}
}

void GameTimer::Start() {
	if (_Stop) {
		_int64 currentTime; 
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime); 
		_PauseTime += currentTime - _StopTime;
		_PrevTime = currentTime;
		_CurrentTime = 0;
		_StopTime = 0;
		_Stop = 1; 
	}
}

void GameTimer::Reset() {
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	_BaseTime = currentTime; 
	_PrevTime = currentTime; 
	_StopTime = 0; 
	_PauseTime = 0;
	_Stop = 0;

}