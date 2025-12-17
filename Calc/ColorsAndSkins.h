#pragma once
#include <Windows.h>

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_521";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hwnd, CONST CHAR skin[]);
VOID SetSkinDLL(HWND hwnd, CONST CHAR skin[]);

VOID SetFontDLL(HWND hwnd, CONST CHAR font[]);

CONST INT g_i_WINDOW_COLOR = 0;
CONST INT g_i_DISPLAY_COLOR = 1;
CONST INT g_i_FONT_COLOR = 2;
CONST COLORREF g_clr_COLORS[][3] =
{
	{RGB(7,127,164), RGB(0,0,100), RGB(255,0,0)},
	{RGB(150,150,150),RGB(50,50,50), RGB(0,255,0)},
	{ RGB(220, 20, 60),RGB(0, 139, 139), RGB(255, 0, 0) }
};
CONST CHAR* g_sz_SKIN[] = { "square_blue", "metal_mistral", "my_skin"};

CONST CHAR* g_sz_FONT[] = { "Digital-7", "28 Days Later" };