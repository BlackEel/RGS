#pragma once
#include "RGS/Shaders/ShaderBase.h"

#include "RGS/Maths.h"

namespace RGS {
	struct BlinnVertex : public VertexBase
	{
		Vec3 ModelNormal;
		Vec2 TexCoord = { 0.0f, 0.0f };

		/*friend std::ostream& operator<<(std::ostream& os, const BlinnVertex& vertex)
		{
			os << "ModelPos: " << vertex.ModelPos << ", ModelNormal: " << vertex.ModelNormal << ", TexCoord: " << vertex.TexCoord;
			return os;
		}*/
	};

	struct BlinnVaryings : public VaryingsBase
	{
		Vec3 WorldPos;
		Vec3 WorldNormal;
		Vec2 TexCoord;
	};

	struct BlinnUniforms : public UniformsBase
	{
		Mat4 Model;
		Mat4 ModelNormalToWorld;
		Vec3 LightPos = { 0.0f, 1.0f, 2.0f };
		Vec3 LightAmbient = { 0.3f, 0.3f, 0.3f };
		Vec3 LightDiffuse = { 0.5f, 0.5f, 0.5f };
		Vec3 LightSpecular = { 1.0f, 1.0f, 1.0f };
		Vec3 objectColor = { 0.8f, 0.1f, 0.2f };
		Vec3 CameraPos;

		float Shininess = 64.0f;
	};

	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}