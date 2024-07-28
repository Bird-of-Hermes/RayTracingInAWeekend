#include "RT_Window.h"

namespace Platform
{
	Window::Window(uint16_t width, uint16_t height, std::wstring_view windowName) : m_Height(height), m_Width(width)
	{
		WNDCLASSEX wc = { };

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = MsgHandling;
		wc.hInstance = this->m_hInstance;
		wc.lpszClassName = L"RTWindowProgram";

		RegisterClassEx(&wc);

        m_hwnd = CreateWindowExW(0, L"RTWindowProgram", windowName.data(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, this->m_hInstance, nullptr);

        ShowWindow(m_hwnd, SW_SHOWNORMAL);

		m_hdc = GetDC(m_hwnd);
	}
	
	LRESULT Window::MsgHandling(_In_ HWND hWND, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (Msg)
		{
			case WM_DESTROY:
			{
				PostQuitMessage(0); 
				return 0;
			}
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}
			case WM_QUIT:
			{
				PostQuitMessage(0);
				return 0;
			}
		}
		return DefWindowProc(hWND, Msg, wParam, lParam);
	}
};
