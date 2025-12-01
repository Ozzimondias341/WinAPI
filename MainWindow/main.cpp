#include <Windows.h>
#include "resource.h"
#include <cstdio>

#define IDC_BUTTON 1000

CONST CHAR g_sz_WINDOW_CLASS[] = "My first window";

LRESULT CALLBACK WndProc(HWND hwnd, UINT, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmbLine, INT nCmdShow)
{

	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	//Инициализируем размеры и стиль
	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass); //cb_ - Count Bytes
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	//Инициализируем внешний вид окон
	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_BITCOIN));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PALM));
	/*wClass.hIcon = (HICON)LoadImage(NULL, "bitcoin.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(NULL, "palm.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);*/


	//wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wClass.hCursor = (HCURSOR)LoadImage(hInstance, 
		"deltarune-lancer\\lancer working in background.ani",
		IMAGE_CURSOR,
		LR_DEFAULTSIZE, LR_DEFAULTSIZE,
		LR_LOADFROMFILE
		);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	//Инициализация системных переменных
	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "", MB_OK | MB_ICONERROR);
			return 0;
	}

	//Создание окна


	INT screenW = GetSystemMetrics(SM_CXSCREEN);
	INT screenH = GetSystemMetrics(SM_CYSCREEN);

	INT winW = screenW * 3 / 4;
	INT winH = screenH * 3 / 4;

	INT posX = screenW / 8;
	INT posY = screenH / 8;


	HWND hwnd = CreateWindowEx
	(
		NULL, //exStyle
		g_sz_WINDOW_CLASS, //Имя класса окна
		g_sz_WINDOW_CLASS, //заголовок окна
		WS_OVERLAPPEDWINDOW, //Стиль окна. Стили всегда зависят от класса окна. "WS_OVERLAPPEDWINDOW" - главное окно
		posX, posY, //Position
		winW, winH, //Размер окна
		NULL,
		NULL, //Для главного окна это ResourseID главного меню, для дочернего (Control) - ResourseID дочернего окна(IDC_BUTTON_COPY)
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Windows creation failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}


	ShowWindow(hwnd, nCmdShow); //Задаёт режим отображения окна - развёрнуто на весь экран | свёрнуто в окно | свёрнуто на панель задач.
	UpdateWindow(hwnd); //Обновляет рабочую область окна отправляя сообщение WM_PAINT, если клиентская область окна не пустая




	//Запуск цикла сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg); //Преобразует сообщения виртуальных клавиш в символьные сообщения
		DispatchMessage(&msg);
	}



	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hwnd = CreateWindowEx
		(
			NULL,
			"Button",
			"Кнопка",
			WS_CHILD | WS_VISIBLE,
			10, 10,
			150, 80,
			hwnd,
			(HMENU)1000, //Для главного окна - это ResourseID главного меню,
			//Для дочернего окна(элемент управления окна) - это ResourseID дочернего элемента
			GetModuleHandle(NULL),
			NULL
		);
	}
		break;

	case WM_MOVE:

	case WM_SIZE:
	{
		RECT window_rect = {};
		GetWindowRect(hwnd, &window_rect);
		CONST INT SIZE = 1024;
		CHAR sz_title[SIZE] = {};
		wsprintf
		(
			sz_title,
			"%s, Position: %ix%i Size: %ix%i",
			g_sz_WINDOW_CLASS,
			window_rect.left, window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
	break;

	case WM_COMMAND:

		switch(LOWORD(wParam))
		{
		case IDC_BUTTON:
			MessageBox(hwnd, "Cursor check", "info", MB_OK | MB_ICONINFORMATION);
			break;
		}
		

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
return FALSE;
}