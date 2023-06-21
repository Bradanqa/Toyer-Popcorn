#include "Ball_Set.h"

//ABall_Set
//-------------------------------------------------------------------------------------------------------------------------
ABall_Set::ABall_Set()
   : Balls(AsConfig::Max_Ball_Count)
{
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Act()
{
   for (auto &ball : Balls)
      if (ball.Get_State() == EBall_State::On_Platform)
         if (ball.Release_Timer_Tick != 0 && AsConfig::Current_Timer_Tick >= ball.Release_Timer_Tick)
            ball.Release();
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Release_From_Platform(double platform_x_pos)
{
   for (auto &ball : Balls)
      if (ball.Get_State() == EBall_State::On_Platform)
         ball.Set_State(EBall_State::Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::Release_Next_Ball()
{
   for (auto &ball : Balls)
      if (ball.Get_State() == EBall_State::On_Platform)
      {
         ball.Release();
         return true;
      }
   return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Set_On_Platform(double platfprm_x_pos)
{
   int i;

   for (i = 0; i < 1; i++)
   {
      Balls[i].Set_State(EBall_State::Normal);
      Balls[i].Set_State(EBall_State::On_Platform, platfprm_x_pos, AsConfig::Start_Ball_Y_Pos);
      Balls[i].Release_Timer_Tick = 0;
   }

   for (; i < (int)Balls.size(); i++)
      Balls[i].Set_State(EBall_State::Disabled);
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::All_Balls_Are_Lost()
{
   int active_ball_count = 0;
   int lost_ball_count = 0;

   for (auto &ball : Balls)
   {
      if (ball.Get_State() == EBall_State::Disabled)
         continue;

      ++active_ball_count;

      if (ball.Get_State() == EBall_State::Lost)
      {
         ++lost_ball_count;
         continue;
      }
   }

   if (active_ball_count == lost_ball_count)
      return true;
   else
      return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Set_For_Test()
{
   Balls[0].Set_For_Test();
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::Is_Test_Finished()
{
   return Balls[0].Is_Test_Finished();
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Triple_Balls()
{ // "Растроить" самый дальний летящий от платформы мячик

   ABall *f_ball = 0;
   ABall *left_cond = 0, * right_cond = 0;
   double curr_ball_x, curr_ball_y;
   double f_ball_x, f_ball_y;
   double prev_direction, direction_delta;

   // Выбираем самый дальний мячик по Y
   for (auto &ball : Balls)
   {
      if (ball.Get_State() != EBall_State::Normal)
         continue;

      if (f_ball == 0)
         f_ball = &ball;
      else
      {
         ball.Get_Center(curr_ball_x, curr_ball_y);
         f_ball->Get_Center(f_ball_x, f_ball_y);

         if (curr_ball_y < f_ball_y)
            f_ball = &ball;
      }
   }

   // Размножить "Нормальный" мячик если он есть
   if (f_ball == 0)
      return;

   for (auto &ball : Balls)
   {
      switch (ball.Get_State())
      {
      case EBall_State::Disabled:
      case EBall_State::Lost:
         if (left_cond == 0)
            left_cond = &ball;
         else
            if (right_cond == 0)
            {
               right_cond = &ball;
               break;
            }
      }
   }

   // Разводим боковые мячики в стороны
   if (left_cond != 0)
   {
      *left_cond = *f_ball;
      Turn_Tripled_Ball(left_cond, true);
   }

   if (right_cond != 0)
   {
      *right_cond = *f_ball;
      Turn_Tripled_Ball(right_cond, false);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Turn_Tripled_Ball(ABall *ball, bool add_min_angle)
{// Корректируем направление растроившегося мячика

   double prev_direction, direction_delta;
   double correction_angle;

   prev_direction = ball->Get_Direction();

   if (add_min_angle)
      correction_angle = AsConfig::Min_Ball_Angle;
   else
      correction_angle = -AsConfig::Min_Ball_Angle;

   ball->Set_Direction(prev_direction + correction_angle);

   direction_delta = fabs(ball->Get_Direction() - prev_direction);

   if (direction_delta < AsConfig::Min_Ball_Angle / 2.0)
      ball->Set_Direction(prev_direction - correction_angle / 2.0);
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Inverse_Balls()
{// Меняем направление у всех мячиков

   for (auto &ball : Balls)
   {
      if (ball.Get_State() == EBall_State::Normal)
         ball.Set_Direction(ball.Get_Direction() + M_PI);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Accelerate()
{
   for (auto &ball : Balls)
   {
      if (ball.Get_State() == EBall_State::Normal)
         ball.Set_Speed(ball.Get_Speed() * AsConfig::Ball_Acceleration);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Reset_Speed()
{
   for (auto &ball : Balls)
   {
      if (ball.Get_State() == EBall_State::Normal)
         ball.Set_Speed(AsConfig::Normal_Ball_Speed);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::On_Platform_Advance(double direction, double platform_speed, double max_speed)
{
   for (auto &ball : Balls)
   {
      if (ball.Get_State() == EBall_State::On_Platform)
         ball.Forced_Advance(direction, platform_speed, max_speed);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::Get_Next_Game_Object(int &index, AGame_Object **game_object)
{
   if (index < 0 || index >= (int)Balls.size())
      return false;

   *game_object = &Balls[index++];

   return true;
}
//-------------------------------------------------------------------------------------------------------------------------
