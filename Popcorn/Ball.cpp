﻿#include "Ball.h"


//ABall
AHit_Checker_List ABall::Hit_Checker_List;
//-------------------------------------------------------------------------------------------------------------------------
ABall::ABall()
   : Ball_State(EBall_State::Disabled), Prev_Ball_State(EBall_State::Disabled), Release_Timer_Tick(0), Center_X_Pos(0.0), Center_Y_Pos(0.0), Ball_Speed(0.0), Prev_Ball_Speed(0.0),
   Ball_Direction(0.0), Prev_Ball_Direction(0.0), Testing_Is_Active(false), Test_Iteration(0), Ball_Rect{}, Prev_Ball_Rect{}
{

}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Act()
{
   
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Clear(HDC hdc, RECT& paint_area)
{
   RECT intersection_rect;

   if (Ball_State == EBall_State::Disabled)
      return;

   if ((Ball_State == EBall_State::Teleporting || Ball_State == EBall_State::Lost) && Ball_State == Prev_Ball_State)
      return;

   // 1. Очищаем фон.
   if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
      AsTools::Ellipse(hdc, Prev_Ball_Rect, AsConfig::BG_Color);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area)
{
   RECT intersection_rect;

   if (Ball_State == EBall_State::Disabled)
      return;

   if ((Ball_State == EBall_State::Teleporting || Ball_State == EBall_State::Lost) && Ball_State == Prev_Ball_State)
      return;

   switch (Ball_State)
   {
   case EBall_State::On_Parachute:
      Draw_Parachute(hdc, paint_area);
      break;

   case EBall_State::Off_Parachute:
      Clear_Parachute(hdc);
      Set_State(EBall_State::Normal, Center_X_Pos, Center_Y_Pos);
      break;

   case EBall_State::Lost:
      if (Prev_Ball_State == EBall_State::On_Parachute)
         Clear_Parachute(hdc);

   case EBall_State::Teleporting:
      return;
   }

   // 2. Рисуем шарик.
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      AsTools::Ellipse(hdc, Ball_Rect, AsConfig::White_Color);
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall::Is_Finished()
{
   return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Begin_Movement()
{
   Prev_Ball_Rect = Ball_Rect;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Finish_Movement()
{
   if (Ball_State == EBall_State::Disabled || Ball_State == EBall_State::Lost || Ball_State == EBall_State::On_Platform)
      return;

   Redraw_Ball();

   if (Ball_State == EBall_State::On_Parachute)
   {
      Prev_Parachute_Rect = Parachute_Rect;
      Parachute_Rect.bottom = Ball_Rect.bottom;
      Parachute_Rect.top = Parachute_Rect.bottom - Parachute_Size * AsConfig::Global_Scale;

      Redraw_Parachute();
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Advance(double max_speed)
{
   int prev_hits_count = 0;
   const int max_hits_count = 8;
   bool got_hit = true;
   double next_x_pos, next_y_pos;
   double next_step;

   if (Ball_State == EBall_State::Disabled || Ball_State == EBall_State::Lost || Ball_State == EBall_State::On_Platform || Ball_State == EBall_State::Teleporting)
      return;

   next_step = Ball_Speed / max_speed * AsConfig::Moving_Step_Size;

   while (got_hit)
   {
      got_hit = false;
      next_x_pos = Center_X_Pos + next_step * cos(Ball_Direction);
      next_y_pos = Center_Y_Pos - next_step * sin(Ball_Direction);

      got_hit = Hit_Checker_List.Check_Hit(next_x_pos, next_y_pos, this);

      if (got_hit)
      {
         ++prev_hits_count;

         if (prev_hits_count >= max_hits_count)
         {
            Ball_Direction += AsConfig::Min_Ball_Angle;
            prev_hits_count = 0;
         }
      }
      else
      {//Мячик продолжит движение, если не взаимодействовал я другими объектами
         
         Center_X_Pos = next_x_pos;
         Center_Y_Pos = next_y_pos;

         if (Testing_Is_Active)
            Rest_Test_Distance -= next_step;
      }

      if (Ball_State == EBall_State::On_Platform)
         break;
   }
}
//-------------------------------------------------------------------------------------------------------------------------
double ABall::Get_Speed()
{
   return Ball_Speed;
}
//-------------------------------------------------------------------------------------------------------------------------
double ABall::Get_Direction()
{
   return Ball_Direction;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Set_Direction(double new_derection)
{
   const double pi_2 = 2.0 * M_PI;

   // Переводим угол в диапазон от 0 до Пи / 2
   while (new_derection > pi_2)
      new_derection -= pi_2;

   while (new_derection < 0)
      new_derection += pi_2;

   // Не позволяем мячику приближаться близко к горизонтальному движеию
   // Left
   if (new_derection < AsConfig::Min_Ball_Angle)
      new_derection = AsConfig::Min_Ball_Angle;

   if (new_derection > pi_2 - AsConfig::Min_Ball_Angle)
      new_derection = pi_2 - AsConfig::Min_Ball_Angle;

   // Right
   if (new_derection > M_PI - AsConfig::Min_Ball_Angle && new_derection < M_PI)
      new_derection = M_PI - AsConfig::Min_Ball_Angle;

   if (new_derection >= M_PI && new_derection < M_PI + AsConfig::Min_Ball_Angle)
      new_derection = M_PI + AsConfig::Min_Ball_Angle;

   Ball_Direction = new_derection;
}
//-------------------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
   return Ball_State;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, double x_pos, double y_pos)
{
   switch (new_state)
   {
   case EBall_State::Disabled:
      Ball_Speed = 0.0;
      //Rest_Distance = 0.0;
      break;

   case EBall_State::Normal:
      Center_X_Pos = (double)x_pos;
      Center_Y_Pos = y_pos;
      Ball_Speed = AsConfig::Normal_Ball_Speed;
      //Rest_Distance = 0.0;
      Ball_Direction = M_PI_4;
      Redraw_Ball();
      break;

   case EBall_State::Lost:
      if (!(Ball_State == EBall_State::Normal || Ball_State == EBall_State::On_Parachute))
         AsConfig::Throw();

      Ball_Speed = 0.0;
      Redraw_Ball();
      Redraw_Parachute();
      break;

   case EBall_State::On_Platform:
      Center_X_Pos = (double)x_pos;
      Center_Y_Pos = y_pos;
      Prev_Ball_Speed = Ball_Speed;
      Ball_Speed = 0.0;
      Prev_Ball_Direction = Ball_Direction;
      //Rest_Distance = 0.0;
      Release_Timer_Tick = AsConfig::Current_Timer_Tick + On_Platform_Timeout;
      Redraw_Ball();
      break;

   case EBall_State::On_Parachute:
      AsConfig::Throw(); // 
      break;

   case EBall_State::Off_Parachute:
      if(Ball_State != EBall_State::On_Parachute)
         AsConfig::Throw();

      Ball_Speed = 0.0;
      //Rest_Distance = 0.0;
      Redraw_Ball();
      Redraw_Parachute();
      break;

   case EBall_State::Teleporting:
      if (!(Ball_State == EBall_State::Normal || Ball_State == EBall_State::On_Parachute || Ball_State == EBall_State::Teleporting))
         AsConfig::Throw();
      Center_X_Pos = x_pos;
      Center_Y_Pos = y_pos;
      Ball_Speed = 0.0;
      //Rest_Distance = 0.0;
      Redraw_Ball();

      if(Ball_State == EBall_State::On_Parachute)
         Redraw_Parachute();
      break;

   default:
      AsConfig::Throw();
   }

   Prev_Ball_State = Ball_State;
   Ball_State = new_state;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Reflect(bool from_horizontal)
{
   if(from_horizontal)
      Set_Direction(-Ball_Direction);
   else
      Set_Direction(M_PI - Ball_Direction);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Draw_Teleporting(HDC hdc, int step)
{
   int top_y = Ball_Rect.top + step / 2;
   int low_y = Ball_Rect.bottom - step / 2 - 1;

   if (top_y >= low_y)
      return;

   AsConfig::White_Color.Select(hdc);
   Ellipse(hdc, Ball_Rect.left, top_y, Ball_Rect.right - 1, low_y);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Set_On_Parachute(int brick_x, int brick_y)
{
   int cell_x = AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width;
   int cell_y = AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height;
   int scale = AsConfig::Global_Scale;

   Ball_Direction = M_PI + M_PI_2;
   Ball_Speed = 1.0;
   Ball_State = EBall_State::On_Parachute;

   Parachute_Rect.left = cell_x * scale;
   Parachute_Rect.top = cell_y * scale;
   Parachute_Rect.right = Parachute_Rect.left + Parachute_Size * scale;
   Parachute_Rect.bottom = Parachute_Rect.top + Parachute_Size * scale;

   Prev_Parachute_Rect = Parachute_Rect;

   Center_X_Pos = (double)(cell_x + AsConfig::Cell_Width / 2) - 1 / AsConfig::D_Global_Scale;
   Center_Y_Pos = (double)(cell_y + Parachute_Size) - AsConfig::Ball_Radius * 2.0;;

   Redraw_Parachute();
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Get_Center(double& x_pos, double& y_pos)
{
   x_pos = Center_X_Pos;
   y_pos = Center_Y_Pos;
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Up()
{
   if (Ball_Direction >= 0.0 && Ball_Direction < M_PI)
      return true;
   else
      return false;
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Left()
{
   if (Ball_Direction > M_PI_2 && Ball_Direction < M_PI + M_PI_2)
      return true;
   else
      return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Set_Speed(double new_speed)
{
   Ball_Speed = new_speed;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Set_For_Test()
{
   Testing_Is_Active = true;

   Rest_Test_Distance = 40.0;

   Set_State(EBall_State::Normal, 90 + Test_Iteration, 90);
   Ball_Direction = M_PI - M_PI_4;

   ++Test_Iteration;
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall::Is_Test_Finished()
{
   if (Testing_Is_Active)
   {
      if (Rest_Test_Distance <= 0)
      {
         Testing_Is_Active = false;
         Set_State(EBall_State::Lost);
         return true;
      }
   }

   return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Forced_Advance(double direction, double speed, double max_speed)
{// Принудительное смещение шарика

   EBall_State prev_ball_state = Ball_State;
   double prev_direction = Ball_Direction;
   double prev_speed = Ball_Speed;

   Ball_State = EBall_State::Normal;
   Ball_Direction = direction;
   Ball_Speed = speed;

   Advance(max_speed);

   Ball_State = prev_ball_state;
   Ball_Direction = prev_direction;
   Ball_Speed = prev_speed;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Release()
{
   Set_State(EBall_State::Normal, Center_X_Pos, Center_Y_Pos);
   Ball_Speed = Prev_Ball_Speed;

   if (Ball_Speed < AsConfig::Normal_Ball_Speed)
      Ball_Speed = AsConfig::Normal_Ball_Speed;

   Ball_Direction = Prev_Ball_Direction;
   Release_Timer_Tick = 0;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
   Ball_Rect.left = (int)((Center_X_Pos - AsConfig::Ball_Radius) * AsConfig::D_Global_Scale);
   Ball_Rect.top = (int)((Center_Y_Pos - AsConfig::Ball_Radius) * AsConfig::D_Global_Scale);
   Ball_Rect.right = (int)((Center_X_Pos + AsConfig::Ball_Radius) * AsConfig::D_Global_Scale);
   Ball_Rect.bottom = (int)((Center_Y_Pos + AsConfig::Ball_Radius) * AsConfig::D_Global_Scale);

   AsTools::Invalidate_Rect(Prev_Ball_Rect);
   AsTools::Invalidate_Rect(Ball_Rect);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Draw_Parachute(HDC hdc, RECT &paint_area)
{
   const int scale = AsConfig::Global_Scale;
   int dome_height = (Parachute_Rect.bottom - Parachute_Rect.top) / 2;
   int arc_height = 4 * scale;
   int arc_x;
   int line_y, ball_center_x, ball_center_y;

   RECT intersection_rect, sub_arc, other_arc;
   
   if (! IntersectRect(&intersection_rect, &paint_area, &Parachute_Rect))
      return;

   // Очищаем фон
   Clear_Parachute(hdc);

   // 1. Купол
   AsConfig::Blue_Color.Select(hdc);
   Chord(hdc, Parachute_Rect.left, Parachute_Rect.top, Parachute_Rect.right - 1, Parachute_Rect.bottom - 1,
      Parachute_Rect.right, Parachute_Rect.top + dome_height - 1, Parachute_Rect.left, Parachute_Rect.top + dome_height - 1);

   // 2. Арки
   AsConfig::BG_Color.Select(hdc);

   arc_x = Parachute_Rect.left + 1;

   // 2.1 левая
   sub_arc.left = arc_x;
   sub_arc.top = Parachute_Rect.top + dome_height - arc_height / 2;
   sub_arc.right = sub_arc.left + 3 * scale;
   sub_arc.bottom = sub_arc.top + 4 * scale;

   AsTools::Ellipse(hdc, sub_arc, AsConfig::BG_Color);

   // 2.1 средняя
   other_arc = sub_arc;

   other_arc.left = arc_x + 3 * scale + 1;
   other_arc.right = arc_x + 11 * scale;

   AsTools::Ellipse(hdc, other_arc, AsConfig::BG_Color);

   // 2.1 правая
   other_arc = sub_arc;

   other_arc.left = arc_x + 11 * scale + 1;
   other_arc.right = arc_x + 14 * scale + 1;

   AsTools::Ellipse(hdc, other_arc, AsConfig::BG_Color);

   // 3. Стропы
   line_y = Parachute_Rect.top + dome_height;
   ball_center_x = (Parachute_Rect.left + Parachute_Rect.right) / 2;
   ball_center_y = Parachute_Rect.bottom - 2 * scale;

   AsConfig::White_Color.Select(hdc);

   MoveToEx(hdc, Parachute_Rect.left, line_y, 0);
   LineTo(hdc, ball_center_x, ball_center_y);

   MoveToEx(hdc, Parachute_Rect.left + 3 * scale + 1, line_y, 0);
   LineTo(hdc, ball_center_x, ball_center_y);

   MoveToEx(hdc, Parachute_Rect.right - 4 * scale + 1, line_y, 0);
   LineTo(hdc, ball_center_x, ball_center_y);

   MoveToEx(hdc, Parachute_Rect.right, line_y - 1, 0);
   LineTo(hdc, ball_center_x, ball_center_y);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Parachute()
{
   AsTools::Invalidate_Rect(Prev_Parachute_Rect);
   AsTools::Invalidate_Rect(Parachute_Rect);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall::Clear_Parachute(HDC hdc)
{
   AsConfig::BG_Color.Select(hdc);
   AsTools::Round_Rect(hdc, Prev_Parachute_Rect);
}
//-------------------------------------------------------------------------------------------------------------------------
