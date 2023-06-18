﻿#pragma once
#include "Border.h"
#include "Level.h"
#include "Platform.h"
#include "Ball_Set.h"
#include "Monster_Set.h"
#include "Info_Panel.h"

//-------------------------------------------------------------------------------------------------------------------------
enum class EGame_State: unsigned char
{
   Test_Ball,

   Play_Level,
   Lost_Ball,
   Restart_Level
};
//-------------------------------------------------------------------------------------------------------------------------
enum class EKey_Type: unsigned char
{
   Left,
   Right,
   Space
};
//-------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
   AsEngine();

   void Init_Engine(HWND hwnd);
   void Draw_Frame(HDC hdc, RECT &paint_area);
   int On_Key(EKey_Type key_type, bool key_down);
   int On_Timer();

   const int Timer_ID;

private:
   void Restart_Level();
   void Play_Level();
   void Advance_Movers();
   void Act();
   void On_Falling_Letter(AFalling_Letter *falling_letter);
   void Add_Next_Module(int &index, AGame_Object *object);

   EGame_State Game_State;
   double Rest_Distance;
   int Life_Count;

   AsLevel Level;
   AsPlatform Platform;
   AsBorder Border;
   ABall_Set Ball_Set;
   AsLaser_Beam_Set Laser_Beam_Set;
   AsMonster_Set Monster_Set;
   AsInfo_Panel Info_Panel;

   //AMover *Movers[AsConfig::Max_Movers_Count];
   //AGraphics_Object *Modules[AsConfig::Max_Modules_Count];

   AGame_Object *Modules[AsConfig::Max_Modules_Count];
};
//-------------------------------------------------------------------------------------------------------------------------
