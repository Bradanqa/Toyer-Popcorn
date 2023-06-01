#include "Laser_Beam_Set.h"


// ALaser_Beam_Set
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Begin_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Ball_Count; i++)
		Laser_Beams[i].Begin_Movement();
}
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Finish_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Ball_Count; i++)
		Laser_Beams[i].Finish_Movement();
}
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Advance(double max_speed)
{
	int i;

	for (i = 0; i < AsConfig::Max_Ball_Count; i++)
		Laser_Beams[i].Advance(max_speed);
}
//-------------------------------------------------------------------------------------------------------------------------
double ALaser_Beam_Set::Get_Speed()
{
	int i;
	double curr_speed, max_speed = 0.0;

	for (i = 0; i < AsConfig::Max_Ball_Count; i++)
	{
		curr_speed = Laser_Beams[i].Get_Speed();

		if (curr_speed > max_speed)
			max_speed = curr_speed;
	}

	return max_speed;
}
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Act()
{
}
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Clear(HDC hdc, RECT& paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Ball_Count; i++)
		Laser_Beams[i].Clear(hdc, paint_area);
}
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Draw(HDC hdc, RECT& paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Ball_Count; i++)
		Laser_Beams[i].Draw(hdc, paint_area);
}
//-------------------------------------------------------------------------------------------------------------------------
bool ALaser_Beam_Set::Is_Finished()
{
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------
void ALaser_Beam_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
	int i;
	ALaser_Beam *left_beam = 0, *right_beam = 0;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
	{
		if (Laser_Beams[i].Is_Active())
			continue;

		if (left_beam == 0)
			left_beam = &Laser_Beams[i];
		else
			if (right_beam == 0)
			{
				right_beam = &Laser_Beams[i];
				break;
			}
	}
	if (left_beam == 0 || right_beam == 0)
		AsConfig::Throw();


	left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
	right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
}
//------------------------------------------------------------------------------------------------------------

