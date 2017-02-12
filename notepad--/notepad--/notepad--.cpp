// notepad--.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <Commdlg.h>

#include "notepad--.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

/*Declare Variables*/
HWND g_hwnd;
HWND g_hEdit;
HFONT g_hFont;
COLORREF g_editcolor = RGB(0, 0, 0);
HBRUSH g_hbrbackground = CreateSolidBrush(RGB(255, 255, 255));

char curfile[MAX_PATH];
TCHAR searchParam[MAX_LOADSTRING];

bool findUp = true;
bool caseSensitive = false;
bool isopened = false;
bool needsave = false;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Find(HWND, UINT, WPARAM, LPARAM);

BOOL find();
void LoadFileToEdit();
void SaveTextFileFromEdit();
bool GetFileNameForSave();
void checksave();
void ChooseFontForEdit();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NOTEPAD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTEPAD));

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTEPAD));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NOTEPAD);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);       

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
        case WM_CREATE:
        {
            HWND hEdit;
            HFONT hFont;
            //For Edit Control...
/*            hEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE,                   /* Extended p ossibilites for variation #1#
                "EDIT",         /* Classname #1#
                "",       /* Title Text #1#
                WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL, /* default window #1#
                1,       /* Windows decides the position #1#
                1,       /* where the window ends up on the screen #1#
                100,                 /* The programs width #1#
                100,                 /* and height in pixels #1#
                hWnd,        /* The window is a child-window to hwnd #1#
                HMENU(IDC_EDIT),                     /* No menu #1#
                hInst,       /* Program Instance handler #1#
                nullptr                 /* No Window Creation data #1#
                );*/

            hEdit = CreateWindow(
                "EDIT",         /* Classname */
                nullptr,       /* Title Text */
                WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL, /* default window */
                1,       /* Windows decides the position */
                1,       /* where the window ends up on the screen */
                100,                 /* The programs width */
                100,                 /* and height in pixels */
                hWnd,        /* The window is a child-window to hwnd */
                HMENU(IDC_EDIT),                     /* No menu */
                hInst,       /* Program Instance handler */
                nullptr                 /* No Window Creation data */
                );

            if (hEdit == nullptr) {
                MessageBox(g_hwnd, "Could not Create Edit control!!", "Error", MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
            }

            hFont = HFONT(GetStockObject(DEFAULT_GUI_FONT));
            SendMessage(hEdit, WM_SETFONT, WPARAM(hFont), MAKELPARAM(FALSE, 0));

            g_hEdit = hEdit;
            g_hFont = hFont;

            RECT rcClient;
            GetClientRect(g_hwnd, &rcClient);
            SetWindowPos(g_hEdit, nullptr, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
        }
        break;
        case WM_SIZE:
            {
                RECT rcClient;
                GetClientRect(g_hwnd, &rcClient);
                SetWindowPos(g_hEdit, nullptr, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
            }
	    case WM_COMMAND:
		    wmEvent = HIWORD(wParam);

		    // Parse the menu selections:
            switch (LOWORD(wParam))
		    {
		    case IDM_ABOUT:
			    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			    break;
            case ID_FILE_NEW:
                checksave();
                isopened = false;
                needsave = false;
                SetWindowText(g_hEdit, "");
                SetWindowText(g_hwnd, "notepad-- 1.0 [Untitled]");
                break;
            case ID_FILE_OPEN:
                checksave();
                LoadFileToEdit();
                break;
            case ID_FILE_SAVE:
                if (needsave) {
                    if (isopened)
                        SaveTextFileFromEdit();
                    else if (GetFileNameForSave())
                        SaveTextFileFromEdit();
                }
                break;
            case ID_FILE_SAVEAS:
                if (GetFileNameForSave()) {
                    SaveTextFileFromEdit();
                }
                break;
            case ID_EDIT_COPY:
                SendMessage(g_hEdit, WM_COPY, 0, 0);
                break;
            case ID_EDIT_CUT:
                SendMessage(g_hEdit, WM_CUT, 0, 0);
                break;
            case ID_EDIT_PASTE:
                SendMessage(g_hEdit, WM_PASTE, 0, 0);
                break;
            case ID_EDIT_SELECT_ALL:
                SendMessage(g_hEdit, EM_SETSEL, 0, -1);
                break;
            case ID_EDIT_DELETE:
                SendMessage(g_hEdit, WM_CLEAR, 0, 0);
                break;
            case ID_EDIT_UNDO:
                SendMessage(g_hEdit, WM_UNDO, 0, 0);
                break;
            case ID_EDIT_FIND:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND), g_hEdit, Find);
                break;
            case ID_EDIT_FINDNEXT:
                if (searchParam == nullptr) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND), g_hEdit, Find);
                } else {
                    find();
                }
                break;
            case ID_FORMAT_FONT:
                ChooseFontForEdit();
                break;
            case IDM_EXIT:
                {
                    if (needsave) {
                        int res;
                        res = MessageBox(g_hwnd, "The File has been changed!!!\nDo you want to save it before exit?", "Save File before Existing!!", MB_YESNOCANCEL|MB_ICONINFORMATION);

                        if (res == IDCANCEL) return 0;

                        if (res == IDYES && GetFileNameForSave()) {
                            SaveTextFileFromEdit();
                        }
                    }
                    if (MessageBox(g_hwnd, "Are you sure you want to exit!!!", "Sure Exit?", MB_YESNO|MB_ICONQUESTION) == IDNO) {
                        return 0;
                    }
			        DestroyWindow(hWnd);
                }
			    break;
            case IDC_EDIT:
                {
                    /// simply not working
                    if (GetAsyncKeyState(VK_CONTROL)& 0x8000 && GetAsyncKeyState('A')& 0x8000) {
                        SendMessage(g_hEdit, EM_SETSEL, 0, -1);
                    }

                    if (HIWORD(wParam) == EN_CHANGE) {
                        needsave = true;
                    }
                }
                break;
		    default:
			    return DefWindowProc(hWnd, message, wParam, lParam);
		    }
		    break;
	    case WM_PAINT:
		    hdc = BeginPaint(hWnd, &ps);
		    // TODO: Add any drawing code here...
		    EndPaint(hWnd, &ps);
		    break;
/*        case WM_KEYDOWN:
            if (GetAsyncKeyState(VK_CONTROL)&0x8000) {
                MessageBox(g_hEdit, "CTRL+A", "Sure Exit?", MB_OK);
                SendMessage(g_hEdit, EM_SETSEL, 0, -1);
            }
            break;*/
        case WM_DESTROY:
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return INT_PTR(TRUE);

	case WM_COMMAND:
        if (HIWORD(wParam) == EN_CHANGE) {
            needsave = true;
        }

        switch (LOWORD(wParam))
        {
            case ID_EDIT_SELECT_ALL:
                MessageBox(g_hEdit, "Are you sure you want to exit!!!", "Sure Exit?", MB_OK);
                SendMessage(g_hEdit, EM_SETSEL, 0, -1);
                break;
        }


		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return INT_PTR(TRUE);
		}
		break;
	}
	return INT_PTR(FALSE);
}

// Message handler for about box.
INT_PTR CALLBACK Find(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            CheckDlgButton(hDlg, IDC_FIND_UP, BST_CHECKED);

            searchParam[0] = '\0';

            findUp = true;
            caseSensitive = false;

            return INT_PTR(TRUE);

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_FIND_NEXT:
                    if (searchParam == nullptr) {
                        GetDlgItemText(hDlg, IDC_FIND_STR, searchParam, MAX_LOADSTRING);
                    }

                    find();
                    break;
                case IDC_FIND_UP:
                    findUp = true;
                    break;
                case IDC_FIND_DOWN:
                    findUp = false;
                    break;
                case IDC_CASE_SENSITIVE:
                    caseSensitive = !caseSensitive;
                    break;
            }

            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return INT_PTR(TRUE);
            }
            break;
    }
    return INT_PTR(FALSE);
}

/*
LRESULT CALLBACK TextEditor(HWND hEdit, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_KEYDOWN:
            if (GetKeyState(VK_CONTROL)&0x8000&&wParam=='a') {
                SendMessage(hEdit, EM_SETSEL, 0, -1);
            }
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                //EndDialog(hDlg, LOWORD(wParam));
                return INT_PTR(TRUE);
            }
            break;
    }
    return CallWindowProc(nullptr, hEdit, message, wParam, lParam);
}
*/

BOOL find() {
    if (searchParam == nullptr) {
        return FALSE;
    }

    MSG msg;
    UINT MessageID;
    HWND regFt;
    HWND hdlgFt;

    FINDREPLACE* lpfr;
    WPARAM wParam;
    LPARAM lParam = 0;

    FINDREPLACE fr;

    memset(&fr, 0, sizeof(fr));

    //memset(szFindWhat, 0, sizeof(szFindWhat));

    fr.Flags = FR_NOWHOLEWORD|FR_NOUPDOWN;
    fr.lStructSize = sizeof(fr);
    fr.hwndOwner = g_hEdit;
    fr.lpstrFindWhat = searchParam;
    fr.wFindWhatLen = sizeof(searchParam);


    MessageID = RegisterWindowMessage(FINDMSGSTRING);
    wParam = 0;

    lpfr = LPFINDREPLACE(lParam);


    hdlgFt = FindText(&fr);

    //      while (GetMessage(&msg, NULL, 0, 0)) 


    //       if(!IsDialogMessage(hdlgFt, &msg))
    //               SendMessage(hRichEdit, WM_SETTEXT, 0, (LPARAM)lpfr->lpstrFindWhat);

    //       DispatchMessage(&msg);

    return TRUE;
}

void LoadFileToEdit() {
    curfile[0] = '\0';
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = g_hwnd;
    ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0All File(*.*)\0*.*\0";
    ofn.lpstrFile = curfile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

    if (!GetOpenFileName(&ofn)) return;

    HANDLE hFile;
    DWORD dwFileSize;
    DWORD dwRead;
    LPSTR tempftext;
    bool bsucces = false;

    hFile = CreateFile(curfile, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

    if (hFile!=INVALID_HANDLE_VALUE) {
        dwFileSize = GetFileSize(hFile, nullptr);
        if (dwFileSize != 0xFFFFFFFF) {
            tempftext = (char*)GlobalAlloc(GPTR, dwFileSize+1);
            if (tempftext!=nullptr) {
                if (ReadFile(hFile, tempftext, dwFileSize, &dwRead, nullptr)) {
                    tempftext[dwFileSize] = 0;
                    if (SetWindowText(g_hEdit, tempftext))
                        bsucces = true;
                }
                GlobalFree(tempftext);
            }
        }
        CloseHandle(hFile);
    }
    if (!bsucces) {
        MessageBox(g_hwnd, "The File could not be loaded!!", "Error", MB_OK|MB_ICONERROR);
        return;
    }
    SetWindowText(g_hwnd, curfile);
    needsave = false;
    isopened = true;
}

void SaveTextFileFromEdit() {
    HANDLE hFile;
    bool bsucces = false;

    hFile = CreateFile(curfile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwTextLength;
        DWORD dwWritten;
        LPSTR pszText;

        dwTextLength = GetWindowTextLength(g_hEdit);

        DWORD dwBufferSize = dwTextLength+1;

        pszText = (char*)GlobalAlloc(GPTR, dwBufferSize);

        if (pszText != nullptr) {
            if (GetWindowText(g_hEdit, pszText, dwBufferSize)
                &&WriteFile(hFile, pszText, dwTextLength, &dwWritten, nullptr)) {
                bsucces = true;
            }
            GlobalFree(pszText);
        }
        CloseHandle(hFile);
    }

    if (!bsucces) {
        MessageBox(g_hwnd, "The File could not be saved!!!", "Error", MB_OK|MB_ICONERROR);
        return;
    }

    isopened = true;
    needsave = false;
}

bool GetFileNameForSave() {
    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = g_hwnd;
    ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0All File(*.*)\0*.*\0";
    ofn.lpstrFile = curfile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "txt";

    return !GetSaveFileName(&ofn) ? false : true;
}

void checksave() {
    if (needsave) {
        int res;
        res = MessageBox(g_hwnd, "The File has been changed!!!\nDo you want to save it before continueing?", "Save File before continueing!!", MB_YESNOCANCEL|MB_ICONINFORMATION);

        if (res == IDCANCEL) return;

        if (GetFileNameForSave())
            SaveTextFileFromEdit();
    }
}

void ChooseFontForEdit() {
    CHOOSEFONT cf = { sizeof(CHOOSEFONT) };
    LOGFONT lf;
    GetObject(g_hFont, sizeof(LOGFONT), &lf);
    cf.Flags = CF_EFFECTS|CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT;
    cf.hwndOwner = g_hwnd;
    cf.lpLogFont = &lf;
    cf.rgbColors = g_editcolor;
    if (!ChooseFont(&cf))
        return;
    HFONT hf = CreateFontIndirect(&lf);
    if (hf) {
        g_hFont = hf;
        SendMessage(g_hEdit, WM_SETFONT, WPARAM(g_hFont), TRUE);
    }
    g_editcolor = cf.rgbColors;
}