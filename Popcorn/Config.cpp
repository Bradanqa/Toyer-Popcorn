#include "Config.h"


//AsConfig CLASS
//-------------------------------------------------------------------------------------------------------------------------
bool AsConfig::Level_Has_Floor = false;
int AsConfig::Current_Timer_Tick = 0;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Orange_Color(255, 118, 14);
const AColor AsConfig::Blue_Color(106, 90, 235);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Gate_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Orange_Highlight(AsConfig::Orange_Color, 3 * AsConfig::Global_Scale); 
const AColor AsConfig::Advertisement_Orange_Table(AsConfig::Orange_Color, 2 * AsConfig::Global_Scale);
const AColor AsConfig::Advertisement_Blue_Table(0, 159, 159, AsConfig::Global_Scale);
const AColor AsConfig::Teleport_Portal_Color(AsConfig::Blue_Color, AsConfig::BG_Color, AsConfig::Global_Scale);

HWND AsConfig::Hwnd;

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const double AsConfig::Start_Ball_Y_Pos = 184.0;
const double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
const double AsConfig::Ball_Acceleration = 1.001;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;
//-------------------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
   throw 13;
}
//-------------------------------------------------------------------------------------------------------------------------



// AsTools
//-------------------------------------------------------------------------------------------------------------------------
int AsTools::Rand(int range)
{
   return rand() * range / RAND_MAX;
}
//-------------------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, RECT &rect, int corner_radius)
{
   int radius;

   radius = corner_radius * AsConfig::Global_Scale;
   RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, RECT &rect, const AColor &color)
{
   color.Select(hdc);
   Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, int x, int y, int width, int height , const AColor& color)
{
   const int scale = AsConfig::Global_Scale;

   color.Select(hdc);
   Rectangle(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}
//-------------------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT &rect)
{
   InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------