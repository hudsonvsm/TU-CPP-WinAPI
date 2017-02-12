// minesweeper.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "minesweeper.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void newGame();
int getCell(int, int);
void drawScreen(HDC, RECT*);
bool usedCell(int, int);
int openCell(int, int);

/* GLobal variables */
HBITMAP mine;
HDC hdcBuffer;
HBITMAP hbmBuffer;

int height = 9;
int width = 9;

char levelgr[31][17];
bool leveldi[31][17];

int usecell[200][2];
int num = 0;
int screennum = 0;
int timer = 0;

int lmx, lmy;

bool md = false;
int numMines = 15;
int numLaid = 0;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINESWEEPER));

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MINESWEEPER);
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

  // hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   hWnd = CreateWindowEx(
       WS_EX_CLIENTEDGE,
       szWindowClass,
       szTitle,             //"Minesweeper",
       WS_SYSMENU,
       CW_USEDEFAULT, CW_USEDEFAULT, width * 18 + 2, height * 18 + 68,
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rcClient;

    switch (message) {
        case WM_CREATE:
            {
                newGame();

                hdcBuffer = CreateCompatibleDC(GetDC(hWnd));
                hbmBuffer = CreateCompatibleBitmap(GetDC(hWnd), 16 * 10 + 10, 16 * 10 + 20);
                SetTimer(hWnd, 1, 1000, nullptr);

                mine = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_MINE));
            }
            break;
        case WM_COMMAND:
            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId) {
                case ID_GAMES_BEGINER:
                    height = 9, width = 9, numMines = 10;
                    GetWindowRect(hWnd, &rcClient);
                    MoveWindow(hWnd, rcClient.left, rcClient.top, width * 18 + 2, height * 18 + 68, true);

                    newGame( );
                    break;
                case ID_GAMES_INTERMIDATE:
                    height = 16, width = 16, numMines = 40;

                    GetWindowRect(hWnd, &rcClient);
                    MoveWindow(hWnd, rcClient.left, rcClient.top, width * 17 + 5, height * 17 + 70, true);

                    newGame( );
                    break;
                case ID_GAMES_HARD:
                    height = 16, width = 30, numMines = 99;

                    GetWindowRect(hWnd, &rcClient);
                    MoveWindow(hWnd, rcClient.left, rcClient.top, width * 17 - 10, height * 17 + 70, true);

                    newGame( );
                    break;
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            GetClientRect(hWnd, &rcClient);

            drawScreen(hdc, &rcClient);

            EndPaint(hWnd, &ps);
            break;
        case WM_TIMER:
            {
                if (screennum == 0) timer++;

                InvalidateRect(hWnd, nullptr, false);
            }
            break;
        case WM_LBUTTONDOWN:
            if (screennum == 0) md = true;

            InvalidateRect(hWnd, nullptr, false);
            break;
        case WM_LBUTTONUP:
            {
                int	xPos = LOWORD(lParam);
                int yPos = HIWORD(lParam);

                int xpos = (xPos / 16);
                int ypos = (yPos / 16);


                if (screennum == 0) {
                    if ((xpos < width) && (ypos < height)) {
                        if (levelgr[xpos][ypos] == 'M') {
                            break;
                        }

                        if (leveldi[xpos][ypos] == false)
                            openCell(xpos, ypos);
                        else {
                            screennum = 2;
                        }
                        lmx = xpos;
                        lmy = ypos;



                        if (screennum == 0) {
                            bool t = true;
                            for (int xt = 0; xt < 10; xt++)
                                for (int yt = 0; yt < 10; yt++)
                                    if ((levelgr[xt][yt] == '?') && (leveldi[xt][yt] == false))
                                        t = false;
                            if (t == true)
                                screennum = 1;
                        }

                        num = 0;
                    }
                } else {

                }

                if ((ypos > height - 1) && (xpos > 2) && (xpos < 4)) {
                    newGame( );
                }
                md = false;

                InvalidateRect(hWnd, nullptr, true);
            }
            break;
        case WM_RBUTTONUP:
            {
                if (screennum == 0) {
                    int	xPos = LOWORD(lParam);
                    int yPos = HIWORD(lParam);

                    int xpos = (xPos / 16);
                    int ypos = (yPos / 16);

                    bool putMine = (levelgr[xpos][ypos] == '?') ? numMines > numLaid : true;
                    if ((xpos < width) && (ypos < height) && putMine) {
                        if (levelgr[xpos][ypos] == '?')
                            levelgr[xpos][ypos] = 'M', numLaid++;
                        else if (levelgr[xpos][ypos] == 'M')
                            levelgr[xpos][ypos] = '?', numLaid--;
                    }

                    bool t = true;
                    for (int xt = 0; xt < 10; xt++)
                        for (int yt = 0; yt < 10; yt++)
                            if (levelgr[xt][yt] == '?')
                                t = false;
                    if (t == true)
                        screennum = 1;

                    t = true;
                    for (int xt = 0; xt < 10; xt++)
                        for (int yt = 0; yt < 10; yt++)
                            if ((!(levelgr[xt][yt] == 'M')) && (leveldi[xt][yt] == true))
                                t = false;

                    if (t == true)
                        screennum = 1;
                }
                InvalidateRect(hWnd, nullptr, true);
            }
            break;
        case WM_DESTROY:
            KillTimer(hWnd, 1);
            DeleteDC(hdcBuffer);
            DeleteObject(hbmBuffer);
            DeleteObject(mine);

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
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return INT_PTR(TRUE);
		}
		break;
	}
	return INT_PTR(FALSE);
}



void newGame( ) {
    srand(int(time(nullptr)));

    timer = 0;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            levelgr[x][y] = '?', leveldi[x][y] = false;

    for (int i = 0; i < numMines; i++) {
        leveldi[rand( ) % width][rand( ) % height] = true;
    }

    numLaid = 0;
    screennum = 0;
}

int getCell(int xi, int yi) {
    int out = 0;

    if ((xi - 1 >= 0) && (yi - 1 >= 0)) {
        out += leveldi[xi - 1][yi - 1];
    }

    if (yi - 1 >= 0) {
        out += leveldi[xi][yi - 1];
    }

    if ((xi + 1 <= (width - 1)) && (yi - 1 >= 0)) {
        out += leveldi[xi + 1][yi - 1];
    }

    if (xi - 1 >= 0) {
        out += leveldi[xi - 1][yi];
    }

    if (xi + 1 <= (width - 1)) {
        out += leveldi[xi + 1][yi];
    }

    if ((xi - 1 >= 0) && (yi + 1 <= (height - 1))) {
        out += leveldi[xi - 1][yi + 1];
    }

    if (yi + 1 <= (height - 1)) {
        out += leveldi[xi][yi + 1];
    }

    if ((yi + 1 <= (height - 1)) && (xi + 1 <= (width - 1))) {
        out += leveldi[xi + 1][yi + 1];
    }

    return out;
}

void drawScreen(HDC hdc, RECT* prc) {
    HDC hdcMem = CreateCompatibleDC(hdc);

    SelectObject(hdcMem, mine);
    if (screennum == 0) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                if (levelgr[x][y] == '?')
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 0, 0, SRCCOPY);
                if (levelgr[x][y] == 'M')
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 16, 0, SRCCOPY);

                if ((levelgr[x][y] >= '0') && (levelgr[x][y] <= '9'))
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, (levelgr[x][y] - '0') * 16, 16, SRCCOPY);
            }
    } else if (screennum == 2) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                if (levelgr[x][y] == '?')
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 0, 0, SRCCOPY);
                if ((levelgr[x][y] >= '0') && (levelgr[x][y] <= '9'))
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, (levelgr[x][y] - '0') * 16, 16, SRCCOPY);

                if (levelgr[x][y] == 'M')
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 32, 32, SRCCOPY);

                if (leveldi[x][y] == true)
                    if (levelgr[x][y] == 'M')
                        BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 16, 0, SRCCOPY);
                    else
                        BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 0, 32, SRCCOPY);
            }
        BitBlt(hdc, lmx * 16, lmy * 16, 16, 16, hdcMem, 16, 32, SRCCOPY);
    } else if (screennum == 1) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, getCell(x, y) * 16, 16, SRCCOPY);

                if (leveldi[x][y] == true)
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 16, 0, SRCCOPY);

            }
    }

    switch (screennum) {
        case 0:
        case 1:
            BitBlt(hdc, 13 * 3, 16 * height, 25, 23, hdcMem, 0, 23 + 16 * 3, SRCCOPY);
            break;
        case 2:
            BitBlt(hdc, 13 * 3, 16 * height, 25, 23, hdcMem, 25, 23 + 16 * 3, SRCCOPY);
            break;
    }


    auto mineCount = numMines - numLaid;

    BitBlt(hdc, 0, 16 * height, 13, 26, hdcMem, (mineCount / 100) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13, 16 * height, 13, 26, hdcMem, (mineCount % 100 / 10) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 26, 16 * height, 13, 26, hdcMem, (mineCount % 100 % 10) * 13, 16 * 3, SRCCOPY);

    BitBlt(hdc, 13 * 5 - 2, 16 * height, 13, 26, hdcMem, (timer / 100) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13 * 6 - 2, 16 * height, 13, 26, hdcMem, (timer % 100 / 10) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13 * 7 - 2, 16 * height, 13, 26, hdcMem, (timer % 100 % 10) * 13, 16 * 3, SRCCOPY);
    if (md == true) {
        BitBlt(hdc, 13 * 3, 16 * height, 25, 23, hdcMem, 50, 23 + 16 * 3, SRCCOPY);
    }

    DeleteDC(hdcMem);
}

bool usedCell(int xi, int yi) {
    for (int i = 0; i < num; i++) {
        if ((usecell[i][0] == xi) && (usecell[i][1] == yi)) {
            return true;
        }
    }

    return false;
}

int openCell(int xi, int yi) {
    if (usedCell(xi, yi)) return 0;

    usecell[num][0] = xi;
    usecell[num][1] = yi;

    if (levelgr[xi][yi] == 'M') {
        numLaid--;
    }

    num++;
    auto temp = getCell(xi, yi);

    levelgr[xi][yi] = '0' + temp;

    if (temp == 0) {
        if ((yi - 1 >= 0) && (xi - 1 >= 0) && (yi - 1 <= (height - 1)) && (xi - 1 <= (width - 1)))
            openCell(xi - 1, yi - 1);

        if ((yi + 1 >= 0) && (xi - 1 >= 0) && (yi + 1 <= (height - 1)) && (xi - 1 <= (width - 1)))
            openCell(xi - 1, yi + 1);

        if ((yi - 1 >= 0) && (xi + 1 >= 0) && (yi - 1 <= (height - 1)) && (xi + 1 <= (width - 1)))
            openCell(xi + 1, yi - 1);

        if ((yi + 1 >= 0) && (xi + 1 >= 0) && (yi + 1 <= (height - 1)) && (xi + 1 <= (width - 1)))
            openCell(xi + 1, yi + 1);

        if (yi - 1 >= 0) openCell(xi, yi - 1);

        if (xi - 1 >= 0) openCell(xi - 1, yi);

        if (xi + 1 <= (width - 1)) openCell(xi + 1, yi);

        if (yi + 1 <= (height - 1)) openCell(xi, yi + 1);
    }

    return 0;
}

