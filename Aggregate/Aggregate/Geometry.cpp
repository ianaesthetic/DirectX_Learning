#include "Geometry.h"

Geometry::Geometry() :
	mVCount(0), 
	mICount(0), 
	mVSize(0), 
	mISize(0){
}

int Geometry::IndexCount() {
	return mICount;
}

int Geometry::VertexCount() {
	return mVCount;
}

int Geometry::SizeofIndex() {
	return mISize;
}

int Geometry::SizeofVertex() {
	return mVSize;
}
