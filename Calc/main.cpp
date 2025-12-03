#include <Windows.h>
#include "resource1.h"


CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_521";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 2;

CONST INT g_i_DOUBLE_BUTTON_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = 22;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL * 3;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 64;

#define X_BUTTON_POSITION(position) g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (position)
#define Y_BUTTON_POSITION(position) g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (position)

CONST CHAR g_OPERATIONS[] = "+-*/";


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//Регистрация класса окна:
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL, //exStyle
		g_sz_WINDOW_CLASS, //Имя класса окна
		g_sz_WINDOW_CLASS, //заголовок окна
		WS_OVERLAPPEDWINDOW ^  WS_MAXIMIZEBOX  ^ WS_THICKFRAME, //Стиль окна. Стили всегда зависят от класса окна. "WS_OVERLAPPEDWINDOW" - главное окно
		CW_USEDEFAULT, CW_USEDEFAULT, //Position
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT, //Размер окна
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
		CreateWindowEx
		(
			NULL,
			"Edit",
			"0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT ,
			10, 10,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)999,
			GetModuleHandle(NULL),
			NULL
		);
		CHAR sz_button[2] = {};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_button[0] = i + j + '1';
				CreateWindowEx
				(
					NULL, "Button", sz_button,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					X_BUTTON_POSITION(j),
					Y_BUTTON_POSITION(2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			X_BUTTON_POSITION(0),
			Y_BUTTON_POSITION(3),
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_0),
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			X_BUTTON_POSITION(2),
			Y_BUTTON_POSITION(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_POINT),
			GetModuleHandle(NULL),
			NULL
		);
		CHAR sz_operation[2] = "";
		for (int i = 0; i < 4; i++)
		{
			sz_operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				X_BUTTON_POSITION(3),
				Y_BUTTON_POSITION(3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		
			CreateWindowEx
			(
				NULL, "Button", "<-",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				X_BUTTON_POSITION(4),
				Y_BUTTON_POSITION(0),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_BSP),
				GetModuleHandle(NULL),
				NULL
			);
			CreateWindowEx
			(
				NULL, "Button", "CLR",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				X_BUTTON_POSITION(4),
				Y_BUTTON_POSITION(1),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_CLR),
				GetModuleHandle(NULL),
				NULL
			);
			CreateWindowEx
			(
				NULL, "Button", "=",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				X_BUTTON_POSITION(4),
				Y_BUTTON_POSITION(2),
				g_i_BUTTON_SIZE, g_i_DOUBLE_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_EQUAL),
				GetModuleHandle(NULL),
				NULL
			);
		
	}


		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_0:
			break;
		case IDC_BUTTON_1:
			break;
		case IDC_BUTTON_2:
			break;
		case IDC_BUTTON_3:
			break;
		case IDC_BUTTON_4:
			break;
		case IDC_BUTTON_5:
			break;
		case IDC_BUTTON_6:
			break;
		case IDC_BUTTON_7:
			break;
		case IDC_BUTTON_8:
			break;
		case IDC_BUTTON_9:
			break;
		case IDC_BUTTON_POINT:
			break;
		case IDC_BUTTON_PLUS:
			break;
		case IDC_BUTTON_MINUS:
			break;
		case IDC_BUTTON_ASTER:
			break;
		case IDC_BUTTON_SLASH:
			break;
		case IDC_BUTTON_BSP:
			break;
		case IDC_BUTTON_CLR:
			break;
		case IDC_BUTTON_EQUAL:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return FALSE;
}