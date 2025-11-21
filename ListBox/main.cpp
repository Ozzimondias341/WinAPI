//#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is", "my", "first", "List", "box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param);
BOOL CALLBACK DlgProcADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, 0);


	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		{
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		}
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		case IDC_ADD_BUTTON:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcADD, 0);
		}
			break;
		case IDC_DELETE_BUTTON:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
		}
		break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}


BOOL CALLBACK DlgProcADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hwnd, IDC_EDIT));
	}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			if (SendMessage(hList, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			else
			{
				MessageBox(hwnd, "Такой элемент уже существует", "Warning", MB_OK | MB_ICONWARNING);
				break;
			}
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
