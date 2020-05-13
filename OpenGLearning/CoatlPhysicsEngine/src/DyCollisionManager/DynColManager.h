#pragma once
namespace CoatlPhysicsEngine {
	class DynColManager
	{
	private:
	public:
		DynColManager();
		~DynColManager();
		template<typename _T,typename _N>
		bool CheckCollide(_T R, _N N);
	};
}