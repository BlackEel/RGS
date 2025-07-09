#include "Application.h"
#include "Window.h"

#include <string>
#include <iostream>

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
			OnUpdate();

			Window::PollInputEvents();
		}
	}

	void Application::OnUpdate()
	{
		if (m_Window->GetKey(RGS_KEY_0) == RGS_PRESS)
		{
			std::cout << "0 ±»°´ÏÂ" << std::endl;
		}

		Framebuffer framebuffer(m_Width, m_Height);
		framebuffer.Clear({ 1.0f,0.0f,1.0f });
		m_Window->DrawFramebuffer(framebuffer);
	}
}