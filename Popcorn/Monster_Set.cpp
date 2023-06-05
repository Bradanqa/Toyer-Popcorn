#include "Monster_Set.h"

// AMonster
const double AMonster::Max_Cornea_Height = 11.0;
const double AMonster::Blinks_Timeouts[Blink_Stages_Count] = {0.4, 0.2, 0.8, 0.4, 0.4, 0.4, 0.8};
const EEye_State AMonster::Blink_States[Blink_Stages_Count] =
{
	EEye_State::Closed, EEye_State::Openning, EEye_State::Staring, EEye_State::Closing,
	EEye_State::Openning, EEye_State::Staring, EEye_State::Closing
};
//-------------------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
	: Is_Active(false), Eye_State(EEye_State::Closed), X_Pos(0), Y_Pos(0), Cornea_Height(Max_Cornea_Height), Start_Blink_Timeout(0), Total_Animation_Timeout(0),
	Monster_Rect{}, Blink_Ticks{}
{
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Advance(double max_speed)
{
}
//-------------------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	return 0.0;
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	int i;
	int curr_tick_offset;
	int prev_tick;
	double ratio;

	if (!Is_Active)
		return;

	curr_tick_offset = (AsConfig::Current_Timer_Tick - Start_Blink_Timeout) % Total_Animation_Timeout;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		if (curr_tick_offset < Blink_Ticks[i])
		{
			Eye_State =  Blink_States[i];
			break;
		}
	}

	if (i == 0)
		prev_tick = 0;
	else
		prev_tick = Blink_Ticks[i - 1];

	ratio = (double)(curr_tick_offset - prev_tick) / (double)(Blink_Ticks[i] - prev_tick);

	switch (Eye_State)
	{
	case EEye_State::Closed:
		Cornea_Height = 0.0;
		break;

	case EEye_State::Openning:
		Cornea_Height = Max_Cornea_Height * ratio;
		break;

	case EEye_State::Staring:
		Cornea_Height = Max_Cornea_Height;
		break;

	case EEye_State::Closing:
		Cornea_Height = Max_Cornea_Height * (1.0 - ratio);
		break;

	default:
		AsConfig::Throw();
	}

	AsTools::Invalidate_Rect(Monster_Rect);
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Clear(HDC hdc, RECT& paint_area)
{
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT& paint_area)
{
	HRGN region;
	RECT itersection_rect;
	RECT rect, cornea_rect;
	const int scale = AsConfig::Global_Scale;
	const int half_scale = scale / 2;

	if (!Is_Active)
		return;

	if (!IntersectRect(&itersection_rect, &paint_area, &Monster_Rect))
		return;

	rect = Monster_Rect;

	++rect.right;
	++rect.bottom;

	region = CreateEllipticRgnIndirect(&Monster_Rect);
	SelectClipRgn(hdc, region);

	// Темый Фон
	AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Monster_Dark_Orange);

	// Светлый Фон
	rect = Monster_Rect;

	rect.left -= 2 * scale;
	rect.top -= 3 * scale;
	rect.right -= 2 * scale;
	rect.bottom -= 3 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Orange_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	if (Eye_State == EEye_State::Closed)
		return;

	// Роговица
	cornea_rect = Monster_Rect;

	cornea_rect.left += scale + half_scale;
	cornea_rect.top += 2 * scale + (int)((Max_Cornea_Height / 2.0 - Cornea_Height / 2.0) * AsConfig::D_Global_Scale);
	cornea_rect.right -= scale + half_scale;
	cornea_rect.bottom = cornea_rect.top + Cornea_Height * AsConfig::D_Global_Scale;

	region = CreateEllipticRgnIndirect(&cornea_rect);
	SelectClipRgn(hdc, region);

	AsTools::Ellipse(hdc, cornea_rect, AsConfig::Monster_Cornea_Color);

	// Радужка
	rect = Monster_Rect;

	rect.left += 4 * scale + half_scale;
	rect.top += 4 * scale;
	rect.right -= 4 * scale + half_scale;
	rect.bottom -= 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Monster_Iris_Color);

	// Зрачок
	rect = Monster_Rect;

	rect.left += 7 * scale;
	rect.top += 6 * scale;
	rect.right = rect.left + 2 * scale;
	rect.bottom = rect.top + 3 * scale;

	AsTools::Rect(hdc, rect, AsConfig::BG_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	// Обводка роговицы
	AsConfig::BG_Outline_Color.Select(hdc);
	Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right - 1, cornea_rect.bottom - 1, 0, 0, 0, 0);
}
//-------------------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void AMonster::Activate(int x_pos, int y_pos)
{
	int i;
	int tick_offset;
	double curr_timeout = 0.0;
	const int scale = AsConfig::Global_Scale;

	Is_Active = true;

	X_Pos = x_pos + 10;
	Y_Pos = y_pos;

	Monster_Rect.left = X_Pos * scale;
	Monster_Rect.top = Y_Pos * scale;
	Monster_Rect.right = Monster_Rect.left + Width * scale;
	Monster_Rect.bottom = Monster_Rect.top + Height * scale;

	// Рассчитываем тики анимации
	Start_Blink_Timeout = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		curr_timeout += Blinks_Timeouts[i];
		tick_offset = (int)((double)AsConfig::FPS * curr_timeout);
		Blink_Ticks[i] = tick_offset;
	}

	Total_Animation_Timeout = tick_offset;
}
//-------------------------------------------------------------------------------------------------------------------------




// AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
: Border(0)
{
}
void AsMonster_Set::Init(AsBorder* border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_At_Gate(int gate_index)
{
	int i;
	int gate_x_pos, gate_y_pos;
	AMonster *monster = 0;

	if (gate_index < 0 || gate_index >= AsConfig::Gate_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (! Monsters[i].Is_Active)
		{
			monster = &Monsters[i];
			break;
		}
	}
	if (monster == 0)
		return;

	Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);

	monster->Activate(gate_x_pos, gate_y_pos);
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_Game_Object(int &index, AGame_Object **game_object)
{
	if (index < 0 || index >= AsConfig::Max_Ball_Count)
		return false;

	*game_object = &Monsters[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------
