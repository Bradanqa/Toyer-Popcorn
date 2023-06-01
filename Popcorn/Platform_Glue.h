#pragma once

#include "Ball_Set.h"
#include "Platform_State.h"


//-------------------------------------------------------------------------------------------------------------------------
class AsPlatform_Glue 
{
public:
   AsPlatform_Glue(AsPlatform_State &platform_state);

   bool Act(ABall_Set *ball_set, EPlatform_State &next_state);
   void Draw_State(HDC hdc, double x_pos);
   void Reset();

private:
   void Draw_Glue_Spot(HDC hdc, double x_pos, int x_offset, int width, int height);

   double Glue_Spot_Height_Ratio;
   AsPlatform_State *Platform_State;

   static const double Max_Glue_Spot_Height_Ratio;
   static const double Min_Glue_Spot_Height_Ratio;
   static const double Glue_Spot_Height_Ratio_Step;
};
//-------------------------------------------------------------------------------------------------------------------------