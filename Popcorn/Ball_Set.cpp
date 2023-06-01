#include "Ball_Set.h"

//ABall_Set
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Act()
{
   int i;
   ABall *curr_ball;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::On_Platform)
         if (curr_ball->Release_Timer_Tick != 0 && AsConfig::Current_Timer_Tick >= curr_ball->Release_Timer_Tick)
            curr_ball->Release();
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Release_From_Platform(double platform_x_pos)
{
   int i;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
      if (Balls[i].Get_State() == EBall_State::On_Platform)
         Balls[i].Set_State(EBall_State::Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::Release_Next_Ball()
{
   int i;
   ABall *curr_ball;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];
      if (curr_ball->Get_State() == EBall_State::On_Platform)
      {
         curr_ball->Release();
         return true;
      }
   }
   return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Set_On_Platform(double platfprm_x_pos)
{
   int i;

   for (i = 0; i < 1; i++)
   {
      Balls[0].Set_State(EBall_State::Normal);
      Balls[0].Set_State(EBall_State::On_Platform, platfprm_x_pos, AsConfig::Start_Ball_Y_Pos);
   }

   for (; i < AsConfig::Max_Ball_Count; i++)
      Balls[i].Set_State(EBall_State::Disabled);
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::All_Balls_Are_Lost()
{
   int i;
   int active_ball_count = 0;
   int lost_ball_count = 0;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      if (Balls[i].Get_State() == EBall_State::Disabled)
         continue;

      ++active_ball_count;

      if (Balls[i].Get_State() == EBall_State::Lost)
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

   int i;
   ABall *curr_ball;
   ABall *f_ball = 0;
   ABall *left_cond = 0, * right_cond = 0;
   double curr_ball_x, curr_ball_y;
   double f_ball_x, f_ball_y;

   // Выбираем самый дальний мячик по Y
   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];

      if (Balls[i].Get_State() != EBall_State::Normal)
         continue;

      if (f_ball == 0)
         f_ball = &Balls[i];
      else
      {
         curr_ball->Get_Center(curr_ball_x, curr_ball_y);
         f_ball->Get_Center(f_ball_x, f_ball_y);

         if (curr_ball_y < f_ball_y)
            f_ball = curr_ball;
      }
   }

   // Размножить "Нормальный" мячик если он есть
   if (f_ball == 0)
      return;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];

      switch (curr_ball->Get_State())
      {
      case EBall_State::Disabled:
      case EBall_State::Lost:
         if (left_cond == 0)
            left_cond = curr_ball;
         else
            if (right_cond == 0)
            {
               right_cond = curr_ball;
               break;
            }
      }
   }

   // Разводим боковые мячики в стороны
   if (left_cond != 0)
   {
      *left_cond = *f_ball;
      left_cond->Set_Direction(left_cond->Get_Direction() + AsConfig::Min_Ball_Angle);
   }

   if (right_cond != 0)
   {
      *right_cond = *f_ball;
      right_cond->Set_Direction(right_cond->Get_Direction() - AsConfig::Min_Ball_Angle);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Inverse_Balls()
{// Меняем направление у всех мячиков

   int i;
   ABall* curr_ball;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::Normal)
         curr_ball->Set_Direction(curr_ball->Get_Direction() + M_PI);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Accelerate()
{
   int i;
   ABall* curr_ball;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::Normal)
         curr_ball->Set_Speed(curr_ball->Get_Speed() * AsConfig::Ball_Acceleration);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::Reset_Speed()
{
   int i;
   ABall* curr_ball;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::Normal)
         curr_ball->Set_Speed(AsConfig::Normal_Ball_Speed);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
void ABall_Set::On_Platform_Advance(double direction, double platform_speed, double max_speed)
{
   int i;
   ABall* curr_ball;

   for (i = 0; i < AsConfig::Max_Ball_Count; i++)
   {
      curr_ball = &Balls[i];
      if (curr_ball->Get_State() == EBall_State::On_Platform)
         curr_ball->Forced_Advance(direction, platform_speed, max_speed);
   }
}
//-------------------------------------------------------------------------------------------------------------------------
bool ABall_Set::Get_Next_Game_Object(int &index, AGame_Object **game_object)
{
   if (index < 0 || index >= AsConfig::Max_Ball_Count)
      return false;

   *game_object = &Balls[index++];

   return true;
}
//-------------------------------------------------------------------------------------------------------------------------
