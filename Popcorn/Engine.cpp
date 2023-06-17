#include "Engine.h"

// AsInfo_Panel
//-------------------------------------------------------------------------------------------------------------------------
AsInfo_Panel::~AsInfo_Panel()
{
   delete Shadow_Color;
   delete Highlight_Color;
   delete Dark_Blue;
   delete Dark_Orange;

   if (Logo_Pop_Font != 0)
      DeleteObject(Logo_Pop_Font);

   if (Logo_Corn_Font != 0)
      DeleteObject(Logo_Corn_Font);

   if (Name_Font != 0)
      DeleteObject(Name_Font);

   if (Score_Font != 0)
      DeleteObject(Score_Font);
}
//-------------------------------------------------------------------------------------------------------------------------
AsInfo_Panel::AsInfo_Panel()
   : Logo_Pop_Font(0), Logo_Corn_Font(0), Name_Font(0), Score_Font(0), Shadow_Color(0), Highlight_Color(0), Dark_Blue(0), Dark_Orange(0), 
   Letter_P(EBrick_Type::Blue, ELetter_Type::P, 214 * AsConfig::Global_Scale + 1, 153 * AsConfig::Global_Scale),
   Letter_G(EBrick_Type::Blue, ELetter_Type::G, 256 * AsConfig::Global_Scale, 153 * AsConfig::Global_Scale),
   Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::Global_Scale - 1, 153 * AsConfig::Global_Scale)
{
   Letter_P.Show_Full_Size();
   Letter_G.Show_Full_Size();
   Letter_M.Show_Full_Size();
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Begin_Movement()
{

}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Finish_Movement()
{

}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Advance(double max_speed)
{

}
//-------------------------------------------------------------------------------------------------------------------------
double AsInfo_Panel::Get_Speed()
{
   return 0.0;
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Act()
{

}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Clear(HDC hdc, RECT& paint_area)
{

}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw(HDC hdc, RECT& paint_area)
{
   int i, j;
   const int scale = AsConfig::Global_Scale;
   int logo_x_pos = 214 * scale;
   int logo_y_pos = 0;
   int shadow_x_offset = 3 * scale;
   int shadow_y_offset = 6 * scale;
   const wchar_t *pop_str = L"POP";
   const wchar_t *corn_str = L"CORN";
   const wchar_t *player_str = L"COMPUTER";
   const wchar_t *score_str = L"SCORE:000000";
   RECT rect;

   // Logo
   AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);

   SetBkMode(hdc, TRANSPARENT);

   SelectObject(hdc, Logo_Pop_Font);
   SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
   TextOut(hdc, logo_x_pos + shadow_x_offset, logo_y_pos + shadow_y_offset, pop_str, wcslen(pop_str));
   SetTextColor(hdc, AsConfig::Orange_Color.Get_RGB());
   TextOut(hdc, logo_x_pos, logo_y_pos, pop_str, wcslen(pop_str));

   SelectObject(hdc, Logo_Corn_Font);
   SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
   TextOut(hdc, logo_x_pos - 4 * scale + shadow_x_offset, logo_y_pos + 44 * scale + shadow_y_offset, corn_str, wcslen(corn_str));
   SetTextColor(hdc, AsConfig::Orange_Color.Get_RGB());
   TextOut(hdc, logo_x_pos - 4 * scale, logo_y_pos + 44 * scale, corn_str, wcslen(corn_str));

   // Score panel
   // Orange border
   AsTools::Rect(hdc, Score_X, Score_Y, Score_Width, 2, AsConfig::Orange_Color);
   AsTools::Rect(hdc, Score_X, Score_Y + Score_Height - 2, Score_Width, 2, AsConfig::Orange_Color);
   AsTools::Rect(hdc, Score_X, Score_Y, 2, Score_Height, AsConfig::Orange_Color);
   AsTools::Rect(hdc, Score_X + Score_Width - 2, Score_Y, 2, Score_Height, AsConfig::Orange_Color);
   
   // Main dark rect
   AsTools::Rect(hdc, Score_X + 2, Score_Y + 2, Score_Width - 4, Score_Height - 4, *Dark_Blue);

   // Highlight and shadow border
   Highlight_Color->Select_Pen(hdc);
   MoveToEx(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale, 0);
   LineTo(hdc, (Score_X + 2) * scale, (Score_Y + 2) * scale);
   LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale);

   Shadow_Color->Select_Pen(hdc);
   MoveToEx(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale, 0);
   LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + Score_Height - 2) * scale);
   LineTo(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale);

   // Player name rect
   rect.left = (Score_X + 5) * scale;
   rect.top = (Score_Y + 5) * scale;
   rect.right = rect.left + (Score_Width - 2 * 5) * scale;
   rect.bottom = rect.top + 16 * scale;

   Draw_String(hdc, rect, player_str, true);

   // Score panel
   rect.top += Score_Value_Offset * scale;
   rect.bottom += Score_Value_Offset * scale;
   Draw_String(hdc, rect, score_str, false);

   // Letter indicators
   Letter_P.Draw(hdc, paint_area);
   Letter_G.Draw(hdc, paint_area);
   Letter_M.Draw(hdc, paint_area);

   // Indicators
   AsTools::Rect(hdc, Score_X + 8, Score_Y + 55, 12, 30, AsConfig::Teleport_Portal_Color); // Left Ind
   AsTools::Rect(hdc, Score_X + 27, Score_Y + 55, 56, 30, AsConfig::Teleport_Portal_Color); // Middle Ind
   AsTools::Rect(hdc, Score_X + 90, Score_Y + 55, 12, 30, AsConfig::Teleport_Portal_Color); // Right Ind

   // Lifes
   for (i = 0; i < 4; i++)
      for (j = 0; j < 3; j++)
      Draw_Extra_Life(hdc, 27 + 6 + j * 16, 57 + i * 7);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_Extra_Life(HDC hdc, int x_pos, int y_pos)
{
   RECT rect;
   const int scale = AsConfig::Global_Scale;

   // Lifes
   rect.left = (Score_X + x_pos) * scale;
   rect.top = (Score_Y + y_pos) * scale;
   rect.right = rect.left + 4 * scale;
   rect.bottom = rect.top + 5 * scale;

   AsTools::Ellipse(hdc, rect, AsConfig::Orange_Color);

   rect.left += 8 * scale;
   rect.right += 8 * scale;
   AsTools::Ellipse(hdc, rect, AsConfig::Orange_Color);

   rect.left = (Score_X + x_pos + 2) * scale;
   rect.top = (Score_Y + y_pos + 1) * scale;
   rect.right = rect.left + 8 * scale;
   rect.bottom = rect.top + 3 * scale;

   AsConfig::Blue_Color.Select(hdc);
   AsTools::Round_Rect(hdc, rect);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_String(HDC hdc, RECT &rect, const wchar_t *str, bool draw_name)
{
   const int scale = AsConfig::Global_Scale;
   int str_left_offset, str_top_offset;
   SIZE str_size;

   // Background rect
   AsTools::Rect(hdc, rect, AsConfig::Orange_Color);

   // Draw string
   if (draw_name)
      SelectObject(hdc, Name_Font);
   else
      SelectObject(hdc, Score_Font);

   GetTextExtentPoint32(hdc, str, wcslen(str), &str_size);

   str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
   str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;

   // Draw shadow
   SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
   TextOut(hdc, str_left_offset + 1 * scale, str_top_offset + 1 * scale, str, wcslen(str));

   // Draw text
   SetTextColor(hdc, AsConfig::White_Color.Get_RGB());
   TextOut(hdc, str_left_offset, str_top_offset, str, wcslen(str));
}
//-------------------------------------------------------------------------------------------------------------------------
bool AsInfo_Panel::Is_Finished()
{
   return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Init()
{
   LOGFONT log_font{};

   log_font.lfHeight = -128;
   log_font.lfWeight = 900;
   log_font.lfOutPrecision = 3;
   log_font.lfClipPrecision = 2;
   log_font.lfQuality = 1;
   log_font.lfPitchAndFamily = 34;
   wcscpy_s(log_font.lfFaceName, L"Arial Black");
   Logo_Pop_Font = CreateFontIndirect(&log_font);

   log_font.lfHeight = -96;
   Logo_Corn_Font = CreateFontIndirect(&log_font);


   log_font.lfHeight = -48;
   log_font.lfWeight = 700;
   log_font.lfOutPrecision = 3;
   log_font.lfClipPrecision = 2;
   log_font.lfQuality = 1;
   log_font.lfPitchAndFamily = 49;
   wcscpy_s(log_font.lfFaceName, L"Consolas");
   Name_Font = CreateFontIndirect(&log_font);

   log_font.lfHeight = -44;
   Score_Font = CreateFontIndirect(&log_font);

   Shadow_Color = new AColor(AsConfig::BG_Color, AsConfig::Global_Scale);
   Highlight_Color = new AColor(AsConfig::White_Color, AsConfig::Global_Scale);
   Dark_Blue = new AColor(69, 58, 153);
   Dark_Orange = new AColor(179, 82, 9);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Choose_Font()
{
   CHOOSEFONT cf{};
   LOGFONT lf{};

   cf.lStructSize = sizeof(CHOOSEFONT);
   cf.lpLogFont = &lf;
   cf.Flags = CF_SCREENFONTS;
   cf.nFontType = SCREEN_FONTTYPE;

   ChooseFont(&cf);
}
//-------------------------------------------------------------------------------------------------------------------------




//AsEngine
//-------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
   :  Timer_ID(WM_USER + 1), Game_State(EGame_State::Lost_Ball), Rest_Distance(0), Life_Count(AsConfig::Initial_Life_Count), Modules{}
{
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{//Настройка игры при старте

   int index;
   SYSTEMTIME sis_time;
   FILETIME file_time;

   GetSystemTime(&sis_time);
   SystemTimeToFileTime(&sis_time, &file_time);

   srand(file_time.dwLowDateTime);

   AsConfig::Hwnd = hwnd;

   AActive_Brick_Blue_Orange::Setup_Color();
   AExplosive_Ball::Setup_Color();

   Level.Init();
   Platform.Init(&Ball_Set, &Laser_Beam_Set);
   Monster_Set.Init(&Border);
   Info_Panel.Init();

   AFalling_Letter::Init();

   ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
   ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
   ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);
   ABall::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

   ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);
   ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

   AsPlatform::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

   Level.Set_Current_Level(AsLevel::Level_01);

   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

   // Modules
   memset(Modules, 0, sizeof(Modules));
   index = 0;

   Add_Next_Module(index, &Level);
   Add_Next_Module(index, &Border);
   Add_Next_Module(index, &Platform);
   Add_Next_Module(index, &Ball_Set);
   Add_Next_Module(index, &Laser_Beam_Set);
   Add_Next_Module(index, &Monster_Set);
   Add_Next_Module(index, &Info_Panel);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Отрисовка экрана игры

   int i;

   SetGraphicsMode(hdc, GM_ADVANCED);

   for (i = 0; i < AsConfig::Max_Modules_Count; i++)
      if (Modules[i] != 0)
         Modules[i]->Clear(hdc, paint_area);

   for (i = 0; i < AsConfig::Max_Modules_Count; i++)
      if (Modules[i] != 0)
         Modules[i]->Draw(hdc, paint_area);
}
//-------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
   switch (key_type)
   {
   case EKey_Type::Left:
      Platform.Move(true, key_down);
      break;

   case EKey_Type::Right:
      Platform.Move(false, key_down);
      break;

   case EKey_Type::Space:
      Platform.On_Space_Key(key_down);
      break;
   }
   return 0;
}
//-------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
   ++AsConfig::Current_Timer_Tick;

   switch (Game_State)
   {
   case EGame_State::Test_Ball:
      Ball_Set.Set_For_Test();
      Game_State = EGame_State::Play_Level;
      break;

   case EGame_State::Play_Level:
      Play_Level();
      break;

   case EGame_State::Lost_Ball:
      if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
         Restart_Level();
      break;

   case EGame_State::Restart_Level:
      if (Platform.Has_State(EPlatform_Substate_Regular::Ready))
      {
         Game_State = EGame_State::Play_Level;
         Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos());
         Monster_Set.Activate(3);
      }
      break;

   default:
      break;
   }

   Act();

   return 0;
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
   Game_State = EGame_State::Restart_Level;
   Border.Open_Gate(7, true);
   //Border.Open_Gate(5, false);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
   Advance_Movers();

   if (Ball_Set.All_Balls_Are_Lost())
   {// Потеряли все мячики
      Game_State = EGame_State::Lost_Ball;
      Level.Stop();
      Monster_Set.Destroy_All();
      Laser_Beam_Set.Disable_All();
      Platform.Set_State(EPlatform_State::Meltdown);
   }
   else
      Ball_Set.Accelerate();

   if (Ball_Set.Is_Test_Finished())
      Game_State = EGame_State::Test_Ball;
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
   int i;
   double current_speed, max_speed = 0.0;

   // Получаем максимальную скорость движущихся объектов
   for (i = 0; i < AsConfig::Max_Movers_Count; i++)
   {
      if (Modules[i] != 0)
      {
         Modules[i]->Begin_Movement();

         current_speed = fabs(Modules[i]->Get_Speed());

         if (current_speed > max_speed)
            max_speed = current_speed;
      }
   }

   // 1. Смещаем все объекты
   Rest_Distance += max_speed;

   while (Rest_Distance > 0.0)
   {
      for (i = 0; i < AsConfig::Max_Movers_Count; i++)
      {
         if (Modules[i] != 0)
            Modules[i]->Advance(max_speed);
      }

      Rest_Distance -= AsConfig::Moving_Step_Size;
   }

   // Заканчиваем все движения на этом кадре
   for (i = 0; i < AsConfig::Max_Movers_Count; i++)
      if (Modules[i] != 0)
         Modules[i]->Finish_Movement();
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
   int i;
   int index = 0;
   AFalling_Letter *falling_letter;

   // 1. Выполняем все действия
   for (i = 0; i < AsConfig::Max_Modules_Count; i++)
      if (Modules[i] != 0)
         Modules[i]->Act();

   // 2. Ловим пдаюзие буквы
   while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
   {
      if (Platform.Hit_By(falling_letter))
         On_Falling_Letter(falling_letter);
   }

   // 3. Выпускаем мячик если гейт открыт
   if (Game_State == EGame_State::Restart_Level)
      if (Border.Is_Gate_Opened(AsConfig::Gate_Count - 1))
         Platform.Set_State(EPlatform_State::Rolling);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
   switch (falling_letter->Letter_Type)
   {
   case ELetter_Type::O:
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   case ELetter_Type::I:
      Ball_Set.Inverse_Balls();
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   case ELetter_Type::C:
      Ball_Set.Reset_Speed();
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   //case ELetter_Type::M:
   //   break;

   case ELetter_Type::G:
      if (Life_Count < AsConfig::Max_Life_Count)
         ++Life_Count;
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   case ELetter_Type::K:
      Platform.Set_State(EPlatform_State::Glue);
      break;

   case ELetter_Type::W:
      Platform.Set_State(EPlatform_State::Expanding);
      break;

   case ELetter_Type::T:
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      Ball_Set.Triple_Balls();
      break;

   case ELetter_Type::L:
      Platform.Set_State(EPlatform_State::Laser);
      break;

   case ELetter_Type::P:
      AsConfig::Level_Has_Floor = true;
      Border.Redraw_Floor();
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   //case ELetter_Type::Plus:
   //   break;

   default:
      break;
   }

   falling_letter->Finalize();
}
//-------------------------------------------------------------------------------------------------------------------------
void AsEngine::Add_Next_Module(int &index, AGame_Object *object)
{
   if (index >= 0 && index < AsConfig::Max_Modules_Count)
      Modules[index++] = object;
   else
      AsConfig::Throw();
}
//-------------------------------------------------------------------------------------------------------------------------



