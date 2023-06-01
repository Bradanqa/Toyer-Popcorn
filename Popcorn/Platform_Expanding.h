#pragma once

#include "Platform_State.h"

//-------------------------------------------------------------------------------------------------------------------------
class AsPlatform_Expanding
{
public:
   ~AsPlatform_Expanding();
   AsPlatform_Expanding(AsPlatform_State &platform_state);

   void Init(AColor &platform_inner_color, AColor &platform_circle_color, AColor &highlight_color);
   bool Act(double &x_pos, EPlatform_State &next_state, bool &correct_pos);
   void Draw_State(HDC hdc, double x_pos);
   void Draw_Circle_Highlight(HDC hdc, int x, int y);
   void Reset();

   double Expanding_Platform_Width;

private:
   void Draw_Expanding_Platform_Ball(HDC hdc, bool is_left, double x);
   void Draw_Expanding_Truss(HDC hdc, RECT &inner_rect, bool is_left);

   AColor *Truss_Color, *Inner_Color, *Platform_Circle_Color, *Highlight_Color;
   AsPlatform_State *Platform_State;

   static const double Max_Expanding_Platform_Width;
   static const double Min_Expanding_Platform_Width;
   static const double Expanding_Platform_Width_Step;

};
//-------------------------------------------------------------------------------------------------------------------------