#ifndef RT_RENDERER_H
#define RT_RENDERER_H

#include "RT_Window.h"
#include "ErrorEnum.h"
#include "NaiveMath.h"
#include <chrono>

static constexpr size_t BACKBUFFERCOUNT = 2;

class Application
{
public:
	constexpr Application() {}
	virtual ~Application()
	{
		for (size_t i = 0; i < BACKBUFFERCOUNT; ++i)
		{
			delete[] m_backBuffers[i];
			if (m_bitMaps[i])
			{
				DeleteObject(m_bitMaps[i]);
			}
			if (m_bitMapsHDC[i])
			{
				DeleteDC(m_bitMapsHDC[i]);
			}
		}
		if (m_windowContext->m_hwnd && m_windowContext->m_hdc)
		{
			ReleaseDC(m_windowContext->m_hwnd, m_windowContext->m_hdc);
		}
	}

	// if dt > 1.0f to check if a full second has passed
	virtual void OnUpdate(float dt) noexcept = 0;

	RESULT_VALUE Start(uint16_t width = 800, uint16_t height = 600, std::wstring_view windowName = L"My Application") noexcept
	{
		m_windowContext = std::make_unique<Platform::Window>(width, height, windowName.data());
		m_drawingRectangle.top = 0;
		m_drawingRectangle.left = 0;
		m_drawingRectangle.right = width;
		m_drawingRectangle.bottom = height;

		// For User
		canvasWidth = width;
		canvasHeight = height;

		CreateBackBuffers();

		return Loop();
	}

	void DrawPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, size_t currentSampleIndex = 1) noexcept
	{
		if (x >= m_windowContext->m_Width || y >= m_windowContext->m_Height)
		{
			return;
		}
		if (currentSampleIndex > 1)
		{
			DrawPixelAccumulate(x, y, red, green, blue, currentSampleIndex);
			return;
		}
		const size_t index = (static_cast<size_t>(y) * m_windowContext->m_Width + x) * 3;

		m_backBuffers[presentBufferIndex][index] = blue;
		m_backBuffers[presentBufferIndex][index + 1] = green;
		m_backBuffers[presentBufferIndex][index + 2] = red;
	}

	void DrawPixel(uint16_t x, uint16_t y, const Vec3f& rgb, size_t currentSampleIndex = 1) noexcept
	{
		if (x >= m_windowContext->m_Width || y >= m_windowContext->m_Height)
		{
			return;
		}
		if (currentSampleIndex > 1)
		{
			DrawPixelAccumulate(x, y, rgb, currentSampleIndex);
			return;
		}
		const size_t index = (static_cast<size_t>(y) * m_windowContext->m_Width + x) * 3;

		m_backBuffers[presentBufferIndex][index] = static_cast<uint8_t>(rgb.b * 255.0f);
		m_backBuffers[presentBufferIndex][index + 1] = static_cast<uint8_t>(rgb.g * 255.0f);
		m_backBuffers[presentBufferIndex][index + 2] = static_cast<uint8_t>(rgb.r * 255.0f);
	}

	void SetWindowTitle(std::wstring_view name) const noexcept
	{
		SetWindowTextW(m_windowContext->m_hwnd, name.data());
	}

	void SetWindowTitle(std::string_view name) const noexcept
	{
		SetWindowTextA(m_windowContext->m_hwnd, name.data());
	}

	void ClearScreenEveryFrame(bool value) noexcept
	{
		m_clearScreen = value;
	}

protected:
	// User-Utility
	size_t canvasWidth = 800;
	size_t canvasHeight = 600;
	size_t currentFPS = 0;
	size_t frameIndex = 1;

private:
	void CreateBackBuffers()
	{
		const LONG width = m_windowContext->m_Width;
		const LONG height = m_windowContext->m_Height;

		for (size_t i = 0; i < BACKBUFFERCOUNT; i++)
		{
			if (m_backBuffers[i] == nullptr)
			{
				m_backBuffers[i] = new BYTE[width * height * 3]{ 0 };
			}
		}

		for (size_t i = 0; i < BACKBUFFERCOUNT; ++i)
		{
			m_bitMapsHDC[i] = CreateCompatibleDC(m_windowContext->m_hdc);
			m_bitMaps[i] = CreateCompatibleBitmap(m_bitMapsHDC[i], m_windowContext->m_Width, m_windowContext->m_Height);
			SelectObject(m_bitMapsHDC[i], m_bitMaps[i]);
		}

		m_accumulationBuffer.resize(static_cast<size_t>(width) * height * 3);
		memset(m_accumulationBuffer.data(), 0, m_accumulationBuffer.size() * sizeof(float));
	}
	void AdvanceBufferIndex() noexcept
	{
		presentBufferIndex = (presentBufferIndex + 1) % BACKBUFFERCOUNT;
	}
	void DrawPixelAccumulate(uint16_t x, uint16_t y, const Vec3f& color, size_t currentSampleIndex = 1) noexcept
	{
		const size_t index = (static_cast<size_t>(y) * m_windowContext->m_Width + x) * 3;
		m_accumulationBuffer[index    ] += color.r;
		m_accumulationBuffer[index + 1] += color.g;
		m_accumulationBuffer[index + 2] += color.b;

		const Vec3f averagedColor =
		{
			m_accumulationBuffer[index    ] / (currentSampleIndex + 1),
			m_accumulationBuffer[index + 1] / (currentSampleIndex + 1),
			m_accumulationBuffer[index + 2] / (currentSampleIndex + 1)
		};

		m_backBuffers[presentBufferIndex][index    ] = static_cast<uint8_t>(averagedColor.b * 255.0f);
		m_backBuffers[presentBufferIndex][index + 1] = static_cast<uint8_t>(averagedColor.g * 255.0f);
		m_backBuffers[presentBufferIndex][index + 2] = static_cast<uint8_t>(averagedColor.r * 255.0f);
	}
	void DrawPixelAccumulate(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, size_t currentSampleIndex = 1) noexcept
	{
		const size_t index = (static_cast<size_t>(y) * m_windowContext->m_Width + x) * 3;
		m_accumulationBuffer[index    ] += red;
		m_accumulationBuffer[index + 1] += green;
		m_accumulationBuffer[index + 2] += blue;

		const Vec3f averagedColor =
		{
			m_accumulationBuffer[index    ] / (currentSampleIndex + 1),
			m_accumulationBuffer[index + 1] / (currentSampleIndex + 1),
			m_accumulationBuffer[index + 2] / (currentSampleIndex + 1)
		};

		m_backBuffers[presentBufferIndex][index    ] = static_cast<uint8_t>(averagedColor.b);
		m_backBuffers[presentBufferIndex][index + 1] = static_cast<uint8_t>(averagedColor.g);
		m_backBuffers[presentBufferIndex][index + 2] = static_cast<uint8_t>(averagedColor.r);
	}
	void Present() noexcept
	{
		const HDC hdcMem = m_bitMapsHDC[presentBufferIndex];
		const HBITMAP hBitmap = m_bitMaps[presentBufferIndex];
		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biWidth = m_windowContext->m_Width;
		bmi.bmiHeader.biHeight = static_cast<LONG>(-m_windowContext->m_Height);
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biCompression = BI_RGB;

		SelectObject(hdcMem, hBitmap);
		SetDIBitsToDevice(m_windowContext->m_hdc, 0, 0, m_windowContext->m_Width, m_windowContext->m_Height, 0, 0, 0, m_windowContext->m_Height, m_backBuffers[presentBufferIndex], &bmi, DIB_RGB_COLORS);

		AdvanceBufferIndex();
	}
	void ClearScreen() noexcept
	{
		const size_t bufferSize = (static_cast<size_t>(m_windowContext->m_Height) * m_windowContext->m_Width) * 3;
		memset(m_backBuffers[presentBufferIndex], 0, bufferSize);
	}
	RESULT_VALUE Loop()
	{
		auto last = std::chrono::steady_clock::now();
		MSG msg = {};

		while (true)
		{
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					return RESULT_VALUE::OK;
				}

				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}

			const auto now = std::chrono::steady_clock::now();
			const auto elapsed = now - last;
			last = now;
			const float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0f;

			currentFPS = static_cast<size_t>(1.0 / deltaTime);

			if (m_clearScreen)
			{
				ClearScreen();
			}
			OnUpdate(deltaTime);
			Present();

			++frameIndex;
		}

		return RESULT_VALUE::OK;
	}

private:
	// Content Window
	std::unique_ptr<Platform::Window> m_windowContext = {nullptr};

	// Data
	RECT m_drawingRectangle;
	size_t presentBufferIndex = 0;
	HDC m_bitMapsHDC[BACKBUFFERCOUNT] = { nullptr };
	HBITMAP m_bitMaps[BACKBUFFERCOUNT] = { nullptr };
	BYTE* m_backBuffers[BACKBUFFERCOUNT] = { nullptr };
	std::vector<float> m_accumulationBuffer;
	bool m_clearScreen = true;
};

#endif