#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* play_sound_t)(void*, const char*);
#define CLIENT_DLL "client.dll"
void __stdcall hkPlaySound(const char* szFileName)
{
	static auto ofunc = hooks::surface.get_original<play_sound_t>(82);
	//Call original PlaySound
	ofunc(g_Surface, szFileName);

	if (g_Options.Misc.AutoAccept && !strcmp(szFileName, "UI/competitive_accept_beep.wav"))
	{
		typedef void(*IsReadyCallBackFn)();

		IsReadyCallBackFn IsReadyCallBack = 0;

		if (!IsReadyCallBack)
		{
			IsReadyCallBack = (IsReadyCallBackFn)(U::FindPattern3(CLIENT_DLL, "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B 8E", 0));
		}

		if (IsReadyCallBack)
		{
			IsReadyCallBack();
		}
	}
}