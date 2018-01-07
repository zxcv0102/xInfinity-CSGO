#pragma once

#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"


visuals::visuals()
{
	BombCarrier = nullptr;
}

int width = 0;
int height = 0;
bool done = false;
void visuals::OnPaintTraverse(C_BaseEntity* local)
{
	ESPBox Box;

	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{

		C_BaseEntity *entity = g_EntityList->GetClientEntity(i);
		player_info_t pinfo;
		if (entity == local && local->IsAlive() && g_Engine->GetPlayerInfo(g_Engine->GetLocalPlayer(), &pinfo))
		{
			if (g_Input->m_fCameraInThirdPerson && g_Options.Visuals.Enabled)
			{
				Vector max = entity->GetCollideable()->OBBMaxs();
				Vector pos, pos3D;
				Vector top, top3D;
				pos3D = entity->GetOrigin();
				top3D = pos3D + Vector(0, 0, max.z);

				if (!g_Render->WorldToScreen(pos3D, pos) || !g_Render->WorldToScreen(top3D, top))
					return;

				float height = (pos.y - top.y);
				float width = height / 4.f;

				if (g_Options.Visuals.Filter.pLocalPlayer)
				{

					if (g_Options.Visuals.Box)
					{
						Color color;
						color = GetPlayerColor(entity, local);
						PlayerBox(top.x, top.y, width, height, color);

					}
					if (g_Options.Visuals.HP)
					{
						DrawHealth(pos, top, local->GetHealth());
					}

					if (g_Options.Visuals.Name)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);
					}
				}
			}
		}

		if (entity && entity != local && !entity->IsDormant())
		{
			if (g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
			{
				if (g_Options.Legitbot.backtrackl)
				{
					if (local->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
							{
								if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									g_Surface->DrawSetColor(Color::Red());
									g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}

				if (g_Options.Visuals.Enabled && g_Options.Visuals.Filter.Players)
				{
					if (g_Options.Visuals.DLight)
						DLight(local, entity);
					DrawPlayer(entity, pinfo, local);
				}

			}
			if (g_Options.Visuals.Enabled)
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();
				if (g_Options.Visuals.WeaponsWorld && cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
				{
					DrawDrop(entity);
				}
				if (g_Options.Visuals.C4World)
				{
					if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
						DrawBombPlanted(entity, local);
				}

				if (cClass->m_ClassID == (int)ClassID::CC4)
					DrawBomb(entity, cClass);
				if (g_Options.Visuals.GrenadeESP && strstr(cClass->m_pNetworkName, "Projectile"))
				{
					DrawThrowable(entity);
				}
			}
		}
	}
	if (g_Options.Misc.SpecList) SpecList(local);

	NightMode();
	grenade_prediction::instance().Paint();


	if (g_Options.Visuals.CrosshairOptions == 1)
	{
		g_Engine->GetScreenSize(width, height);
		if (local  && local->IsAlive())
		{
			static Vector ViewAngles;
			g_Engine->GetViewAngles(ViewAngles);
			ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

			Vector fowardVec;
			AngleVectors(ViewAngles, &fowardVec);
			fowardVec *= 10000;

			Vector start = local->GetOrigin() + local->GetViewOffset();
			Vector end = start + fowardVec, endScreen;

			if (g_Render->WorldToScreen(end, endScreen) && local->IsAlive())
			{
				g_Render->Line(endScreen.x - 4, endScreen.y, endScreen.x + 4, endScreen.y, Color(255, 255, 0, 255));
				g_Render->Line(endScreen.x, endScreen.y - 4, endScreen.x, endScreen.y + 4, Color(255, 255, 0, 255));
			}
		}
	}
	if (g_Options.Visuals.CrosshairOptions == 2)
	{
		g_Engine->GetScreenSize(width, height);
		if (local  && local->IsAlive())
		{
			static Vector ViewAngles;
			g_Engine->GetViewAngles(ViewAngles);
			ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

			static Vector fowardVec;
			AngleVectors(ViewAngles, &fowardVec);
			fowardVec *= 10000;

			// Get ray start / end
			Vector start = local->GetOrigin() + local->GetViewOffset();
			Vector end = start + fowardVec, endScreen;

			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
			float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
			if (cone > 0.0f)
			{
				if (cone < 0.01f) cone = 0.01f;
				float size = (cone * height) * 0.7f;
				Color color(255, 0, 0, 100);

				if (g_Render->WorldToScreen(end, endScreen))
				{
					g_Render->OutlineCircle(endScreen.x, endScreen.y, (int)size, 48, color);
				}
			}
		}
	}
}

std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}


C_CSPlayerResource* playerresources;
void visuals::DrawPlayer(C_BaseEntity* entity, player_info_t pinfo, C_BaseEntity* local)
{
	ESPBox Box;
	Vector max = entity->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = entity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);

	if (!g_Render->WorldToScreen(pos3D, pos) || !g_Render->WorldToScreen(top3D, top))
		return;

	float height = (pos.y - top.y);
	float width = height / 4.f;

	Color color;
	ESPBox size;
	
	if (g_Options.Visuals.Filter.EnemyOnly && (entity->GetTeamNum() == local->GetTeamNum())) 
	return;

	color = GetPlayerColor(entity, local);

	if (g_Options.Visuals.Box)
	{
		Color color;
		color = GetPlayerColor(entity, local);
		PlayerBox(top.x, top.y, width, height, color);
	}

	if (g_Options.Visuals.Name)
	{
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);

	}

	if (g_Options.Visuals.HP)
	{
		DrawHealth(pos, top, entity->GetHealth());
	}
	

	if (g_Options.Visuals.Snaplines)
	{
		Snaplines(Box, Color::Red(), entity);
	}

	if (g_Options.Visuals.Skeleton)
	{
		DrawSkeleton(entity);
	}


	std::vector<std::string> weapon;
	std::vector<std::string> bomb;
	std::vector<std::string> rank;
	std::vector<std::string> wins;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());
	if (g_Options.Visuals.Weapon && pWeapon)
	{
		int weapon_id = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();

		auto weapon_name = ItemDefinitionIndexToString(weapon_id);
		weapon.push_back(weapon_name);
	}

	if (g_Options.Visuals.C4 && entity == BombCarrier)
	{
		bomb.push_back("Bomb");
	}

	//Definitions
	int i = 0;
	int bottom = 0;

	if (g_Options.Visuals.Weapon)
	{
		for (auto Text : weapon)
		{
			g_Render->DrawString2(g_Render->font.Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::White(), FONT_CENTER, "%s", Text.c_str());
			i++;
		}
	}
	if (g_Options.Visuals.C4)
	{
		for (auto Text : bomb)
		{
			g_Render->DrawString2(g_Render->font.Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::Red(), FONT_CENTER, Text.c_str());
			i++;
		}
	}


	if (g_Options.Visuals.Money)
	{
		g_Render->Textf(int(top.x + width + 3), int(top.y + 12), Color(255, 255, 255, 255), g_Render->font.ESP, "$%i", entity->GetMoney());
	}


	if (g_Options.Visuals.IsScoped && entity->IsScoped())
	{
		int TeamCheck = entity->GetTeamNum();

		if (TeamCheck == TEAM_CS_T && MiscFunctions::IsSniper)
		{
			g_Render->DrawString2(g_Render->font.ESP, (int)top.x + width + 3, int(top.y + 7), Color::Red(), FONT_CENTER, "*Scoped*");
		}
		else
		{
			g_Render->DrawString2(g_Render->font.ESP, (int)top.x + width + 3, int(top.y + 7), Color::Red(), FONT_CENTER, "*Scoped*");

		}
	}

	if (g_Options.Visuals.IsHasDefuser && entity->hasDefuser())
	{
		int TeamCheck = entity->GetTeamNum();

		if (TeamCheck == TEAM_CS_T)
		{
			g_Render->DrawString2(g_Render->font.ESP, (int)top.x, int(top.y + height + 8 + (14 * bottom++)), Color::LightBlue(), FONT_CENTER, "Has Defuser");
			i++;
		}
		else
		{
			g_Render->DrawString2(g_Render->font.ESP, (int)top.x, int(top.y + height + 8 + (14 * bottom++)), Color::LightBlue(), FONT_CENTER, "Has Defuser");
			i++;
		}

	}

	if (g_Options.Visuals.IsDefusing && entity->IsDefusing())
	{
		int TeamCheck = entity->GetTeamNum();

		if (TeamCheck == TEAM_CS_CT)
		{
			g_Render->DrawString2(g_Render->font.ESP, (int)top.x, int(top.y + height + 8 + (14 * bottom++)), Color::LightBlue(), FONT_CENTER, "Defusing");
			i++;
		} 
	}

	if (g_Options.Visuals.IsFlashed &&  entity->IsFlashed())
	{
		int TeamCheck = entity->GetTeamNum();

		if (TeamCheck == TEAM_CS_T)
		{
			g_Render->DrawString2(g_Render->font.ESP, (int)top.x + width + 1, int(top.y + 6), Color::Red(), FONT_CENTER, "Flashed");
			i++;
		}
	}


}

void visuals::PlayerBox(float x, float y, float w, float h, Color clr)
{
	if (g_Options.Visuals.Box == 1)
	{
		g_Surface->DrawSetColor(clr);
		g_Surface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
		g_Surface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
	}

	if (g_Options.Visuals.Box == 2)
	{
		g_Surface->DrawSetColor(clr);
		g_Surface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
		g_Surface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
		g_Surface->DrawOutlinedRect(int(x - w + 1), int(y + 1), int(x + w - 1), int(y + h - 1));
	}
}


Color visuals::GetPlayerColor(C_BaseEntity* entity, C_BaseEntity* local)
{
	int TeamCheck = entity->GetTeamNum();
	bool IsVis = MiscFunctions::IsVisible(local, entity, Head);

	if (TeamCheck == TEAM_CS_T)
	{
		if (IsVis)
		{
			return Color(g_Options.Visuals.espVisT[0] * 255, g_Options.Visuals.espVisT[1] * 255, g_Options.Visuals.espVisT[2] * 255);
		}
		else 
		{
			return Color(g_Options.Visuals.espInvisT[0] * 255, g_Options.Visuals.espInvisT[1] * 255, g_Options.Visuals.espInvisT[2] * 255);
		}

	}
	else 
	{
		if (IsVis)
		{
			return Color(g_Options.Visuals.espVisCT[0] * 255, g_Options.Visuals.espVisCT[1] * 255, g_Options.Visuals.espVisCT[2] * 255);
		}
		else
		{
			return Color(g_Options.Visuals.espInvisCT[0] * 255, g_Options.Visuals.espInvisCT[1] * 255, g_Options.Visuals.espInvisCT[2] * 255);
		}

	}
	return Color(255, 255, 255);
}

//==============================================================================//

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void visuals::DrawHealth(Vector bot, Vector top, float health)
{
	
		float h = (bot.y - top.y);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;

		UINT hp = UINT(h - (UINT)((h * clamp(health, 0, 100)) / 100));  // Percentage

		int Red = int(255 - (health*2.55));
		int Green = int(health*2.55);

		g_Render->DrawOutlinedRect(int((top.x - offset) - 1), int(top.y - 1), 3, int(h + 2), Color::Black());
		g_Render->Line(int((top.x - offset)), int(top.y + hp), int((top.x - offset)), int(top.y + h), Color(Red, Green, 0, 255));

}

//==============================================================================//

void visuals::Snaplines(visuals::ESPBox size, Color color, C_BaseEntity* entity)
{
	C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	Vector src3D, src;
	src3D = entity->GetOrigin() - Vector(0, 0, 0);

	if (!g_Render->WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	g_Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;

	y = ScreenHeight;

	if (local->IsAlive() && !g_Input->m_fCameraInThirdPerson)
	{
		g_Render->Line((int)(src.x), (int)(src.y), x, y, Color(255, 0, 0, 255));
	}
}


void visuals::DrawSkeleton(C_BaseEntity* entity)
{
	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(entity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & 0x100) && (pBone->parent != -1))
		{
			vChild = entity->GetBonePos(j);
			vParent = entity->GetBonePos(pBone->parent);

				if (g_Render->WorldToScreen(vParent, sParent) && g_Render->WorldToScreen(vChild, sChild))
				{
					g_Render->Line(sParent.x, sParent.y, sChild.x, sChild.y, Color(255, 255, 255, 200));
				}
		}
	}
}

void visuals::DrawDrop(C_BaseEntity* entity)
{
    if (entity)
    {
        CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

        auto owner = pWeapon->GetOwnerHandle();

        if (owner > -1)
            return;

        Vector pos3D = entity->GetAbsOrigin2();

        if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
            return;

        Vector pos;

        if (!g_Render->WorldToScreen(pos3D, pos))
            return;

        int weaponID = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
        auto weaponName = ItemDefinitionIndexToString(weaponID);


		g_Render->Text(pos.x, pos.y, Color(255, 255, 255, 255), g_Render->font.DroppedGuns, weaponName);
    }
}

void visuals::DrawBombPlanted(C_BaseEntity* entity, C_BaseEntity* local)
{
	BombCarrier = nullptr;

	float damage;
	char bombdamagestringdead[24];
	char bombdamagestringalive[24];

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (g_Globals->interval_per_tick * local->GetTickBase());
	if (g_Render->WorldToScreen(vOrig, vScreen))
	{
		if (local->IsAlive())
		{
			float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a*exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "ur dead nigga");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health left: %.0f", local->GetHealth() - damage);
            if (lifetime > -2.f)
            {
                if (damage >= local->GetHealth())
                {
                    g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), g_Render->font.Defuse, bombdamagestringdead);
                }
                else if (local->GetHealth() > damage)
                {
                    g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), g_Render->font.Defuse, bombdamagestringalive);
                }
            }
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Bomb: %.1f", lifetime);
			g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.ESP, buffer);
		}

	}

	g_Engine->GetScreenSize(width, height);
	int halfX = width / 2;
	int halfY = height / 2;


	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * g_Globals->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.Defuse, defuseTimeString);
			}
			else
			{
				g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.Defuse, "No time! Get OUT!");
			}
		}
	}
}

void visuals::DrawBomb(C_BaseEntity* entity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)entity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	bool adopted = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		C_BaseEntity* pParentEnt = (g_EntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
	if (g_Options.Visuals.C4World)
	{
		if (adopted)
		{
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(112, 20, 20, 255), g_Render->font.ESP, "Bomb");
			}
		}
	}


}


bool visuals::GetBox(C_BaseEntity* entity, visuals::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = entity->GetOrigin();
	min = entity->collisionProperty()->GetMins() + vOrigin;
	max = entity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!g_Render->WorldToScreen(points[3], flb) || !g_Render->WorldToScreen(points[5], brt)
		|| !g_Render->WorldToScreen(points[0], blb) || !g_Render->WorldToScreen(points[4], frt)
		|| !g_Render->WorldToScreen(points[2], frb) || !g_Render->WorldToScreen(points[1], brb)
		|| !g_Render->WorldToScreen(points[6], blt) || !g_Render->WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = (int)left;
	result.y = (int)top;
	result.w = int(right - left);
	result.h = int(bottom - top);
	result.gay = (int)top;

	return true;
}


void visuals::DrawBox(visuals::ESPBox size, Color color)
{
	g_Render->Outline(size.x, size.y, size.w, size.h, color);
	g_Render->Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, color);
	g_Render->Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, color);
}

void visuals::BoxAndText(C_BaseEntity* entity, std::string text)
{
	ESPBox Box;
	std::vector<std::string> Info;
	if (GetBox(entity, Box))
	{
		Info.push_back(text);
		if (g_Options.Visuals.GrenadeESP)
		{
			DrawBox(Box, Color(255, 255, 255, 255));
			int i = 0;
			for (auto kek : Info)
			{
				g_Render->Text(Box.x + 1, Box.y + 1, Color(255, 255, 255, 255), g_Render->font.ESP, kek.c_str());
				i++;
			}
		}
	}
}
void visuals::DrawThrowable(C_BaseEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_ModelInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	std::string nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	g_ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "HE";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}

	BoxAndText(throwable, nadeName);
}

void visuals::DLight(C_BaseEntity *local, C_BaseEntity* entity)
{
    player_info_t pinfo;
    if (local && entity && entity != local)
    {
        if (g_Engine->GetPlayerInfo(entity->GetIndex(), &pinfo) && entity->IsAlive() && !entity->IsDormant())
        {
            if (local->GetTeamNum() != entity->GetTeamNum())
            {
                dlight_t* pElight = g_Dlight->CL_AllocElight(entity->GetIndex());
                pElight->origin = entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
                pElight->radius = 300.0f;
                pElight->color.b = 36;
                pElight->color.g = 224;
                pElight->color.r = 41;
                pElight->die = g_Globals->curtime + 0.05f;
                pElight->decay = pElight->radius / 5.0f;
                pElight->key = entity->GetIndex();

                dlight_t* pDlight = g_Dlight->CL_AllocDlight(entity->GetIndex());
                pDlight->origin = entity->GetOrigin();
                pDlight->radius = 300.0f;
                pDlight->color.b = 36;
                pDlight->color.g = 224;
                pDlight->color.r = 41;
                pDlight->die = g_Globals->curtime + 0.05f;
                pDlight->decay = pDlight->radius; // / 5.0f;
                pDlight->key = entity->GetIndex();
            }
        }
    }
}


void visuals::NightMode()
{
	if (g_Options.Misc.nightMode)
	{
		if (!done)
		{


			static auto sv_skyname = g_CVar->FindVar("sv_skyname");
			static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
			r_DrawSpecificStaticProp->SetValue(1);
			sv_skyname->SetValue("sky_csgo_night02");

			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(0.10, 0.10, 0.10);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}

				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				done = true;
			}

		}
	}
	else
	{
		if (done)
		{
			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "StaticProp"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
			}
			done = false;
		}
	}
}

void visuals::SpecList(C_BaseEntity *local)
{
    RECT scrn = g_Render->GetViewport();
    int kapi = 0;

    if (local)
    {
        for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
        {
            // Get the entity
            C_BaseEntity *pEntity = g_EntityList->GetClientEntity(i);
            player_info_t pinfo;
            if (pEntity && pEntity != local)
            {
                if (g_Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
                {
                    HANDLE obs = pEntity->GetObserverTargetHandle();
                    if (obs)
                    {
                        C_BaseEntity *pTarget = g_EntityList->GetClientEntityFromHandle(obs);
                        player_info_t pinfo2;
                        if (pTarget && pTarget->GetIndex() == local->GetIndex())
                        {
                            if (g_Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
                            {
								
								
								pinfo.name[16] = 0;
								strcat(pinfo.name, "...");
								puts(pinfo.name);
								char buf[255]; sprintf(buf, "%s <= Spec => %s", pinfo.name, pinfo2.name);
								RECT TextSize = g_Render->GetTextSize(g_Render->font.ESP, buf);

								g_Render->Clear(scrn.right - 260, (scrn.bottom / 2) + (5 * kapi), 260, 5, Color(0, 0, 0, 140));
								g_Render->Text(scrn.right - TextSize.right - 3, (scrn.bottom / 2) + (16 * kapi), pTarget->GetIndex() == local->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 255), g_Render->font.ESP, buf);
								kapi++;
                            }
                        }
                    }
                }
            }
        }
    }

	g_Render->Outline(scrn.right - 261, (scrn.bottom / 2) - 1, 262, (16 * kapi) + 2, Color(21, 21, 21, 255));
	g_Render->Outline(scrn.right - 260, (scrn.bottom / 2), 260, (16 * kapi), Color(21, 21, 21, 21));
}


