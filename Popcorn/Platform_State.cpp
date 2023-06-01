#include "Platform_State.h"


//AsPlatform_State
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::AsPlatform_State()
	: Current_State(EPlatform_State::Regular), Next_State(EPlatform_State::Unknown), Regular(EPlatform_Substate_Regular::Missing), Moving(EPlatform_Moving_State::Stop),
	Meltdown(EPlatform_Substate_Meltdown::Unknown), Rolling(EPlatform_Substate_Rolling::Roll_In), Glue(EPlatform_Transformation::Unknown), Laser(EPlatform_Transformation::Unknown),
	Expanding(EPlatform_Transformation::Unknown)
{
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::operator EPlatform_State() const
{
	return Current_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::operator = (EPlatform_State new_state)
{
	Current_State = new_state;
}

void AsPlatform_State::Set_Next_State(EPlatform_State next_state)
{
	if (next_state == Current_State)
		return;

	switch (Current_State)
	{
	case EPlatform_State::Regular:
		break;

	case EPlatform_State::Meltdown:
		return;

	case EPlatform_State::Rolling:
		AsConfig::Throw();
		break;

	case EPlatform_State::Glue:
		Glue = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Laser:
		Laser = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Expanding:
		Expanding = EPlatform_Transformation::Finalize;
		break;

	default:
		AsConfig::Throw();
	}

	Next_State = next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Get_Next_State()
{
	return Next_State;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_Transformation *transformation_state = 0;

	if (Current_State == EPlatform_State::Regular && Regular == new_regular_state)
		return EPlatform_State::Unknown;

	if (new_regular_state == EPlatform_Substate_Regular::Normal)
	{
		switch (Current_State)
		{
		case EPlatform_State::Glue:
			transformation_state = &Glue;
			break;

		case EPlatform_State::Expanding:
			transformation_state = &Expanding;
			break;

		case EPlatform_State::Laser:
			transformation_state = &Laser;
			break;
		}

		if (transformation_state != 0)
		{
			if (*transformation_state == EPlatform_Transformation::Unknown)
				return Set_Next_Or_Regular_State(new_regular_state); // ‘инализаци€ состо€ни€ закончилась
			else
				*transformation_state = EPlatform_Transformation::Finalize; // «апускаем финализацию состо€ни€

			return EPlatform_State::Unknown;
		}
	}

	Current_State = EPlatform_State::Regular;
	Regular = new_regular_state;

	return EPlatform_State::Unknown;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state)
{ // ¬озврат: если не Unknown, то нужно перейти в это новое состо€ние

	EPlatform_State next_state;

	Current_State = EPlatform_State::Regular;

	// ≈сли есть отложенное состо€ние, то переведем в него, а не в –егулар

	next_state = Get_Next_State();

	if (next_state == EPlatform_State::Unknown)
		Regular = new_regular_state;

	return next_state;
}
//------------------------------------------------------------------------------------------------------------