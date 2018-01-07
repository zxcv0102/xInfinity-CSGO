#pragma once

extern void RenderInterface();

//Color Feature for Color Changer
struct ColorP
{
public:
	const char* Name;
	float* Ccolor;

	ColorP(const char* name, float* color)
	{
		this->Name = name;
		this->Ccolor = color;
	}
};

struct Variables
{
	Variables()
	{

	}


	struct
	{
		bool MainSwitch;
        bool backtrack;
		bool backtrackl;
		struct
		{
			bool 	Enabled;
			bool    AutoPistol;
			bool	Resolver;
            
		} Aimbot;


			int MainKey = 1;
            float MainSmooth;
			float Mainfov;
            float main_random_smooth;
            float main_recoil_min;
            float main_recoil_max;
            float main_randomized_angle;

			int PistolKey = 6;
			float Pistolfov;
			float PistolSmooth;
            float pistol_random_smooth;
            float pistol_recoil_min;
            float pistol_recoil_max;
            float pistol_randomized_angle;

			int SniperKey = 6;
			float Sniperfov;
			float SniperSmooth;
            float sniper_random_smooth;
            float sniper_recoil_min;
            float sniper_recoil_max;
            float sniper_randomized_angle;


		struct
		{
			bool	Enabled;
			float Delay;
			int Key = 6;
			float hitchance;
			struct
			{
				bool Head;
				bool Arms;
				bool Chest;
				bool Stomach;
				bool Legs;
			} Filter;

		} Triggerbot;

	} Legitbot;

	struct
	{
		bool Enabled;

		int Box;
		bool HP;
		bool Name;
		bool Skeleton;
		bool Weapon;
		bool Snaplines;

		//DrawChams Options
		int Chams;
		int Hands;
		bool ChamsEnabled;

		//DrawOptions For Players
		bool Money;
		bool IsHasDefuser;
		bool IsScoped;
		bool IsDefusing;
		bool IsFlashed;

		bool AimSpot;
		bool DLight;
		bool GrenadeESP;
		bool C4;

		bool NoVisualRecoil;
		int Weapons;
		float FOVChanger;
		float viewmodelChanger;
		bool NoFlash;
		bool NoSmoke;
		bool ThirdPerson;

		bool C4World;
		bool WeaponsWorld;


		bool noscopeborder;
		bool GrenadePrediction;


		//Colors for Chams
		float chamsPlyrTVis[3] = { 0.9f,0.4f,0.f };
		float chamsPlyrTInvis[3] = { 0.9f, 0.0f, 0.f };
		float chamsPlyrCTVis[3] = { 0.f,0.4f,0.9f };
		float chamsPlyrCTInvis[3] = { 0.f,0.0f,0.9f };

		float espVisT[3] = { 0.9f, 0.2f, 0.2f };
		float espInvisT[3] = { 0.7f, 0.1f, 0.1f };
		float espVisCT[3] = { 0.9f, 0.2f, 0.2f };
		float espInvisCT[3] = { 0.7f, 0.1f, 0.1f };

		//Colors for Hands
		float chamsT[3] = { 0.9f, 0.2f, 0.2f };
		float chamsCT[3] = { 0.9f, 0.2f, 0.2f };

		//Crosshairs
		int CrosshairOptions;

		struct
		{
			bool Players;
			bool EnemyOnly;
			bool VisibleOnly;
			bool pLocalPlayer;

		} Filter;

	} Visuals;

	struct
	{
        bool silentstealer;
		bool 	Bhop;
		bool  Watermark;
		bool 	AutoStrafe;
		bool ServerRankRevealAll;
		int  NameSpammer;
		bool NameChangerFix;
		bool NoName;
		int	 ChatSpamMode;
		bool ClantagChanger;
		int  ClanTagSpeed;
        bool syncclantag;
		bool SpecList;
        bool namespam;
        int spammer;
		bool Spam;
		bool AutoAccept;
		bool Hitmarker;
		bool SpoofConfirmation = false;
		bool animatedclantag = false;
		int HitMarkerColor;

		//World
		bool ChromeWorld;
		bool nightMode;

		//Disable Post-Processing
		bool disablepost;

	} Misc;
	struct
	{
		bool Enabled;
		int Knife;
        int gloves;
		int KnifeSkin;
		int AK47Skin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		int SCAR20Skin;
		int P90Skin;
		int UMP45Skin;
		int GlockSkin;
		int USPSkin;
		int DeagleSkin;
		int tec9Skin;
		int P2000Skin;
		int P250Skin;
	} Skinchanger;

	struct
	{
		bool	Opened = false;
		int 	Key;
		bool	Legitbot = false;
		bool	Visual = false;
		bool	Misc = false;
		int		ConfigFile = 0;
		int		Theme = 0;
		int     panicbutton;
	} Menu;

};



extern Variables g_Options;