#pragma once
#include "RGS/Maths.h"
#include <string>

namespace RGS {
	struct VertexBase
	{
		Vec4 ModelPos = { 0, 0, 0, 1 };
		operator const std::string() const { return "ModelPos: " + (std::string)ModelPos; }
	};

	struct VaryingsBase
	{
		Vec4 ClipPos = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vec4 NdcPos = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vec4 FragPos = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

	struct UniformsBase
	{
		Mat4 MVP;
		operator const std::string() const { return (std::string)MVP; }
	};
}