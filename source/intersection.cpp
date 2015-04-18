#include "diy/intersection.hpp"

#include <cmath>

#define EPSILON 1.0e-6f

//#define NaiveTriangleIntersection TriangleIntersection
//#define MollerTrumboreTriangleIntersection TriangleIntersection
#define WatertightTriangleIntersection TriangleIntersection

namespace diy
{

	bool PlaneIntersection
		(
		glm::vec3 X0,
		glm::vec3 N,
		glm::vec3 O,
		glm::vec3 D,
		float* out
		)
	{
		float d = glm::dot(D, N);

		if (d == 0.0f)
		{
			return false;
		}

		if (out)
		{
			*out = glm::dot(X0 - O, N) / d;
		}

		return true;
	}

	bool NaiveTriangleIntersection
		(
		glm::vec3 V0,
		glm::vec3 V1,
		glm::vec3 V2,
		glm::vec3 O,
		glm::vec3 D,
		glm::vec4* out
		)
	{
		glm::vec3 E0 = V2 - V1;
		glm::vec3 E1 = V0 - V2;

		glm::vec3 N = glm::cross(E0, E1);
		float l = glm::length(N);
		if (l == 0.0f)
		{
			return false;
		}
		float inv_l = 1.0f / l;
		N *= inv_l;

		float t;
		if (!PlaneIntersection(V2, N, O, D, &t))
		{
			return false;
		}
		if (t < 0.0f)
		{
			return false;
		}

		glm::vec3 P = O + t * D;

		float u = glm::dot(N, glm::cross(E0, (P - V2))) * inv_l;
		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		float v = glm::dot(N, glm::cross(E1, (P - V0))) * inv_l;
		if (v < 0.0f || v > 1.0f)
		{
			return false;
		}

		float w = 1.0f - u - v;
		if (w < 0.0f || w > 1.0f)
		{
			return false;
		}

		if (out)
		{
			*out = glm::vec4(u, v, w, t);
		}

		return true;
	}

	bool MollerTrumboreTriangleIntersection
		(
		glm::vec3 V0,
		glm::vec3 V1,
		glm::vec3 V2,
		glm::vec3 O,
		glm::vec3 D,
		glm::vec4* out
		)
	{
		glm::vec3 E1 = V1 - V0;
		glm::vec3 E2 = V2 - V0;

		glm::vec3 P = glm::cross(D, E2);
		float det = glm::dot(E1, P);
		if (det > -EPSILON && det < EPSILON)
		{
			return false;
		}

		float inv_det = 1.0f / det;

		glm::vec3 T = O - V0;

		float u = glm::dot(T, P) * inv_det;
		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		glm::vec3 Q = glm::cross(T, E1);

		float v = glm::dot(D, Q) * inv_det;

		if (v < 0.0f || u + v  > 1.0f)
		{
			return false;
		}

		float t = glm::dot(E2, Q) * inv_det;

		if (t > EPSILON)
		{
			if (out)
			{
				*out = glm::vec4(1.0f - u - v, u, v, t);
			}

			return true;
		}

		return false;
	}

	bool WatertightTriangleIntersection
		(
		glm::vec3 V0,
		glm::vec3 V1,
		glm::vec3 V2,
		glm::vec3 O,
		glm::vec3 D,
		glm::vec4* out
		)
	{
		int kz = 0;
		if (fabs(D[1]) > fabs(D[kz]))
		{
			kz = 1;
		}
		if (fabs(D[2]) > fabs(D[kz]))
		{
			kz = 2;
		}

		int kx = (kz + 1) % 3;
		int ky = (kx + 1) % 3;

		if (D[kz] < 0.0f)
		{
			int aux = kx;
			kx = ky;
			ky = aux;
		}

		float Sx = D[kx] / D[kz];
		float Sy = D[ky] / D[kz];
		float Sz = 1.0f / D[kz];

		glm::vec3 A = V0 - O;
		glm::vec3 B = V1 - O;
		glm::vec3 C = V2 - O;

		float Ax = A[kx] - Sx * A[kz];
		float Ay = A[ky] - Sy * A[kz];
		float Bx = B[kx] - Sx * B[kz];
		float By = B[ky] - Sy * B[kz];
		float Cx = C[kx] - Sx * C[kz];
		float Cy = C[ky] - Sy * C[kz];

		float U = Cx * By - Cy * Bx;
		float V = Ax * Cy - Ay * Cx;
		float W = Bx * Ay - By * Ax;

		if (U == 0.0f || V == 0.0f || W == 0.0f)
		{
			double CxBy = (double)Cx * (double)By;
			double CyBx = (double)Cy * (double)Bx;
			U = (float)(CxBy - CyBx);
			double AxCy = (double)Ax * (double)Cy;
			double AyCx = (double)Ay * (double)Cx;
			V = (float)(AxCy - AyCx);
			double BxAy = (double)Bx * (double)Ay;
			double ByAx = (double)By * (double)Ax;
			W = (float)(BxAy - ByAx);
		}

#ifdef BACKFACE_CULLING
		if (U < 0.0f || V < 0.0f || W < 0.0f)
		{
			return false;
		}
#else
		if
			(
			(U < 0.0f || V < 0.0f || W < 0.0f) &&
			(U > 0.0f || V > 0.0f || W > 0.0f)
			)
		{
			return false;
		}
#endif

		float det = U + V + W;

		if (det == 0.0f)
		{
			return false;
		}

		float Az = Sz * A[kz];
		float Bz = Sz * B[kz];
		float Cz = Sz * C[kz];
		float T = U * Az + V * Bz + W * Cz;

#ifdef BACKFACE_CULLING
		if (T < 0.0f)
		{
			return false;
		}
#else
		bool det_sign = det > 0.0f;
		if ((det_sign ? T : -T) < 0.0f)
		{
			return false;
		}
#endif

		if (out)
		{
			float rcpDet = 1.0f / det;
			*out = glm::vec4(U * rcpDet, V * rcpDet, W * rcpDet, T * rcpDet);
		}

		return true;
	}

	bool SphereIntersection
		(
		glm::vec3 center,
		float radius,
		glm::vec3 rayOrigin,
		glm::vec3 rayDirection,
		float* out
		)
	{
		glm::vec3 D = center - rayOrigin;
		float d = glm::dot(rayDirection, D);
		float l = glm::length(D);

		float discriminating = d * d - l * l + radius * radius;

		if (discriminating < 0.0f)
		{
			return false;
		}

		if (out)
		{
			discriminating = glm::sqrt(discriminating);
			float d1 = d + discriminating;
			float d2 = d - discriminating;
			*out = d1 < d2 ? d1 : d2;
		}

		return true;
	}

}
