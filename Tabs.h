#pragma once
#include "IDirect3DDevice9.h"
#include "Global.h"


typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
void KnifeApplyCallbk()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(U::FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}
void dankestSetClanTag(const char* tag, const char* name)
{
	static auto ClanTagOffset = U::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
	if (ClanTagOffset)
	{
		if (strlen(name) > 0) {
			auto name_ = std::string(name);
			auto newline = name_.find("\\n");
			auto tab = name_.find("\\t");
			if (newline != std::string::npos) {
				name_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				name_.replace(tab, tab + 2, "\t");
			}
		}
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		dankesttSetClanTag(tag_.data(), tag_.data());
	}
}


void RenderLegit()
{
	ImGui::Checkbox(("Active"), &g_Options.Legitbot.MainSwitch);
	ImGui::Columns(5, nullptr, true);
	{
		ImGui::Text("General"); ImGui::NextColumn(); ImGui::Text("Rifles"); ImGui::NextColumn(); ImGui::Text("Pistol"); ImGui::NextColumn(); ImGui::Text("Sniper"); ImGui::NextColumn(); ImGui::Text("Triggerbot"); ImGui::NextColumn(); ImGui::Separator();

		ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.Aimbot.Enabled);
		//ImGui::Checkbox(("Auto Pistol"), &menu.Legitbot.Aimbot.AutoPistol);
		ImGui::Checkbox(("Backtrack Enabled"), &g_Options.Legitbot.backtrack);
		ImGui::Checkbox(("Backtrack Lines"), &g_Options.Legitbot.backtrackl);
	}
	//ImGui::Separator();
	ImGui::NextColumn();

	{
		ImGui::Hotkey("Key##0", &g_Options.Legitbot.MainKey);
		//ImGui::Combo(("Key##0"), &menu.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
		ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
		ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
	}

	ImGui::NextColumn();

	{
		ImGui::Hotkey("Key##1", &g_Options.Legitbot.PistolKey);
		//ImGui::Combo(("Key##1"), &menu.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
		ImGui::SliderFloat("Smooth##1", &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("FOV##1", &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
		ImGui::SliderFloat("Min RCS##1", &g_Options.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("Max RCS##1", &g_Options.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");
	}
	ImGui::NextColumn();

	{
		ImGui::Hotkey("Key##2", &g_Options.Legitbot.SniperKey);
		//ImGui::Combo(("Key##2"), &menu.Legitbot.SniperKey, keyNames, ARRAYSIZE(keyNames));
		ImGui::SliderFloat("Smooth##2", &g_Options.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("FOV##2", &g_Options.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
		ImGui::SliderFloat("Min RCS##2", &g_Options.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("Max RCS##2", &g_Options.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");
	}

	ImGui::NextColumn();

	{
		//ImGui::Separator();
		ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
		ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
		ImGui::Hotkey("Key##3", &g_Options.Legitbot.Triggerbot.Key);
		//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
		ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");

		ImGui::Text(("Filter"));

		ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 5));
		{
			ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
			ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
			ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
			ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
			ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
		} ImGui::EndChild();
	}
}


void RenderVisuals()
{
	ImGui::Checkbox(("Active"), &g_Options.Visuals.Enabled);

	ImGui::Columns(3, NULL, true);
	{

		ImGui::Text("Player"); ImGui::NextColumn(); ImGui::Text("Filter"); ImGui::NextColumn(); ImGui::Text("Misc"); ImGui::NextColumn(); ImGui::Separator();
		ImGui::Combo(("BoxType"), &g_Options.Visuals.Box, BoxMode, ARRAYSIZE(BoxMode));
		ImGui::Checkbox(("Health"), &g_Options.Visuals.HP);
		ImGui::Checkbox(("Name"), &g_Options.Visuals.Name);
		ImGui::Checkbox(("Weapon"), &g_Options.Visuals.Weapon);
		ImGui::Checkbox(("Skeleton"), &g_Options.Visuals.Skeleton);
		ImGui::Checkbox(("Snaplines"), &g_Options.Visuals.Snaplines); 
		ImGui::Checkbox(("DLight"), &g_Options.Visuals.DLight);
		ImGui::Checkbox(("GrenadeESP"), &g_Options.Visuals.GrenadeESP);
		ImGui::Checkbox(("C4"), &g_Options.Visuals.C4);

		ImGui::BeginChild(("ChamsInfo"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 63 * 2));
		{
		    ImGui::Separator();

			ImGui::Text("Chams Options");

			ImGui::Checkbox(("Enabled"), &g_Options.Visuals.ChamsEnabled);
			ImGui::Spacing();
			ImGui::PushItemWidth(150);  ImGui::Combo(("Chams"), &g_Options.Visuals.Chams, ChamsMode, ARRAYSIZE(ChamsMode));
			ImGui::Spacing();
			ImGui::Combo(("Hands"), &g_Options.Visuals.Hands, HandsMode, ARRAYSIZE(HandsMode));

		}ImGui::EndChild();

		ImGui::Spacing();

		ImGui::BeginChild(("DrawInfo"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 80 * 2)); //70
		{
			ImGui::Separator();
			ImGui::Text("Draw Options");
			ImGui::Checkbox(("Money"), &g_Options.Visuals.Money);
			ImGui::Checkbox(("IsScoped"), &g_Options.Visuals.IsScoped); 
			ImGui::Checkbox(("IsHasDefuser"), &g_Options.Visuals.IsHasDefuser);
			ImGui::Checkbox(("IsDefusing"), &g_Options.Visuals.IsDefusing); 
			ImGui::Checkbox(("IsFlashed"), &g_Options.Visuals.IsFlashed); 
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();

		}ImGui::EndChild();

	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 57 * 1.95));
		{
			ImGui::Checkbox((" All Players"), &g_Options.Visuals.Filter.Players);
			ImGui::Checkbox((" Enemy Only"), &g_Options.Visuals.Filter.EnemyOnly);
			ImGui::Checkbox((" Visible Only"), &g_Options.Visuals.Filter.VisibleOnly);
			ImGui::Checkbox((" Local Player"), &g_Options.Visuals.Filter.pLocalPlayer);

		} ImGui::EndChild();

		ImGui::Text("World");
		ImGui::Checkbox(("C4 - World"), &g_Options.Visuals.C4World);
		ImGui::Checkbox(("Weapons - World"), &g_Options.Visuals.WeaponsWorld);

	}


	ImGui::NextColumn();
	{
		ImGui::Checkbox(("No Visual Recoil"), &g_Options.Visuals.NoVisualRecoil);
		ImGui::SliderFloat(("viewmodel FOV"), &g_Options.Visuals.viewmodelChanger, 0.f, 120.f, "%.0f");
		ImGui::Checkbox(("NoFlash"), &g_Options.Visuals.NoFlash);
		ImGui::Checkbox(("NoSmoke"), &g_Options.Visuals.NoSmoke);
		ImGui::Checkbox(("noScope Border"), &g_Options.Visuals.noscopeborder);
		ImGui::Combo(("CrosshairsStyles"), &g_Options.Visuals.CrosshairOptions, Crosshairoptions, ARRAYSIZE(Crosshairoptions));
		ImGui::Checkbox(("Grenade Prediction"), &g_Options.Visuals.GrenadePrediction);
		ImGui::Checkbox(("ThirdPerson"), &g_Options.Visuals.ThirdPerson);
		ImGui::SliderFloat(("Ingame - FOV"), &g_Options.Visuals.FOVChanger, 0.f, 60.f, "%.0f");
	}

}

void RenderMisc()
{
	ImGui::Columns(3, NULL, true);
	{
		ImGui::Text(("Part-1"));
		ImGui::Checkbox("Name Stealer", &g_Options.Misc.silentstealer);
		ImGui::SameLine();
		if (ImGui::Button("Silent exploit"))
		{
			static ConVar* name = g_CVar->FindVar("name");
			if (name)
			{
				*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
				name->SetValue("\n���");
			}
		}
		ImGui::Checkbox(("Bunny Hop"), &g_Options.Misc.Bhop);
		ImGui::Checkbox(("WatermarkOFF"), &g_Options.Misc.Watermark);
		ImGui::Checkbox(("AutoAccept"), &g_Options.Misc.AutoAccept);
		ImGui::Checkbox(("Hitmarker"), &g_Options.Misc.Hitmarker);
		ImGui::Combo(("HitmarkerColor"), &g_Options.Misc.HitMarkerColor, HitMarkerClrType, ARRAYSIZE(HitMarkerClrType));
		ImGui::Checkbox(("AutoStrafe"), &g_Options.Misc.AutoStrafe);
		ImGui::Checkbox(("Competitive Rank"), &g_Options.Misc.ServerRankRevealAll);
		ImGui::Checkbox(("SpecList"), &g_Options.Misc.SpecList);

	}
	ImGui::NextColumn();
	{
		ImGui::Text(("Part-2"));
		ImGui::Combo(("Spammer"), &g_Options.Misc.spammer, spammers, ARRAYSIZE(spammers));
		ImGui::Checkbox("xInfinity clantag", &g_Options.Misc.syncclantag);
		ImGui::PushItemWidth(100);
		ImGui::Text(("Custom Name"));
		ImGui::PushItemWidth(180);
		static char buf1[128] = ""; ImGui::InputText(("##Name"), buf1, 128, ImGuiInputTextFlags_AllowTabInput);
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::Checkbox("NameSpammer", &g_Options.Misc.namespam);
		if (ImGui::Button(("Change Name")))
		{
			ConVar* name = g_CVar->FindVar("name");
			if (name)
			{
				*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
				name->SetValue(buf1);
			}
		}
		ImGui::Text(("Custom ClanTag"));
		ImGui::PushItemWidth(180);
		static char buf2[128] = ""; ImGui::InputText(("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		if (ImGui::Button(("Change Clantag")))
		{
			dankestSetClanTag(buf2, buf2);
		}

		ImGui::Text(("AnimatedClantag"));
		ImGui::PushItemWidth(180); ImGui::InputText(("##AnimatedClantag"), G::AnimatedClantag, 16, ImGuiInputTextFlags_AllowTabInput);
		ImGui::PushItemWidth(200);
		ImGui::SliderInt(("Speed"), &g_Options.Misc.ClanTagSpeed, 0, 1000, "%.0f");
		ImGui::PushItemWidth(50);
		if (ImGui::Button(("Start")))
		{
			g_Options.Misc.animatedclantag = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(("Stop")))
		{
			g_Options.Misc.animatedclantag = false;
		}


	}
	ImGui::NextColumn();
	{
		ImGui::Text(("Part-3"));
		ImGui::Checkbox(("Chrome - World"), &g_Options.Misc.ChromeWorld);
		ImGui::Checkbox(("NightMode - World"), &g_Options.Misc.nightMode);
		ImGui::Checkbox(("DisablePostProcessing"), &g_Options.Misc.disablepost);
		ImGui::PushItemWidth(120); ImGui::Combo(("Theme Changer"), &g_Options.Menu.Theme, themes, ARRAYSIZE(themes));
	}

}

void RenderSkinChanger()
{
	{
		ImGui::Checkbox(("Enabled"), &g_Options.Skinchanger.Enabled);
		ImGui::SameLine();
		ImGui::PushItemWidth(150);
		if (ImGui::Button(("Force Update")))
		{
			KnifeApplyCallbk();
		}


		//ImGui::Separator();

		ImGui::Text("General");
		ImGui::Combo(("Knife Model"), &g_Options.Skinchanger.Knife, knives, ARRAYSIZE(knives));
		ImGui::Combo(("Knife Skin"), &g_Options.Skinchanger.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
		ImGui::Combo(("Gloves"), &g_Options.Skinchanger.gloves, gloves, ARRAYSIZE(gloves));


		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Rifles");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("AK-47"), &g_Options.Skinchanger.AK47Skin, ak47, ARRAYSIZE(ak47));
		ImGui::Combo(("M4A1-S"), &g_Options.Skinchanger.M4A1SSkin, m4a1s, ARRAYSIZE(m4a1s));
		ImGui::Combo(("M4A4"), &g_Options.Skinchanger.M4A4Skin, m4a4, ARRAYSIZE(m4a4));
		ImGui::NextColumn();
		ImGui::Combo(("AUG"), &g_Options.Skinchanger.AUGSkin, aug, ARRAYSIZE(aug));
		ImGui::Combo(("FAMAS"), &g_Options.Skinchanger.FAMASSkin, famas, ARRAYSIZE(famas));
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Snipers");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("AWP"), &g_Options.Skinchanger.AWPSkin, awp, ARRAYSIZE(awp));
		ImGui::Combo(("SSG08"), &g_Options.Skinchanger.SSG08Skin, ssg08, ARRAYSIZE(ssg08));
		ImGui::NextColumn();
		ImGui::Combo(("SCAR20"), &g_Options.Skinchanger.SCAR20Skin, scar20, ARRAYSIZE(scar20));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("SMG's");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("P90"), &g_Options.Skinchanger.P90Skin, p90, ARRAYSIZE(p90));
		ImGui::NextColumn();
		ImGui::Combo(("UMP45"), &g_Options.Skinchanger.UMP45Skin, ump45, ARRAYSIZE(ump45));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Pistols");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("Glock-18"), &g_Options.Skinchanger.GlockSkin, glock, ARRAYSIZE(glock));
		ImGui::Combo(("USP-S"), &g_Options.Skinchanger.USPSkin, usp, ARRAYSIZE(usp));
		ImGui::Combo(("Deagle"), &g_Options.Skinchanger.DeagleSkin, deagle, ARRAYSIZE(deagle));

		ImGui::NextColumn();
		ImGui::Combo(("TEC-9"), &g_Options.Skinchanger.tec9Skin, tec9, ARRAYSIZE(tec9));
		ImGui::Combo(("P2000"), &g_Options.Skinchanger.P2000Skin, p2000, ARRAYSIZE(p2000));
		ImGui::Combo(("P250"), &g_Options.Skinchanger.P250Skin, p250, ARRAYSIZE(p250));

	}

}


void RenderColorsTab()
{
	
	ImGui::BulletText("CHAMS - COLORS");

	ImGui::Spacing();
	ImGui::BeginChild("##CHAMS-COLORS", ImVec2(500, 150), true);
	{

		static int selectedItem = 0;

		ImVec2 lastCursor = ImGui::GetCursorPos();

		auto& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);

		ImGui::ListBoxHeader("##CHAMS-Visuals", ImVec2(320, 95)); //417

		for (int i = 0; i < Global::ColorsForPicker.size(); i++)
		{
			bool selected = i == selectedItem;

			if (ImGui::Selectable(Global::ColorsForPicker[i].Name, selected))
				selectedItem = i;
		}
		ImGui::ListBoxFooter();


		float nc = lastCursor.x + 260;
		ImGui::SetCursorPos(ImVec2(nc, lastCursor.y));

		ColorP col = Global::ColorsForPicker[selectedItem];

		int r = (col.Ccolor[0] * 255.f);
		int g = (col.Ccolor[1] * 255.f);
		int b = (col.Ccolor[2] * 255.f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
		ImGui::SetCursorPosX(nc);


		col.Ccolor[0] = r / 255.f;
		col.Ccolor[1] = g / 255.f;
		col.Ccolor[2] = b / 255.f;


		ImGui::SameLine();

		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 curWindowPos = ImGui::GetWindowPos();
		curPos.x += curWindowPos.x;
		curPos.y += curWindowPos.y;

		ImGui::PushItemWidth(372);
		ImGui::NewLine();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::ColorEdit3("##clr", col.Ccolor);

	}ImGui::EndChild();

	ImGui::BulletText("ESP - COLORS");

	ImGui::BeginChild("##ESP - COLORS", ImVec2(500, 150), true);
	{

		static int selectedItem2 = 0;

		ImVec2 lastCursor = ImGui::GetCursorPos();

		auto& style2 = ImGui::GetStyle();

		style2.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

		style2.WindowPadding = ImVec2(8, 8);
		style2.ItemSpacing = ImVec2(4, 4);

		ImGui::ListBoxHeader("##ESP - COLORS", ImVec2(320, 95));

		for (int i2 = 0; i2 < Global::ColorsForPicker2.size(); i2++)
		{
			bool selected2 = i2 == selectedItem2;

			if (ImGui::Selectable(Global::ColorsForPicker2[i2].Name, selected2))
				selectedItem2 = i2;
		}
		ImGui::ListBoxFooter();


		float nc2 = lastCursor.x + 260;
		ImGui::SetCursorPos(ImVec2(nc2, lastCursor.y));

		ColorP col2 = Global::ColorsForPicker2[selectedItem2];
		int resp = (col2.Ccolor[0] * 255.f);
		int gesp = (col2.Ccolor[1] * 255.f);
		int besp = (col2.Ccolor[2] * 255.f);


		style2.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
		ImGui::SetCursorPosX(nc2);


		col2.Ccolor[0] = resp / 255.f;
		col2.Ccolor[1] = gesp / 255.f;
		col2.Ccolor[2] = besp / 255.f;

		ImGui::SameLine();

		ImVec2 curPos2 = ImGui::GetCursorPos();
		ImVec2 curWindowPos2 = ImGui::GetWindowPos();
		curPos2.x += curWindowPos2.x;
		curPos2.y += curWindowPos2.y;

		ImGui::PushItemWidth(372);
		ImGui::NewLine();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::ColorEdit3("##clr2", col2.Ccolor);

	}ImGui::EndChild();


	ImGui::BulletText("HANDS - COLORS");

	ImGui::Spacing();
	ImGui::BeginChild("##HANDS-COLORS", ImVec2(500, 120), true);
	{
		
		static int selectedItem3 = 0;

		ImVec2 lastCursor = ImGui::GetCursorPos();

		auto& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);

		ImGui::ListBoxHeader("##HANDS-COLORS", ImVec2(320, 55)); 

		for (int i = 0; i < Global::ColorsForPicker3.size(); i++)
		{
			bool selected = i == selectedItem3;

			if (ImGui::Selectable(Global::ColorsForPicker3[i].Name, selected))
				selectedItem3 = i;
		}
		ImGui::ListBoxFooter();


		float nc = lastCursor.x + 260;
		ImGui::SetCursorPos(ImVec2(nc, lastCursor.y));

		ColorP col3 = Global::ColorsForPicker3[selectedItem3];

		int rhands = (col3.Ccolor[0] * 255.f);
		int ghands = (col3.Ccolor[1] * 255.f);
		int bhands = (col3.Ccolor[2] * 255.f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
		ImGui::SetCursorPosX(nc);


		col3.Ccolor[0] = rhands / 255.f;
		col3.Ccolor[1] = ghands / 255.f;
		col3.Ccolor[2] = bhands / 255.f;


		ImGui::SameLine();

		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 curWindowPos = ImGui::GetWindowPos();
		curPos.x += curWindowPos.x;
		curPos.y += curWindowPos.y;

		ImGui::PushItemWidth(372);
		ImGui::NewLine();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::ColorEdit3("##clr", col3.Ccolor);

	}ImGui::EndChild();

}

void RenderConfigTab()
{
	ImGui::BeginChild("Config", ImVec2(500, 120), true);
	{
		ImGui::Text("Config");
		ImGui::Combo(("File"), &g_Options.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
		if (ImGui::Button("Save Config")) Config->Save();
		ImGui::SameLine();
		if (ImGui::Button("Load Config")) Config->Load();
		ImGui::SameLine();
		if (ImGui::Button("Unload"))
		{
			g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
			unload = true;
		}
		ImGui::SameLine();
		if (ImGui::Hotkey("PanicButton", &g_Options.Menu.panicbutton, ImVec2(110, 25)))
		{
			g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
			unload = true;
		}

	}ImGui::EndChild();

}

