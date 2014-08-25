#include "config.h"
#include <vector>

IPOGLVertexBuffer* POGLXCreateCone(IPOGLRenderContext* context, const POGL_VECTOR3& A, const POGL_VECTOR3& D, POGL_FLOAT H, POGL_FLOAT R, POGL_UINT32 resolution, POGLBufferUsage::Enum usage)
{
	const POGL_VECTOR3X c = POGL_VECTOR3X(A) + (POGL_VECTOR3X(-D) * H);
	const POGL_VECTOR3X e0 = POGLVec3Perp(D);
	const POGL_VECTOR3X e1 = POGLVec3Cross(e0, D);
	const POGL_DOUBLE angleIncr = 360.0 / resolution * POGL_M_PI_DIV180;

	std::vector<POGL_VECTOR3X> circlePoints;
	for (POGL_UINT32 i = 0; i < resolution; ++i) {
		const POGL_DOUBLE rad = angleIncr * i;
		POGL_VECTOR3X p = c + (((e0 * (POGL_FLOAT)cos(rad)) + (e1 * (POGL_FLOAT)sin(rad))) * R);
		circlePoints.push_back(p);
	}

	//
	// Bottom Circle
	//

	std::vector<POGL_POSITION_VERTEX> vertices;

	for (POGL_UINT32 i = 1; i < resolution; ++i) {
		vertices.push_back(c);
		vertices.push_back({ circlePoints[i - 1] });
		vertices.push_back({ circlePoints[i] });
	}

	vertices.push_back(c);
	vertices.push_back({ circlePoints[resolution - 1] });
	vertices.push_back({ circlePoints[0] });

	//
	// Top cylinder
	//

	for (POGL_UINT32 i = 1; i < resolution; ++i) {
		vertices.push_back(A);
		vertices.push_back({ circlePoints[i - 1] });
		vertices.push_back({ circlePoints[i] });
	}

	vertices.push_back(A);
	vertices.push_back({ circlePoints[resolution - 1] });
	vertices.push_back({ circlePoints[0] });

	return context->CreateVertexBuffer(&vertices[0], vertices.size(), POGLPrimitiveType::TRIANGLE, usage);
}

