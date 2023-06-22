#include "Info_Panel.h"

// AIndicator
//-------------------------------------------------------------------------------------------------------------------------
AIndicator::AIndicator(int x_pos, int y_pos)
   : X_Pos(x_pos), Y_Pos(y_pos), End_Tick(0)
{
   const int scale = AsConfig::Global_Scale;

   Indicator_Rect.left = X_Pos * scale;
   Indicator_Rect.top = Y_Pos * scale;
   Indicator_Rect.right = Indicator_Rect.left + Width * scale;
   Indicator_Rect.bottom = Indicator_Rect.top + Height * scale;
}
//-------------------------------------------------------------------------------------------------------------------------
void AIndicator::Act()
{
   if (!Is_Finished())
      AsTools::Invalidate_Rect(Indicator_Rect);
}
//-------------------------------------------------------------------------------------------------------------------------
void AIndicator::Clear(HDC hdc, RECT& paint_area)
{

}
//-------------------------------------------------------------------------------------------------------------------------
void AIndicator::Draw(HDC hdc, RECT& paint_area)
{
   double ratio;
   double curr_height;
   int inner_x_offset = (Width - Inner_Width) / 2;
   int inner_y_offset = (Height - Inner_Height) / 2;
   const int scale = AsConfig::Global_Scale;
   RECT rect;

   AsTools::Rect(hdc, X_Pos, Y_Pos, Width, Height, AsConfig::Teleport_Portal_Color); // Left Indicator

   if (End_Tick == 0 || Is_Finished())
      return;

   ratio = (double)(End_Tick - AsConfig::Current_Timer_Tick) / (double)Indicator_Timeout;

   curr_height = (int)(double)(Inner_Height * scale) * ratio;

   rect.left = (X_Pos + inner_x_offset) * scale;
   rect.top = (Y_Pos + inner_y_offset) * scale + (Inner_Height * scale - curr_height);
   rect.right = rect.left + Inner_Width * scale;
   rect.bottom = (Y_Pos + inner_y_offset + Inner_Height) * scale;

   AsTools::Rect(hdc, rect, AsConfig::Orange_Color); // Left Indicator
}
//-------------------------------------------------------------------------------------------------------------------------
bool AIndicator::Is_Finished()
{
   if (AsConfig::Current_Timer_Tick > End_Tick)
      return true;
   else
      return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void AIndicator::Restart()
{
   End_Tick = AsConfig::Current_Timer_Tick + Indicator_Timeout;
}
//-------------------------------------------------------------------------------------------------------------------------




// AsInfo_Panel
int AsInfo_Panel::Score = 0;
int AsInfo_Panel::Extra_Life_Count = 5;
RECT AsInfo_Panel::Logo_Rect;
RECT AsInfo_Panel::Data_Rect;
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
   Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::Global_Scale - 1, 153 * AsConfig::Global_Scale),
   Floor_Indicator(Score_X + 8, Score_Y + Indicator_Y_Offset), Monster_Indicator(Score_X + 90, Score_Y + Indicator_Y_Offset)
{
   const int scale = AsConfig::Global_Scale;

   Logo_Rect.left = 211 * scale;
   Logo_Rect.top = 5 * scale;
   Logo_Rect.right = Logo_Rect.left + 104 * scale;
   Logo_Rect.bottom = Logo_Rect.top + 100 * scale;

   Data_Rect.left = Score_X * scale;
   Data_Rect.top = Score_Y * scale;
   Data_Rect.right = Data_Rect.left + Score_Width * scale;
   Data_Rect.bottom = Data_Rect.top + Score_Height * scale;

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
   Floor_Indicator.Act();
   Monster_Indicator.Act();
}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Clear(HDC hdc, RECT& paint_area)
{

}
//-------------------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw(HDC hdc, RECT& paint_area)
{
   const int scale = AsConfig::Global_Scale;
   const wchar_t *pop_str = L"POP";
   const wchar_t *corn_str = L"CORN";
   AString score_str(L"SCORE:");
   RECT rect, intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Logo_Rect))
   {
      // Logo
      AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);

      SetBkMode(hdc, TRANSPARENT);

      // POP
      SelectObject(hdc, Logo_Pop_Font);
      SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
      TextOut(hdc, (Logo_X_Pos + Shadow_X_Offset) * scale, (Logo_Y_Pos + Shadow_Y_Offset) * scale, pop_str, wcslen(pop_str));
      SetTextColor(hdc, AsConfig::Orange_Color.Get_RGB());
      TextOut(hdc, Logo_X_Pos * scale, Logo_Y_Pos * scale, pop_str, wcslen(pop_str));

      // CORN
      SelectObject(hdc, Logo_Corn_Font);
      SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
      TextOut(hdc, (Logo_X_Pos + Shadow_X_Offset - 4) * scale, (Logo_Y_Pos + Shadow_Y_Offset + 44) * scale, corn_str, wcslen(corn_str));
      SetTextColor(hdc, AsConfig::Orange_Color.Get_RGB());
      TextOut(hdc, (Logo_X_Pos - 4) * scale, (Logo_Y_Pos + 44) * scale, corn_str, wcslen(corn_str));
   }

   if (IntersectRect(&intersection_rect, &paint_area, &Data_Rect))
   {
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

      Player_Name = L"COMPUTER";
      Draw_String(hdc, rect, Player_Name, true);

      // Score panel
      rect.top += Score_Value_Offset * scale;
      rect.bottom += Score_Value_Offset * scale;

      score_str = L"SCORE:";

      score_str.Append(Score);
      Draw_String(hdc, rect, score_str, false);

      // Letter indicators
      Letter_P.Draw(hdc, paint_area);
      Letter_G.Draw(hdc, paint_area);
      Letter_M.Draw(hdc, paint_area);

      // Indicators
      Floor_Indicator.Draw(hdc, paint_area); // Left Indicator
      Monster_Indicator.Draw(hdc, paint_area); // Right Indicator

      // Lifes
      Show_Extra_Life(hdc);
   }
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
void AsInfo_Panel::Update_Score(EScore_Event_Type event_type)
{
   RECT rect;

   switch (event_type)
   {
   case EScore_Event_Type::Hit_Brick:
      Score += 20;
      break;

   case EScore_Event_Type::Hit_Monster:
      Score += 37;
      break;

   case EScore_Event_Type::Catch_Letter:
      Score += 23;
      break;

   default:
      AsConfig::Throw();
      break;
   }

   AsTools::Invalidate_Rect(Data_Rect);
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
void AsInfo_Panel::Show_Extra_Life(HDC hdc)
{
   int i, j;
   int life_to_draw = Extra_Life_Count;

   AsTools::Rect(hdc, Score_X + 27, Score_Y + Indicator_Y_Offset, 56, 30, AsConfig::Teleport_Portal_Color); // Middle Indicator

   for (j = 0; j < 3; j++)
      for (i = 0; i < 4; i++)
      {
         if (life_to_draw <= 0)
            break;

         Draw_Extra_Life(hdc, 27 + 6 + j * 16, 57 + i * 7);
         --life_to_draw;
      }
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
void AsInfo_Panel::Draw_String(HDC hdc, RECT &rect, AString &str, bool draw_name)
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

   GetTextExtentPoint32(hdc, str.Get_Content(), str.Get_Length(), &str_size);

   str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
   str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;

   // Draw shadow
   SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
   TextOut(hdc, str_left_offset + 1 * scale, str_top_offset + 1 * scale, str.Get_Content(), str.Get_Length());

   // Draw text
   SetTextColor(hdc, AsConfig::White_Color.Get_RGB());
   TextOut(hdc, str_left_offset, str_top_offset, str.Get_Content(), str.Get_Length());
}
//-------------------------------------------------------------------------------------------------------------------------
