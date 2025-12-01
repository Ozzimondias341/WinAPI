#include <Windows.h>
//#include "resourse.h"


CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_521";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);





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
		WS_OVERLAPPEDWINDOW, //Стиль окна. Стили всегда зависят от класса окна. "WS_OVERLAPPEDWINDOW" - главное окно
		CW_USEDEFAULT, CW_USEDEFAULT, //Position
		CW_USEDEFAULT, CW_USEDEFAULT, //Размер окна
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
			250, 22,
			hwnd,
			(HMENU)999,
			GetModuleHandle(NULL),
			NULL
		);
	}
		break;

	case WM_COMMAND:
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