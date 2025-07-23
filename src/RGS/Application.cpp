#include "Application.h"

#include "RGS/Window.h"
#include "RGS/Framebuffer.h"
#include "RGS/Maths.h"
#include "RGS/Shaders/BlinnShader.h"
#include "RGS/Renderer.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

namespace RGS {
	Application::Application(std::string name, const int width, const int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
		Init();
	}

	Application::~Application()
	{
		Terminate();
	}

	void Application::Init()
	{
		Window::Init();
		m_Window = Window::Create(m_Name, m_Width, m_Height);
		m_LastFrameTime = std::chrono::high_resolution_clock::now();

		LoadMesh("C:\\Users\\Administrator\\Desktop\\heart1.obj");
	}

	void Application::Terminate()
	{
		delete m_Window;
		Window::Terminate();
	}

	void Application::Run()
	{
		while (!m_Window->Closed())
		{
			auto nowFrameTime = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(nowFrameTime - m_LastFrameTime);
			float deltaTime = duration.count() * 0.001f * 0.001f;
			m_LastFrameTime = nowFrameTime;

			OnUpdate(deltaTime);

			Window::PollInputEvents();
		}
	}

	void Application::OnCameraUpdate(float time)
	{
		constexpr float speed = 5.0f;
		if (m_Window->GetKey(RGS_KEY_SPACE) == RGS_PRESS)
		{
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Up;
		}
		if (m_Window->GetKey(RGS_KEY_LEFT_SHIFT) == RGS_PRESS)
		{
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Up;
		}
		if (m_Window->GetKey(RGS_KEY_D) == RGS_PRESS)
		{
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Right;
		}
		if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS)
		{
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Right;
		}
		if (m_Window->GetKey(RGS_KEY_W) == RGS_PRESS)
		{
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Dir;
		}
		if (m_Window->GetKey(RGS_KEY_S) == RGS_PRESS)
		{
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Dir;
		}

		constexpr float rotateSpeed = 1.0f;
		Mat4 rotation = Mat4Identity();
		if (m_Window->GetKey(RGS_KEY_Q) == RGS_PRESS)
		{
			rotation = Mat4RotateY(time * rotateSpeed);
		}
		if (m_Window->GetKey(RGS_KEY_E) == RGS_PRESS)
		{
			rotation = Mat4RotateY(-time * rotateSpeed);
		}
		m_Camera.Dir = rotation * m_Camera.Dir;
		m_Camera.Dir = { Normalize(m_Camera.Dir), 0.0f };
		m_Camera.Right = rotation * m_Camera.Right;
		m_Camera.Right = { Normalize(m_Camera.Right), 0.0f };
	}

	void Application::OnUpdate(float time)
	{
		OnCameraUpdate(time);

		Framebuffer framebuffer(m_Width, m_Height);
		Program program(BlinnVertexShader, BlinnFragmentShader);
		
		Mat4 view = Mat4LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, { 0.0f, 1.0f, 0.0f });
		Mat4 proj = Mat4Perspective(45.0f / 360.0f * 2.0f * PI, m_Camera.Aspect, 0.1f, 100.0f);

		Mat4 model = Mat4Identity();
		m_Uniforms.MVP = proj * view * model;
		m_Uniforms.Model = model;
		m_Uniforms.CameraPos = m_Camera.Pos;
		m_Uniforms.ModelNormalToWorld = Mat4Identity();

		m_Uniforms.Shininess *= std::pow(2, time * 2.0f);
		if(m_Uniforms.Shininess > 256.0f)
		{
			m_Uniforms.Shininess -= 256.0f;
		}

		for (auto tri : m_Mesh)
		{
			Renderer::Draw(framebuffer, program, tri, m_Uniforms);
		}

		m_Window->DrawFramebuffer(framebuffer);
	}

	void Application::LoadMesh(const char* fileName)
	{
		std::ifstream file(fileName);
		ASSERT(file);

		std::vector<Vec3> positions;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> normals;
		std::vector<int> posIndices;
		std::vector<int> texIndices;
		std::vector<int> normalIndices;

		std::string line;
		while (!file.eof())
		{
			std::getline(file, line);
			int items = -1;
			if (line.find("v ") == 0)
			{
				Vec3 position;
				items = sscanf(line.c_str(), "v %f %f %f", &position.X, &position.Y, &position.Z);
				ASSERT(items == 3);
				positions.push_back(position);
			}
			else if (line.find("vt ") == 0)
			{
				Vec2 texCoord;
				items = sscanf(line.c_str(), "vt %f %f", &texCoord.X, &texCoord.Y);
				ASSERT(items == 2);
				texCoords.push_back(texCoord);
			}
			else if (line.find("vn ") == 0)
			{
				Vec3 normal;
				items = sscanf(line.c_str(), "vn %f %f %f", &normal.X, &normal.Y, &normal.Z);
				ASSERT(items == 3);
				normals.push_back(normal);
			}
			else if (line.find("f ") == 0)
			{
				int posIndex[3], texIndex[3], normalIndex[3];
				items = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
					&posIndex[0], &texIndex[0], &normalIndex[0],
					&posIndex[1], &texIndex[1], &normalIndex[1],
					&posIndex[2], &texIndex[2], &normalIndex[2]);
				ASSERT(items == 9);
				for (int i = 0; i < 3; i++)
				{
					posIndices.push_back(posIndex[i] - 1);
					texIndices.push_back(texIndex[i] - 1);
					normalIndices.push_back(normalIndex[i] - 1);
				}
			}
		}
		file.close();

		int triNum = posIndices.size() / 3;
		for(int i = 0; i < triNum; i++)
		{
			Triangle<BlinnVertex> triangle;
			for (int j = 0; j < 3; j++)
			{
				int posIndex = posIndices[i * 3 + j];
				int texIndex = texIndices[i * 3 + j];
				int nIndex = normalIndices[i * 3 + j];
				triangle[j].ModelPos = { positions[posIndex], 1.0f };
				triangle[j].TexCoord = texCoords[texIndex];
				triangle[j].ModelNormal = normals[nIndex];
			}
			m_Mesh.emplace_back(triangle);
		}
	}
}