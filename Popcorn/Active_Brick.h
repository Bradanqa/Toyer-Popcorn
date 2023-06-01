#pragma once
#include "Ball.h"

//-------------------------------------------------------------------------------------------------------------------------
enum class EBrick_Type: unsigned char
{
   None,

   Orange,
   Blue,
	Unbreakable,
	Multihit_1,
	Multihit_2,
	Multihit_3,
	Multihit_4,
	Parachute,
	Teleport,
	Ad,
	Invisible
};
//-------------------------------------------------------------------------------------------------------------------------
enum class EDirection_Type: unsigned char
{
	Left,
	Up,
	Right,
	Down
};
//-------------------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
public:
	void Get_Level_Pos(int &dest_brick_x, int &dest_brick_y);

protected:;
	virtual ~AActive_Brick();
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Clear(HDC hdc, RECT& paint_area);

	double Get_Brick_X_Pos(bool of_center);
	double Get_Brick_Y_Pos(bool of_center);

	int Level_X, Level_Y;
	EBrick_Type Brick_Type;
	RECT Brick_Rect;
};
//-------------------------------------------------------------------------------------------------------------------------
class AActive_Brick_Blue_Orange : public AActive_Brick
{
public:
	~AActive_Brick_Blue_Orange();
	AActive_Brick_Blue_Orange(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	static void Setup_Color();
	static void Draw_In_Level(HDC hdc, RECT& brick_rect, EBrick_Type brick_type);

private:
	int Fade_Step;

	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor& origin_color, int step, AColor& result_color);
	static const int Max_Fade_Step = AsConfig::FPS;

	static AColor Fading_Orange_Brick_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Brick_Colors[Max_Fade_Step];
};
//-------------------------------------------------------------------------------------------------------------------------
class AActive_Brick_Unbreakable: public AActive_Brick
{
public:
	~AActive_Brick_Unbreakable();
	AActive_Brick_Unbreakable(int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();
	static void Draw_In_Level(HDC hdc, RECT& brick_rect);

private:
	int Animation_Step;
	HRGN Region;

	static const int Max_Animation_Step = 12;
};
//-------------------------------------------------------------------------------------------------------------------------
class AActive_Brick_Multihit : public AActive_Brick
{
public:
	~AActive_Brick_Multihit();
	AActive_Brick_Multihit(int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT& brick_rect, EBrick_Type brick_type);

private:
	static void Draw_Stage(HDC hdc, RECT& brick_rect, int x, int width);

	int Rotation_Step;

	static const int Steps_Per_Tern = 16;
	static const int Max_Rotation_Step = Steps_Per_Tern * 4;
};
//-------------------------------------------------------------------------------------------------------------------------
enum class ETeleport_State: unsigned char
{
	Starting,
	Finihsing,
	Done
};
//-------------------------------------------------------------------------------------------------------------------------
class AActive_Brick_Teleport : public AActive_Brick
{
public:
	~AActive_Brick_Teleport();
	AActive_Brick_Teleport(int level_x, int level_y, ABall *ball, AActive_Brick_Teleport* destination_teleport);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT& brick_rect, int step = 0);

	EDirection_Type Release_Direction;

private:
	void Set_Ball(ABall* ball);

	int Animation_Step;
	ETeleport_State Teleport_State;
	ABall *Ball;
	AActive_Brick_Teleport *Destination_Teleport;

	static const int Max_Animation_Step = 12;
};
//-------------------------------------------------------------------------------------------------------------------------
class AAdvertisement: public AGraphics_Object
{
public:
	~AAdvertisement();
	AAdvertisement(int level_x, int level_y, int width, int height);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Show_Under_Brick(int level_x, int level_y);
	bool Has_Brick_At(int level_x, int level_y);

private:
	int Level_X, Level_Y;
	int Width, Height;
	RECT Ad_Rect;
	HRGN Empty_Region;

	int Ball_X, Ball_Y;
	int Ball_Width, Ball_Height;
	int Ball_Y_Offset;
	double Falling_Speed, Acceleration_Step;
	double Deformation_Ratio;

	HRGN *Brick_Regions;

	static const int Ball_Size = 12;
	static const int High_Ball_Threshold = 12 * AsConfig::Global_Scale;
	static const int Low_Ball_Threshold = -3 * AsConfig::Global_Scale;
	static const int Deformation_Height = 2 * AsConfig::Global_Scale;
};
//-------------------------------------------------------------------------------------------------------------------------
class AActive_Brick_Ad : public AActive_Brick
{
public:
	~AActive_Brick_Ad();
	AActive_Brick_Ad(int level_x, int level_y, AAdvertisement *advertisement);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT& brick_rect);

private:
	AAdvertisement *Advertisement;

	static const int Circle_size = 7;
};
//-------------------------------------------------------------------------------------------------------------------------



