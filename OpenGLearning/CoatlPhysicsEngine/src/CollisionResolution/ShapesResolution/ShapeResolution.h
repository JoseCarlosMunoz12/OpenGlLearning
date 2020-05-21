#pragma once
#include "../Manifoldsh.h"
namespace CoatlPhysicsEngine
{
	class ShapeResolution
	{
	public:
		ShapeResolution();
		~ShapeResolution();
		template<typename _T, typename _N>
		std::vector<std::shared_ptr<Contact>> GetContacts(_T T,_N N);
	};


}