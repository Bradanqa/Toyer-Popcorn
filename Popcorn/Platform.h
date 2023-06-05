#pragma once
#include "Falling_Letter.h"
#include "Platform_Glue.h"
#include "Platform_Expanding.h"
#include "Platform_Laser.h"

//-------------------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker, public AGame_Object
{
public:
   ~AsPlatform();
   AsPlatform();

   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

   virtual void Begin_Movement();
   virtual void Finish_Movement();
   virtual void Advance(double max_speed);
   virtual double Get_Speed();

   virtual void Act();
   virtual void Clear(HDC hdc, RECT& paint_area);
   virtual void Draw(HDC hdc, RECT& paint_area);
   virtual bool Is_Finished();

   void Init(ABall_Set* ball_set, AsLaser_Beam_Set *laser_beam_set);
   EPlatform_State Get_State();
   void Set_State(EPlatform_State new_state);
   void Set_State(EPlatform_Substate_Regular new_regular_state);
   bool Has_State(EPlatform_Substate_Regular regular_state);
   void Redraw_Platform();
   void Move(bool to_left, bool key_down);
   void On_Space_Key(bool key_down);
   bool Hit_By(AFalling_Letter *falling_letter);
   double Get_Middle_Pos();

   double Platform_X_Pos;

private:
   bool Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation& transformation_state);
   void Act_For_Meltdown_State();
   void Act_For_Rolling_State();
   void Draw_Normal_State(HDC hdc, RECT &paint_area);
   void Draw_Meltdown(HDC hdc, RECT &paint_area);
   void Draw_Rolling_State(HDC hdc, RECT &paint_area);
   void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
   void Get_Normal_Platform_Image(HDC hdc);
   bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall *ball);
   bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor** color, int& stroke_len);
   double Get_Current_Width();
   bool Correct_Platform_Pos();

   AsPlatform_State Platform_State;

   bool Left_Key_Down, Right_Key_Down;

   double Speed;
   int Meltdown_Y_Pos;
   int Inner_width;
   int Rolling_Step;
   int Last_Redraw_Timer_Tick;
   int Meltdown_Platform_Y_Pos[AsConfig::Platform_Normal_Width * AsConfig::Global_Scale];
   ABall_Set *Ball_Set;

   AsPlatform_Glue Platform_Glue;
   AsPlatform_Expanding Platform_Expanding;
   AsPlatform_Laser Platform_Laser;

   int Normal_Platform_Image_Width;
   int Normal_Platform_Image_Height;
   int *Normal_Platform_Image; // Пиксели платформы внутри окна

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color;

   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int Platform_X_Step = 6;
};