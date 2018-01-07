#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);


void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static bool DontDraw = false;
	static auto ofunc = hooks::modelrender.get_original<dme_t>(21);

	//========================CreateMaterial2=================================//

	static IMaterial* CoveredLit = CreateMaterial2(true);
	static IMaterial* OpenLit = CreateMaterial2(false);
	static IMaterial* CoveredFlat = CreateMaterial2(true, false);
	static IMaterial* OpenFlat = CreateMaterial2(false, false);

	//========================CreateMaterial2=================================//

	if (g_Options.Visuals.Enabled && g_Options.Visuals.ChamsEnabled)
	{

		static IMaterial* ignorez = CreateMaterial("VertexLitGeneric", "vgui/white_additive", true, true, true, true, true);
		static IMaterial* notignorez = CreateMaterial("VertexLitGeneric", "vgui/white_additive", false, true, true, true, true);

		DontDraw = false;


		const char * ModelName = g_ModelInfo->GetModelName((model_t*)pInfo.pModel);
		C_BaseEntity* pModelEntity = (C_BaseEntity*)g_EntityList->GetClientEntity(pInfo.entity_index);
		C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

		int ChamsStyle = g_Options.Visuals.Chams;
		int HandsStyle = g_Options.Visuals.Hands;
		if ((ChamsStyle != 0 && g_Options.Visuals.Filter.Players && strstr(ModelName, "models/player")))
		{
			if (pModelEntity && pLocal)
			{
				if (!g_Options.Visuals.Filter.EnemyOnly ||
					pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
				{
					pModelEntity = g_EntityList->GetClientEntity(pInfo.entity_index);
					if (pModelEntity)
					{

						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
						{
							Color col_vis;
							Color col_invis;

							bool TeamCheck = pModelEntity->GetTeamNum() == pLocal->GetTeamNum(); //check for team 


							float alpha = 1.f;
							if (pModelEntity->HasGunGameImmunity())
								alpha = 0, 5.f;


							if (TeamCheck)
							{

								col_vis = Color(g_Options.Visuals.chamsPlyrTVis[0] * 255, g_Options.Visuals.chamsPlyrTVis[1] * 255, g_Options.Visuals.chamsPlyrTVis[2] * 255);
							}
							else
							{
								col_vis = Color(g_Options.Visuals.chamsPlyrCTVis[0] * 255, g_Options.Visuals.chamsPlyrCTVis[1] * 255, g_Options.Visuals.chamsPlyrCTVis[2] * 255);
							}

							if (TeamCheck)
							{
								col_invis = Color(g_Options.Visuals.chamsPlyrTInvis[0] * 255, g_Options.Visuals.chamsPlyrTInvis[1] * 255, g_Options.Visuals.chamsPlyrTInvis[2] * 255);
							}
							else
							{
								col_invis = Color(g_Options.Visuals.chamsPlyrCTInvis[0] * 255, g_Options.Visuals.chamsPlyrCTInvis[1] * 255, g_Options.Visuals.chamsPlyrCTInvis[2] * 255);
							}



							if (ChamsStyle == 1) //Chams Through Walls
							{

								g_RenderView->SetColorModulation(col_vis.Base());
								g_RenderView->SetBlend(alpha);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
								g_ModelRender->ForcedMaterialOverride(notignorez);
								g_ModelRender->ForcedMaterialOverride(ignorez);
								ForceMaterial(Color(col_vis), ignorez);

								g_RenderView->SetColorModulation(col_vis.Base());
								g_RenderView->SetBlend(alpha);

							}

							if (ChamsStyle == 2) //Visible only
							{
								g_RenderView->SetColorModulation(col_invis.Base());
								g_ModelRender->ForcedMaterialOverride(notignorez);
							}

						}
					}
				}
			}
		}
		else if (HandsStyle != 0 && strstr(ModelName, "arms"))
		{
			if (HandsStyle == 1) //No hands
			{
				bool TeamCheck = pLocal->GetTeamNum();
				if (pLocal->IsAlive())
				{
					if (TeamCheck)
					{
						DontDraw = true;
					}
					else
					{
						DontDraw = true;
					}
				}
			
			}
			if (HandsStyle == 2) //Transparent
			{
				bool TeamCheck = pLocal->GetTeamNum();
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						if (TeamCheck)
						{
							g_RenderView->SetBlend(0.3);
						}
						else
						{
							g_RenderView->SetBlend(0.3);
						}
					}
				}
			}

			if (HandsStyle == 3) //WireFrame
			{
				bool TeamCheck = pLocal->GetTeamNum();
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						if (TeamCheck)
						{
							IMaterial* WireFrameHands = g_MaterialSystem->FindMaterial(ModelName, "Model textures");
							WireFrameHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
							g_ModelRender->ForcedMaterialOverride(WireFrameHands);
						}
						else
						{
							IMaterial* WireFrameHands = g_MaterialSystem->FindMaterial(ModelName, "Model textures");
							WireFrameHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
							g_ModelRender->ForcedMaterialOverride(WireFrameHands);
						}
					}
				}
			}



			if (HandsStyle == 4) //ChamsHands
			{
				IMaterial *covered = HandsStyle == 4 ? CoveredLit : CoveredFlat;
				IMaterial *open = HandsStyle == 4 ? OpenLit : OpenFlat;
				Color colorchamshands;
				Color colorchamshands2;

				bool TeamCheck = pLocal->GetTeamNum();

				if (pLocal)
				{
					if (pLocal->IsAlive())
					{

						if (TeamCheck)
						{
							colorchamshands = Color(g_Options.Visuals.chamsT[0] * 255, g_Options.Visuals.chamsT[1] * 255, g_Options.Visuals.chamsT[2] * 255, g_Options.Visuals.chamsT[3] * 255);
							ForceMaterial(colorchamshands, covered);
							covered->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							open->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						}
						else
						{
							colorchamshands2 = Color(g_Options.Visuals.chamsCT[0] * 255, g_Options.Visuals.chamsCT[1] * 255, g_Options.Visuals.chamsCT[2] * 255, g_Options.Visuals.chamsCT[3] * 255);
							ForceMaterial(colorchamshands2, covered);
							covered->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							open->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						}

					}
				}

			}
		}
	}
	if (!DontDraw)
	{
		ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		g_ModelRender->ForcedMaterialOverride(NULL);
	}

}