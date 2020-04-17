// Tic Tac Toe UI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Tic Tac Toe UI.h"
#include "Net.h"
#include <string>
#include "TTTGame.h"
#define MAX_LOADSTRING 100
bool bDrawBoard = false;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
enum pMarks { pBlank, pX, pO };
int startGame = 0;
TTTgame tictactoe;
pMarks tttgmBoard[3][3] = { {pBlank, pBlank, pBlank}, {pBlank, pBlank, pBlank}, {pBlank, pBlank, pBlank} };
HRGN tttSpaces[3][3];
char moves[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
char* boardvia;
bool playing = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DrawX(HDC hdc, int x1, int y1, int x2, int y2, int width)
{
    HPEN hPenOld;
    HPEN hLinePen;
    COLORREF xColor = RGB(225, 50, 50);
    hLinePen = CreatePen(PS_SOLID, width, xColor);
    hPenOld = (HPEN)SelectObject(hdc, hLinePen);
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    MoveToEx(hdc, x1, y2, NULL);
    LineTo(hdc, x2, y1);
    SelectObject(hdc, hPenOld);
    DeleteObject(hLinePen);
}

void DrawO(HDC hdc, int x1, int y1, int x2, int y2, int width)
{
    HPEN hPenOld;
    HPEN hLinePen;
    COLORREF xColor = RGB(50, 50, 225);
    hLinePen = CreatePen(PS_SOLID, width, xColor);
    hPenOld = (HPEN)SelectObject(hdc, hLinePen);
    Arc(hdc, x1, y1, x2, y2, 0, 0, 0, 0);
    SelectObject(hdc, hPenOld);
    DeleteObject(hLinePen);

}

void RunNN()
{

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.



    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TICTACTOEUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOEUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOEUI));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TICTACTOEUI);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
    hInst = hInstance; // Store instance handle in our global variable
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_START:
            bDrawBoard = true;
            InvalidateRect(hWnd, 0, true);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    tttgmBoard[i][j] = pBlank;
            startGame = 2;
            playing = true;
            tictactoe = TTTgame(startGame);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_PRINT:
            PrintWindow(FindWindow(TEXT("Tic Tac Toe"), TEXT("xxx.txt - Tic Tac Toe")), GetDC(hWnd), 0);
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        if (playing)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (PtInRegion(tttSpaces[i][j], x, y))
                    {
                        tictactoe.TTTmove(moves[i][j]);
                        boardvia = tictactoe.TTTupdate();
                        tttgmBoard[i][j] = boardvia[j + 3 * i] == 'X' ? pX : boardvia[j + 3 * i] == 'O' ? pO : pBlank;
                        InvalidateRect(hWnd, 0, true);
                    }
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        HPEN hPenOld;

        if (bDrawBoard)
        {
            RECT wndw;
            HPEN hLinePen;
            GetWindowRect(hWnd, &wndw);
            int xMax = wndw.right - wndw.left - 18;
            int yMax = wndw.bottom - wndw.top - 59;
            int width = xMax > yMax ? (yMax / 10 < 5 ? 5 : (yMax / 20 > 20 ? 20 : yMax / 20)) : (xMax / 10 < 5 ? 5 : (xMax / 20 > 20 ? 20 : xMax / 20));
            COLORREF bColor = RGB(0, 0, 0);
            hLinePen = CreatePen(PS_SOLID, width, bColor);
            hPenOld = (HPEN)SelectObject(hdc, hLinePen);
            for (int i = 1; i < 3; i++)
            {
                MoveToEx(hdc, i * xMax / 3, yMax, NULL);
                LineTo(hdc, i * xMax / 3, yMax - yMax);
                MoveToEx(hdc, xMax - xMax, i * yMax / 3, NULL);
                LineTo(hdc, xMax, i * yMax / 3);
            }
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    if (tttgmBoard[i][j] == pX)
                        DrawX(hdc, j * xMax / 3, i * yMax / 3, (j + 1) * xMax / 3, (i + 1) * yMax / 3, width);
                    else if (tttgmBoard[i][j] == pO)
                        DrawO(hdc, j * xMax / 3, i * yMax / 3, (j + 1) * xMax / 3, (i + 1) * yMax / 3, width);
                    tttSpaces[i][j] = CreateRectRgn(j * xMax / 3, i * yMax / 3, (j + 1) * xMax / 3, (i + 1) * yMax / 3);
                }
            SelectObject(hdc, hPenOld);
            DeleteObject(hLinePen);
            if (tictactoe.TTTwinstatus())
            {
                wchar_t player[20];
                wsprintf(player, L"%s", tictactoe.TTTplayer_current == 'X' ? L"X's Win!" : L"O's Win!");
                ::MessageBox(hWnd, player, L"WINNER!!!", MB_OK);
                playing = false;
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;
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
};

INT_PTR CALLBACK Sizes(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
};