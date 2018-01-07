#pragma once
#include "Hooks.h"
#include "Variables.h"
#include "ImGUI/imgui.h"
#include "ImGui\examples\directx9_example\imgui_impl_dx9.h"


namespace G // Global Stuff
{
	extern bool			Aimbotting;
	extern bool			InAntiAim;
	extern bool			Return;
	extern HMODULE		Dll;
	extern HWND			Window;
	extern bool			PressedKeys[256];
	extern bool			d3dinit;
	extern float		FOV;
	extern int			ChamMode;
	extern char			AnimatedClantag[16];
	extern bool			SendPacket;
	extern int			BestTarget;
	extern char			buf3[128];
	extern char			extra_flags;
}

namespace Global
{
	extern std::vector<ColorP> ColorsForPicker;
	extern std::vector<ColorP> ColorsForPicker2;
	extern std::vector<ColorP> ColorsForPicker3;
}