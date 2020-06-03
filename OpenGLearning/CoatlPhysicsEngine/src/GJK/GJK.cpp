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
		//LINE
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
	case 3: {
		//Triangle
		glm::vec3 A = S->V[0].P;
		glm::vec3 B = S->V[1].P;
		glm::vec3 C = S->V[2].P;

		glm::vec3 AB = A - B;
		glm::vec3 BA = B - A;
		glm::vec3 BC = B - C;
		glm::vec3 CB = C - B;
		glm::vec3 AC = A - C;
		glm::vec3 CA = C - A;

		float u_AB = glm::dot(B, BA);
		float v_AB = glm::dot(A, AB);

		float u_BC = glm::dot(C, CB);
		float v_BC = glm::dot(B, BC);

		float u_CA = glm::dot(A, AC);
		float v_CA = glm::dot(C, CA);

		if (v_AB <= 0.f && u_CA <= 0.f)
		{
			S->BC[0] = 1.f;
			S->Cnt = 1;
			break;
		}
		if (u_AB <= 0.f && v_BC <= 0.f)
		{
			S->V[0] = S->V[1];
			S->BC[0] = 1.f;
			S->Cnt = 1;
			break;
		}
		if (u_BC <= 0.f && v_CA <= 0.f)
		{
			S->V[0] = S->V[2];
			S->BC[0] = 1.f;
			S->Cnt = 1;
		}

		glm::vec3 n = glm::cross(BA, CA);
		glm::vec3 n1 = glm::cross(B, C);
		glm::vec3 n2 = glm::cross(C, A);
		glm::vec3 n3 = glm::cross(A, B);

		float u_ABC = glm::dot(n1, n);
		float v_ABC = glm::dot(n2, n);
		float w_ABC = glm::dot(n3, n);
		if (u_AB > 0.0f && v_AB > 0.0f && w_ABC <= 0.0f) {
			/* region B */
			S->BC[0] = u_AB;
			S->BC[1] = v_AB;
			S->Cnt = 2;
			break;
		}
		if (u_BC > 0.0f && v_BC > 0.0f && u_BC <= 0.0f) {
			/* region BC */
			S->V[0] = S->V[1];
			S->V[1] = S->V[2];
			S->BC[0] = u_BC;
			S->BC[1] = v_BC;
			S->Cnt = 2;
			break;
		}
		if (u_CA > 0.0f && v_CA > 0.0f && v_BC <= 0.0f) {
			/* region CA */
			S->V[1] = S->V[0];
			S->V[0] = S->V[2];
			S->BC[0] = u_CA;
			S->BC[1] = v_CA;
			S->Cnt = 2;
			break;
		}

		S->BC[0] = u_ABC;
		S->BC[1] = v_ABC;
		S->BC[2] = w_ABC;
		S->Cnt = 3;
	} break;
	case 4: {
		//Tetrahedron
		glm::vec3 A = S->V[0].P;
		glm::vec3 B = S->V[1].P;
		glm::vec3 C = S->V[2].P;
		glm::vec3 D = S->V[3].P;

		glm::vec3 AB = A - B;
		glm::vec3 BA = B - A;
		glm::vec3 BC = B - C;
		glm::vec3 CB = C - B;
		glm::vec3 CA = C - A;
		glm::vec3 AC = A - C;

		glm::vec3 DB = D - B;
		glm::vec3 BD = B - D;
		glm::vec3 DC = D - C;
		glm::vec3 CD = C - D;
		glm::vec3 DA = D - A;
		glm::vec3 AD = A - D;

		float u_AB = glm::dot(A, BA);
		float v_AB = glm::dot(A, AB);

	}break;}
	//V. Check if origin is enclosed by tetrahedron
	if (S->Cnt == 4)
	{
		S->Hit = true;
		return  false;
	}
	//VI. Ensure closing in on orgin to prevent multi-step cycling

	return false;
}

void CoatlPhysicsEngine::GJK_analyze(std::shared_ptr<gjk_result>& Res)
{
}

void CoatlPhysicsEngine::GJK_quat(std::shared_ptr<gjk_result>& Res, float A_radius, float B_Radius)
{
}
