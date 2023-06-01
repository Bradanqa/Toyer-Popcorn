#pragma once

#include "Platform_State.h"
#include "Laser_Beam_Set.h"


//-------------------------------------------------------------------------------------------------------------------------
class AsPlatform_Laser
{
public:
   ~AsPlatform_Laser();
   AsPlatform_Laser(AsPlatform_State &platform_state);

   void Init(ALaser_Beam_Set *laser_beam_set, AColor &platform_inner_color, AColor &platform_circle_color, AColor &gun_color);
   bool Act(EPlatform_State &next_state, double x_pos);
   void Draw_State(HDC hdc, double x_pos, RECT &platform_rect);
   void Reset();
   void Fire(bool fire_on);

private:
   void Draw_Laser_Inner_Part(HDC hdc, double x_pos);
   void Draw_Laser_Wing(HDC hdc, double x_pos,bool is_left);
   void Draw_Laser_Leg(HDC hdc, double x_pos, bool is_left);
   void Draw_Laser_Head(HDC hdc, double x_pos);
   void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
   int Get_Expanding_Value(double start, double end, double ratio);
   double Get_Gun_Pos(double platform_x_pos, bool is_left);

   bool Enable_Laser_Firing;
   int Laser_Transformation_Step;
   int Last_Laser_Shot_Tick;

   AColor *Inner_Color, *Platform_Circle_Color;
   AColor *Gun_Color;
   AsPlatform_State *Platform_State;

   ALaser_Beam_Set *Laser_Beam_Set;

   static const int Max_Laser_Transformation_Step = 20;
   static const int Laser_Shot_Timeout = AsConfig::FPS / 2; // 2 раза в секунду
};
//-------------------------------------------------------------------------------------------------------------------------