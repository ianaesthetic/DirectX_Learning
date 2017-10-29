#include "Geometry.h"

Geometry::Geometry() :
	mVCount(0), 
	mICount(0), 
	mVSize(0), 
	mISize(0){
	mVData.clear();
	mIData.clear();
}

Vertex* Geometry::GetVertexData() {
	return &mVData[0]; 
}

UINT* Geometry::GetIndexData() {
	return &mIData[0];
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
