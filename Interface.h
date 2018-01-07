#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include "Tabs.h"
#include "Themes.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
void RenderInterface() {
	// Place all item settings under a collapsing header.

		static int page = 0;
		switch(g_Options.Menu.Theme)
		{
		case 0:
			RenderDefaultTheme();
			break;
		case 1:
			RenderMonoTheme();
			break;
		case 2:
			RenderRedTheme();
			break;;
		}
	
		
		ImGui::SetNextWindowSize(ImVec2(970.f, 450.f));
		if (ImGui::Begin(XorStr("Project-xInfinity"), &g_Options.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | G::extra_flags))
		{
			const char* tabs[] = {
				"Legit",
				"Visuals",
				"Misc",
                "Skinchanger",
				"Colors",
				"Config"
			};

			for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
			{

				if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 0)))
					page = i;

				if (i < IM_ARRAYSIZE(tabs) - 1)
					ImGui::SameLine();
			}

			ImGui::Separator();

			switch (page)
			{
			case 0:
				RenderLegit();
				break;
			case 1:
				RenderVisuals();
				break;
			case 2:
				RenderMisc();
				break;
			case 3:
				RenderSkinChanger();
				break;
			case 4:
				RenderColorsTab();
				break;
			case 5:
				RenderConfigTab();
				break;
			}
			ImGui::End();
		

		}
}
