#include "DynColManager.h"
using namespace CoatlPhysicsEngine;

DynColManager::DynColManager()
{
}

DynColManager::~DynColManager()
{
}

template<typename _T, typename _N>
inline bool DynColManager::CheckCollide(_T R, _N N)
{
	return false;
}