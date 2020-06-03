#include "GJK.h"
using namespace CoatlPhysicsEngine;

bool CoatlPhysicsEngine::GJK(std::shared_ptr<gjk_simplex> &S, std::shared_ptr<gjk_support> &Sup)
{
	if (!S || !Sup)
		return false;
	if (S->Max_iter > 0 && S->iter >= S->Max_iter)
		return false;
	//I. Initialize
	if (S->Cnt == 0)
	{
		S->D = 3.40282347E+38F;
		S->Max_iter = !S->Max_iter ? 3.40282347E+38F : S->Max_iter;
	}
	// II. Check for Duplicaiton;
	for (int ii = 0; ii < S->Cnt; ++ii)
	{
		if (Sup->AId != S->V[ii].AId)continue;
		if (Sup->BId != S->V[ii].BId)continue;
	}
	// III. add Vertex into simplex
	gjk_vertex vert = S->V[S->Cnt];
	vert.A = Sup->A;
	vert.B = Sup->B;
	vert.P = Sup->D;
	S->V[S->Cnt] = vert;
	S->BC[S->Cnt++] = 1.f;
	// IV. find closest simplex point
	switch (S->Cnt)
	{
	case 1:	break;
	case 2: {
		glm::vec3 A = S->V[0].P;
		glm::vec3 B = S->V[1].P;
		glm::vec3 AB = A - B;
		glm::vec3 BA = B - A;
		float u = glm::dot(B, BA);
		float v = glm::dot(A, AB);
		if (v <= 0.f)
		{
			S->BC[0] = 1.f;
			S->Cnt = 1;
			break;
		}
		if (u <= 0.f)
		{
			S->V[0] = S->V[1];
			S->BC[0] = 1.0f;
			S->Cnt = 1;
			break;
		}
		S->BC[0] = u;
		S->BC[1] = v;
		S->Cnt = 2;
	} break;
	}
	return false;
}

void CoatlPhysicsEngine::GJK_analyze(std::shared_ptr<gjk_result>& Res)
{
}

void CoatlPhysicsEngine::GJK_quat(std::shared_ptr<gjk_result>& Res, float A_radius, float B_Radius)
{
}
