#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"

CONST CHAR g_sz_filename[] = "list.txt";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param);
BOOL CALLBACK DlgProcADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param);
BOOL CALLBACK DlgProcEDIT(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param);

VOID SaveList(HWND hwnd, CONST CHAR fileName[]);
VOID LoadList(HWND hwnd, CONST CHAR fileName[]);

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
		LoadList(hwnd, g_sz_filename);
	}
	break;

	case WM_COMMAND:
	{


		switch (LOWORD(wParam))
		{

		case IDC_LIST:
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcEDIT, (LPARAM)SendDlgItemMessage(hwnd, IDC_LIST, LB_GETCURSEL, 0, 0));
			}
		}

		case IDOK:
			break;

		case IDCANCEL:
			SaveList(hwnd, g_sz_filename);
			EndDialog(hwnd, 0);
			break;

		case IDC_ADD_BUTTON:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcADD, 0);
		}
		break;

		case IDC_DELETE_BUTTON:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
		}
		break;

		}
	}
	break;
	
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_RETURN:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			if (GetFocus() == hList)
			{
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcEDIT, 0);
			}
		}
		}
	}
		break;

	case WM_KEYDOWN:
		break;



	case WM_CLOSE:
		SaveList(hwnd, g_sz_filename);
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
			HWND hList = GetDlgItem(hParent, IDC_LIST);

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

BOOL CALLBACK DlgProcEDIT(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM Param)
{
	CONST INT SIZE = 256;
	CHAR sz_buffer[SIZE] = {};
	HWND hList = GetDlgItem(GetParent(hwnd), IDC_LIST);
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hwnd, IDC_EDIT));
		SendMessage(hList, LB_GETTEXT, Param, (LPARAM)sz_buffer);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Изменение!");
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);

		SendMessage(hEdit, EM_SETSEL, strlen(sz_buffer), -1);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (SendMessage(hList, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
			{
				SendMessage(hList, LB_DELETESTRING, i, 0);
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
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

VOID SaveList(HWND hwnd, CONST CHAR fileName[])
{
	CONST INT SIZE = 32768;
	CHAR sz_buffer[SIZE] = {};

	HWND hList = GetDlgItem(hwnd, IDC_LIST);
	INT n = SendMessage(hList, LB_GETCOUNT, 0, 0);

	for (int i = 0; i < n; i++)
	{
		CHAR sz_item[256] = {};
		SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_item);
		lstrcat(sz_buffer, sz_item);
		lstrcat(sz_buffer, "\n");
	}

	HANDLE hFile = CreateFile(
		fileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0);

	DWORD dwByresWritten = 0;
	WriteFile(hFile, sz_buffer, strlen(sz_buffer)+1, &dwByresWritten, NULL);
	CloseHandle(hFile);
}

VOID LoadList(HWND hwnd, CONST CHAR fileName[])
{
	

	HANDLE hFile = CreateFile
	(
		fileName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	DWORD dwError = GetLastError();

	if (dwError == ERROR_FILE_NOT_FOUND) return;

	CONST INT SIZE = 32768;
	CHAR sz_buffer[SIZE] = {};

	DWORD dwBytesRead = 0;

	ReadFile(hFile, sz_buffer, SIZE, &dwBytesRead, NULL);

	HWND hList = GetDlgItem(hwnd, IDC_LIST);

	for (char* pch = strtok(sz_buffer, "\n"); pch;pch = strtok(NULL, "\n"))
	{
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pch);
	}
	CloseHandle(hFile);
}
