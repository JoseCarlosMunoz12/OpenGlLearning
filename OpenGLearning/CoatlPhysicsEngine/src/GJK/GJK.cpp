#include "GJK.h"
using namespace CoatlPhysicsEngine;

float CoatlPhysicsEngine::F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	glm::vec3 N = glm::cross(A, B);
	return glm::dot(N, C);
}

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

		float u_BC = glm::dot(C, CB);
		float v_BC = glm::dot(B, BC);

		float u_CA = glm::dot(A, AC);
		float v_CA = glm::dot(C, CA);

		float u_BD = glm::dot(D, DB);
		float v_BD = glm::dot(B, BD);

		float u_DC = glm::dot(C, CD);
		float v_DC = glm::dot(D, DC);

		float u_AD = glm::dot(D, DA);
		float v_AD = glm::dot(A, AD);
		if (v_AB <= 0.f && u_CA <= 0.f && v_AD <= 0.f)
		{
			S->BC[0] = 1.f;
			S->Cnt = 1;
			break;
		}
		if (u_AB <= 0.f && v_BC <= 0.f && v_BD <= 0.f)
		{
			S->V[0] = S->V[1];
			S->BC[0] = 1.0f;
			S->Cnt = 1;
			break;
		}
		if (u_BC <= 0.0f && v_CA <= 0.0f && u_DC <= 0.0f) {
			/* region C */
			S->V[0] = S->V[2];
			S->BC[0] = 1.0f;
			S->Cnt = 1;
			break;
		}
		if (u_BD <= 0.0f && v_DC <= 0.0f && u_AD <= 0.0f) {
			/* region D */
			S->V[0] = S->V[3];
			S->BC[0] = 1.0f;
			S->Cnt = 1;
			break;
		}

		glm::vec3 N = glm::cross(DA, BA);
		glm::vec3 N1 = glm::cross(D, B);
		glm::vec3 N2 = glm::cross(B, A);
		glm::vec3 N3 = glm::cross(A, D);

		float u_ADB = glm::dot(N1, N);
		float v_ADB = glm::dot(N2, N);
		float w_ADB = glm::dot(N3, N);

		N = glm::cross(CA, DA);
		N1 = glm::cross(C, D);
		N2 = glm::cross(D, A);
		N3 = glm::cross(A, C);

		float u_ACD = glm::dot(N1, N);
		float v_ACD = glm::dot(N2, N);
		float w_ACD = glm::dot(N3, N);

		N = glm::cross(BC, DC);
		N1 = glm::cross(B, D);
		N2 = glm::cross(C, A);
		N3 = glm::cross(A, B);

		float u_CBD = glm::dot(N1, N);
		float v_CBD = glm::dot(N2, N);
		float w_CBD = glm::dot(N3, N);

		N = glm::cross(BA, CA);
		N1 = glm::cross(B, C);
		N2 = glm::cross(C, A);
		N3 = glm::cross(A, B);

		float u_ABC = glm::dot(N1, N);
		float v_ABC = glm::dot(N2, N);
		float w_ABC = glm::dot(N3, N);

		if (w_ABC <= 0.0f && v_ADB <= 0.0f && u_AB > 0.0f && v_AB > 0.0f) {
			/* region AB */
			S->BC[0] = u_AB;
			S->BC[1] = v_AB;
			S->Cnt = 2;
			break;
		}
		if (u_ABC <= 0.0f && w_CBD <= 0.0f && u_BC > 0.0f && v_BC > 0.0f) {
			/* region BC */
			S->V[0] = S->V[1];
			S->V[1] = S->V[2];
			S->BC[0] = u_BC;
			S->BC[1] = v_BC;
			S->Cnt = 2;
			break;
		}
		if (v_ABC <= 0.0f && w_ACD <= 0.0f && u_CA > 0.0f && v_CA > 0.0f) {
			/* region CA */
			S->V[1] = S->V[0];
			S->V[0] = S->V[2];
			S->BC[0] = u_CA;
			S->BC[1] = v_CA;
			S->Cnt = 2;
			break;
		}
		if (v_CBD <= 0.0f && u_ACD <= 0.0f && u_DC > 0.0f && v_DC > 0.0f) {
			/* region DC */
			S->V[0] = S->V[3];
			S->V[1] = S->V[2];
			S->BC[0] = u_DC;
			S->BC[1] = v_DC;
			S->Cnt = 2;
			break;
		}
		if (v_ACD <= 0.0f && w_ADB <= 0.0f && u_AD > 0.0f && v_AD > 0.0f) {
			/* region AD */
			S->V[1] = S->V[3];
			S->BC[0] = u_AD;
			S->BC[1] = v_AD;
			S->Cnt = 2;
			break;
		}
		if (u_CBD <= 0.0f && u_ADB <= 0.0f && u_BD > 0.0f && v_BD > 0.0f) {
			/* region BD */
			S->V[0] = S->V[1];
			S->V[1] = S->V[3];
			S->BC[0] = u_BD;
			S->BC[1] = v_BD;
			S->Cnt = 2;
			break;
		}

		float denom = F3Box(CB, AB, DB);
		float Volume = (denom == 0) ? 1.0f : 1.f / denom;
		float u_abcd = F3Box(C, D, B) * Volume;
		float v_abcd = F3Box(C, A, D) * Volume;
		float w_abcd = F3Box(D, A, B) * Volume;
		float x_abcd = F3Box(B, A, C) * Volume;

		if (x_abcd < 0.0f && u_ABC > 0.0f && v_ABC > 0.0f && w_ABC > 0.0f) {
			/* region ABC */
			S->BC[0] = u_ABC;
			S->BC[1] = v_ABC;
			S->BC[2] = w_ABC;
			S->Cnt = 3;
			break;
		}
		if (u_abcd < 0.0f && u_CBD > 0.0f && v_CBD > 0.0f && w_CBD > 0.0f) {
			/* region CBD */
			S->V[0] = S->V[2];
			S->V[2] = S->V[3];
			S->BC[0] = u_CBD;
			S->BC[1] = v_CBD;
			S->BC[2] = w_CBD;
			S->Cnt = 3;
			break;
		}
		if (v_abcd < 0.0f && u_ACD> 0.0f && v_ACD > 0.0f && w_ACD > 0.0f) {
			/* region ACD */
			S->V[1] = S->V[2];
			S->V[2] = S->V[3];
			S->BC[0] = u_ACD;
			S->BC[1] = v_ACD;
			S->BC[2] = w_ACD;
			S->Cnt = 3;
			break;
		}
		if (w_abcd < 0.0f && u_ADB > 0.0f && v_ADB > 0.0f && w_ADB > 0.0f) {
			/* region ADB */
			S->V[2] = S->V[1];
			S->V[1] = S->V[3];
			S->BC[0] = u_ADB;
			S->BC[1] = v_ADB;
			S->BC[2] = w_ADB;
			S->Cnt = 3;
			break;
		}
		S->BC[0] = u_abcd;
		S->BC[1] = v_abcd;
		S->BC[2] = w_abcd;
		S->BC[3] = x_abcd;
		S->Cnt = 4;
	}break;}
	//V. Check if origin is enclosed by tetrahedron
	if (S->Cnt == 4)
	{
		S->Hit = true;
		return  false;
	}
	//VI. Ensure closing in on orgin to prevent multi-step cycling
	glm::vec3 Pnt; float Denom = 0.f;
	for (int ii = 0; ii < S->Cnt; ++ii)
		Denom += S->BC[ii];
	Denom = 1.f / Denom;

	switch (S->Cnt)
	{
	case 1: Pnt = S->V[0].P; break;
	case 2: {
		glm::vec3 A = S->V[0].P * Denom * S->BC[0];
		glm::vec3 B = S->V[1].P * Denom * S->BC[1];
		Pnt = A + B;
	}break;
	case 3: {
		glm::vec3 A = S->V[0].P * Denom * S->BC[0];
		glm::vec3 B = S->V[1].P * Denom * S->BC[1];
		glm::vec3 C = S->V[2].P * Denom * S->BC[2];
		Pnt = A + B + C;
	}break;}
	float D2 = glm::dot(Pnt, Pnt);
	if (D2 >= S->D)return false;
	S->D = D2;
	//VII. new Search Direction;
	glm::vec3 D;
	switch (S->Cnt)
	{
	default: break;
	case 1: {
		D = -S->V[0].P;
	}break;
	case 2: {
		glm::vec3 BA = S->V[1].P - S->V[0].P;
		glm::vec3 B0 = S->V[1].P;
		glm::vec3 T = glm::cross(BA, B0);
		D = glm::cross(T, BA);
	}break;
	case 3:{
		glm::vec3 AB = S->V[1].P - S->V[0].P;
		glm::vec3 AC = S->V[2].P - S->V[0].P;
		glm::vec3 N = glm::cross(AB, AC);
		if (glm::dot(N, S->V[0].P) <= 0.f)
			D = N;
		else
		{
			D = -N;
		}
	}}
	if (glm::dot(D, D) < 0)
		return false;
	Sup->DA = -D;
	Sup->DB = D;
	return true;
}

void CoatlPhysicsEngine::GJK_analyze(std::shared_ptr<gjk_result>& Res, std::shared_ptr<gjk_simplex>& s)
{
	Res->Iter = s->iter;
	Res->Hit = s->Hit;

	float Denom = 0.f;
	for (int ii = 0; ii < s->Cnt; ++ii)
		Denom += s->BC[ii];
	Denom = 1.0f / Denom;
}

void CoatlPhysicsEngine::GJK_quat(std::shared_ptr<gjk_result>& Res, float A_radius, float B_Radius)
{
}
