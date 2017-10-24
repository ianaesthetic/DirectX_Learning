#pragma once

#include "d3Util.h"

class Grid {
public:
	Grid();
	Grid(float left, float right, float up, float down, int w, int h);
	int SizeofGrid();
	int AmountOfVertex();
	int IndexQuery(const int &x, const int &y);
	void PosQuery(float &fx, float &fy, float &fz, const int &w, const int &h);

private:
	float mLeft; 
	float mRight;
	float mUp; 
	float mDown;

	// W: x-axis, H:z-axis

	int mW; 
	int mH;
	float mdw; 
	float mdh; 

};
