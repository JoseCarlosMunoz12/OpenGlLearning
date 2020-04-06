#include "CapsuleColAABB.h"
using namespace CoatlPhysicsEngine;

bool CapsuleColAABB::CapColAABB(Capsule Cap, AABB_Obj Obj)
{
	std::vector<int> ID = Obj.GetSegmentsID();
	int Size = ID.size() / 2;
	std::vector<glm::vec3> Points = Obj.GetPoints();
	for (int ii = 0; ii < Size; ii++)
	{
		int JJ = ii * 2;
		int KK = JJ + 1;
		std::cout << JJ << "--" << KK <<"\n";
	}
	return false;
}

CapsuleColAABB::CapsuleColAABB()
{
}

CapsuleColAABB::~CapsuleColAABB()
{
}
