// Popcorn.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

// AsFrame_DC
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::~AsFrame_DC()
{
	if (Bitmap != 0)
		DeleteObject(Bitmap);

	if (DC != 0)
		DeleteObject(DC);
}
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::AsFrame_DC()
	: Width(0), Height(0), DC(0), Bitmap(0)
{
}
//------------------------------------------------------------------------------------------------------------
HDC AsFrame_DC::Get_DC(HWND hWnd, HDC hdc)
{
	int dc_width, dc_height;
	RECT rect;

	GetClientRect(hWnd, &rect);

	dc_width = rect.right - rect.left;
	dc_height = rect.bottom - rect.top;

	if (dc_width != Width && dc_height != Height)
	{
		if (Bitmap != 0)
			DeleteObject(Bitmap);

		if (DC != 0)
			DeleteObject(DC);

		Width = dc_width;
		Height = dc_height;

		DC = CreateCompatibleDC(hdc);
		Bitmap = CreateCompatibleBitmap(hdc, Width, Height);
		SelectObject(DC, Bitmap);

		AsTools::Rect(DC, rect, AsConfig::BG_Color);
	}
	return DC;
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
// Global Variables
AsMain_Window Main_Window;
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return Main_Window.Main(hInstance, nCmdShow);
}




// AsMain_Window
AsMain_Window *AsMain_Window::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsMain_Window::AsMain_Window()
	: Instance(0), szTitle{}, szWindowClass{}
{
	Self = this;
}
//------------------------------------------------------------------------------------------------------------
int APIENTRY AsMain_Window::Main(HINSTANCE instance, int command_show)
{
	MSG msg;
	HACCEL accel_table;
	Instance = instance; // Store instance handle in our global variable

	// Initialize global strings
	LoadStringW(Instance, IDS_APP_TITLE, szTitle, Max_String_Size);
	LoadStringW(Instance, IDC_POPCORN, szWindowClass, Max_String_Size);
	Register_Class();

	// Perform application initialization:
	if (!Init_Instance(command_show))
		return FALSE;

	accel_table = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_POPCORN));

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
//-------------------------------------------------------------------------------------------------------------------------
ATOM AsMain_Window::Register_Class()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window_Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_POPCORN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(Instance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//-------------------------------------------------------------------------------------------------------------------------
BOOL AsMain_Window::Init_Instance(int command_show)
{
	HWND hWnd;
	RECT window_rect;

	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * 3;
	window_rect.bottom = 200 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 0, 0, Instance, 0);

	if (hWnd == 0)
		return FALSE;

	Engine.Init_Engine(hWnd);

	ShowWindow(hWnd, command_show);
	UpdateWindow(hWnd);

	return TRUE;
}
//-------------------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc, frame_dc;

	hdc = BeginPaint(hWnd, &ps);

	frame_dc = Frame_DC.Get_DC(hWnd, hdc);

	Engine.Draw_Frame(frame_dc, ps.rcPaint);

	BitBlt(hdc, 0, 0, Frame_DC.Width, Frame_DC.Height, frame_dc, 0, 0, SRCCOPY);

	EndPaint(hWnd, &ps);
}
//-------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK AsMain_Window::Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(Self->Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;


	case WM_PAINT:
		Self->On_Paint(hWnd);
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, true);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, true);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, true);
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, false);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, false);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, false);
		}
		break;


	case WM_TIMER:
		if (wParam == Timer_ID)
			return Self->Engine.On_Timer();
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK AsMain_Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------------------------------
