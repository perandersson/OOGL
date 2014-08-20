#include "config.h"
#include <vector>

IPOGLVertexBuffer* POGLXCreateSphere(IPOGLRenderContext* context, POGL_FLOAT R, POGL_UINT32 Q, POGL_UINT32 H, POGL_FLOAT K, POGL_UINT32 Z, POGLBufferUsage::Enum usage)
{
	const POGL_UINT32 SPACE = Q;
	const POGL_UINT32 VERTEX_COUNT = (180 / SPACE) * (360 / SPACE) * 4;

	std::vector<POGL_POSITION_TEXCOORD_VERTEX> vertices(VERTEX_COUNT);
	POGL_UINT32 n = 0;
	for (POGL_FLOAT b = 0; b <= 180 - SPACE; b += SPACE) {
		for (POGL_FLOAT a = 0; a <= 360 - SPACE; a += SPACE){
			vertices[n].position.x = R * sinf((a) / 180.0f * POGL_M_PI_F) * sinf((b) / 180.0f * POGL_M_PI_F) - H;
			vertices[n].position.y = R * cosf((a) / 180.0f * POGL_M_PI_F) * sinf((b) / 180.0f * POGL_M_PI_F) + K;
			vertices[n].position.z = R * cosf((b) / 180.0f * POGL_M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * b) / 360.0f;
			vertices[n].texCoord.s = (a) / 360.0f;
			n++;

			vertices[n].position.x = R * sinf((a) / 180.0f * POGL_M_PI_F) * sinf((b + SPACE) / 180.0f * POGL_M_PI_F) - H;
			vertices[n].position.y = R * cosf((a) / 180.0f * POGL_M_PI_F) * sinf((b + SPACE) / 180.0f * POGL_M_PI_F) + K;
			vertices[n].position.z = R * cosf((b + SPACE) / 180.0f * POGL_M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * (b + SPACE)) / 360.0f;
			vertices[n].texCoord.s = (a) / 360.0f;
			n++;

			vertices[n].position.x = R * sinf((a + SPACE) / 180.0f * POGL_M_PI_F) * sinf((b) / 180.0f * POGL_M_PI_F) - H;
			vertices[n].position.y = R * cosf((a + SPACE) / 180.0f * POGL_M_PI_F) * sinf((b) / 180.0f * POGL_M_PI_F) + K;
			vertices[n].position.z = R * cosf((b) / 180.0f * POGL_M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * b) / 360.0f;
			vertices[n].texCoord.s = (a + SPACE) / 360.0f;
			n++;

			vertices[n].position.x = R * sinf((a + SPACE) / 180.0f * POGL_M_PI_F) * sinf((b + SPACE) / 180.0f * POGL_M_PI_F) - H;
			vertices[n].position.y = R * cosf((a + SPACE) / 180.0f * POGL_M_PI_F) * sinf((b + SPACE) / 180.0f * POGL_M_PI_F) + K;
			vertices[n].position.z = R * cosf((b + SPACE) / 180.0f * POGL_M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * (b + SPACE)) / 360.0f;
			vertices[n].texCoord.s = (a + SPACE) / 360.0f;
			n++;
		}
	}

	return context->CreateVertexBuffer(&vertices[0], vertices.size(), POGLPrimitiveType::TRIANGLE_STRIP, usage);
}

