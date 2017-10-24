#include "Grid.h"

Grid::Grid() :
	mLeft(0),
	mRight(0), 
	mUp(0), 
	mDown(0),
	mW(0), 
	mH(0),
	mdw(0), 
	mdh(0){
}


Grid::Grid(float left, float right, float up, 
	float down, int w, int h) :
	mLeft(left), 
	mRight(right),
	mUp(up), 
	mDown(down), 
	mW(w),
	mH(h){
	
	if (mLeft > mRight) swap(mLeft, mRight);
	if (mDown > mUp) swap(mUp, mDown);

	mdw = (mRight - mLeft) / static_cast<float> (mW);
	mdh = (mUp - mDown) / static_cast<float> (mH);
}

int Grid::SizeofGrid() {
	return mW * mH;
}

int Grid::AmountOfVertex() {
	return (mW + 1) * (mH + 1);
}

int Grid::IndexQuery(const int &w, const int &h) {
	return h * (mW + 1) + w;
}

void Grid::PosQuery(float &fx, float &fy, float &fz, const int &w, const int &h) {
	fx = mLeft + w * mdw;
	fz = mUp - h * mdh;
	fy = 0.3f * (fz * sinf(0.1f * fx) + fx * cosf(0.1f * fz));
}
