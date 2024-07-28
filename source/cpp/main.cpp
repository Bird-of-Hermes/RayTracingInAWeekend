#include "../Raytracer.h"

int WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd)
{
	RaytracingInAWeekend raytracer;

	return (int)raytracer.Start(1280, 768, L"Software Raytracer");
}
