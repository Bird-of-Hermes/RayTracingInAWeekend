#ifndef RT_WINDOW_H
#define RT_WINDOW_H

#define NO_MIN_MAX
#define WIN32_LEAN_AND_MEAN

#include "Windows.h"
#include <iostream>

class Application;
namespace Platform
{
	class Window
	{
		friend class Application;
	public:
		explicit Window(uint16_t Width = 800, uint16_t Height = 600, std::wstring_view windowName = L"Default Window");

	private:
		uint16_t m_Width;
		uint16_t m_Height;
		HINSTANCE m_hInstance = nullptr;
		HWND m_hwnd = nullptr;
		HDC m_hdc = nullptr;

		static LRESULT CALLBACK MsgHandling(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	};
};

#endif