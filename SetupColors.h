#pragma once

#include "Global.h"

//Colors
void SetupColors()
{

	//ESP
	Global::ColorsForPicker2.push_back(ColorP("Terrorist - Box Visible", g_Options.Visuals.espVisT));
	Global::ColorsForPicker2.push_back(ColorP("Terrorist - Box Invisible", g_Options.Visuals.espInvisT));
	Global::ColorsForPicker2.push_back(ColorP("Counter - Terrorist - Box Visible", g_Options.Visuals.espVisCT));
	Global::ColorsForPicker2.push_back(ColorP("Counter - Terrorist - Box Invisible", g_Options.Visuals.espInvisCT));

	//CHAMS
	Global::ColorsForPicker.push_back(ColorP("Terrorists - Chams_Visible", g_Options.Visuals.chamsPlyrTVis));
	Global::ColorsForPicker.push_back(ColorP("Terrorists - Chams_Invisible", g_Options.Visuals.chamsPlyrTInvis));
	Global::ColorsForPicker.push_back(ColorP("Counter_Terrorists - Chams_Visible", g_Options.Visuals.chamsPlyrCTVis));
	Global::ColorsForPicker.push_back(ColorP("Counter_Terrorists - Chams_Invisible", g_Options.Visuals.chamsPlyrCTInvis));

	//HANDS
	Global::ColorsForPicker3.push_back(ColorP("Chams T", g_Options.Visuals.chamsT));
	Global::ColorsForPicker3.push_back(ColorP("Chams CT", g_Options.Visuals.chamsCT));
}