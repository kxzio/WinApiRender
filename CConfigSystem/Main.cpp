//defines
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
//includes
#include <winternl.h>
#include <vector> 
#include <sstream>
#include <time.h>
#include <iostream>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <strsafe.h>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <filesystem>

//Init filesystem 
// �������� ������� ��������� ��������� � ���������������� ���������:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HDC hDC;
RECT rect;
HBRUSH Hbrush;
int WINAPI WinMain (HINSTANCE hInst, // ���������� ���������� ����������
    HINSTANCE hPrevInst, // �� ����������
    LPSTR lpCmdLine, // �� ����������
    int nCmdShow) // ����� ����������� ������
{
    TCHAR szClassName[] = L"��� �����"; // ������ � ������ ������
    HWND hMainWnd; // ������ ���������� �������� ������
    MSG msg; // ����� ��������� ��������� MSG ��� ��������� ���������
    WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
    wc.cbSize = sizeof(wc); // ������ ��������� (� ������)
    wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
    wc.lpfnWndProc = WndProc; // ��������� �� ���������������� �������
    wc.lpszMenuName = NULL; // ��������� �� ��� ���� (� ��� ��� ���)
    wc.lpszClassName = szClassName; // ��������� �� ��� ������
    wc.cbWndExtra = NULL; // ����� ������������� ������ � ����� ���������
    wc.cbClsExtra = NULL; // ����� ������������� ������ ��� �������� ���������� ����������
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // ��������� �����������
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // ���������� ��������� ����������� (� ����)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���������� ����� ��� �������� ���� ����
    wc.hInstance = hInst; // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
    if (!RegisterClassEx(&wc)) {
        // � ������ ���������� ����������� ������:
        MessageBox(NULL, L"Class error", L"Error", MB_OK);
        return NULL; // ����������, �������������, ������� �� WinMain
    }
    // �������, ��������� ������:
    hMainWnd = CreateWindow(
        szClassName, // ��� ������
        L"Render", // ��� ������ (�� ��� ������)
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, // ������ ����������� ������
        CW_USEDEFAULT, // ������� ������ �� ��� �
        NULL, // ������� ������ �� ��� � (��� ������ � �, �� ������ �� �����)
        CW_USEDEFAULT, // ������ ������
        NULL, // ������ ������ (��� ������ � ������, �� ������ �� �����)
        (HWND)NULL, // ���������� ������������� ����
        NULL, // ���������� ����
        HINSTANCE(hInst), // ���������� ���������� ����������
        NULL); // ������ �� ������� �� WndProc
    if (!hMainWnd) {
        // � ������ ������������� �������� ������ (�������� ��������� � ��):
        MessageBox(NULL, L"Class error", L"Error", MB_OK);
        return NULL;
    }
    ShowWindow(hMainWnd, nCmdShow); // ���������� ������
    UpdateWindow(hMainWnd); // ��������� ������
    while (GetMessage(&msg, NULL, NULL, NULL)) { // ��������� ��������� �� �������, ���������� ��-�����, ��
        TranslateMessage(&msg); // �������������� ���������
        DispatchMessage(&msg); // ������� ��������� ������� ��
    }
    return msg.wParam; // ���������� ��� ������ �� ����������
}
class Vector 
{
public:
    int x;
    int y;
    Vector(int ix, int iy) { x = ix; y = iy; }
};
class Color
{
public:
    int r;
    int g;
    int b;
    Color(int _r, int _g, int _b) { r = _r; g = _g; b = _b; }
};
COLORREF ColorToCOLORREF(Color r) {
    return RGB(r.r, r.g, r.b);
}
enum PenStyles 
{ 
    SOLID = PS_SOLID, 
    DASH = PS_DASH, 
    DOT = PS_DOT, 
    DASHDOT = PS_DASHDOT, 
    DASHDOTDOT = PS_DASHDOTDOT, 
    INVISIBLE = PS_NULL, 
    INSIDEFRAME = PS_INSIDEFRAME
};
void AddRectangle(Vector pos, Vector size, Color color, int rounding = 0, int penSize = 1, int style = 0)
{
    //SetRect();
    auto pen = CreatePen(style, penSize, ColorToCOLORREF(color));
    SelectObject(hDC, pen); //������ ����� ��������
    RoundRect(hDC, pos.x, pos.y, pos.x + size.x, pos.y + size.y, rounding, rounding);

}
void AddRectangleFilled(Vector pos, Vector size, Color color)
{
    HBRUSH brush = CreateSolidBrush(ColorToCOLORREF(color));
    //CreateHatchBrush(int fnStyle, RGB(r, g, b));
    RECT myRect;
    myRect.left = pos.x; myRect.right = pos.x + size.x;
    myRect.top = pos.y; myRect.bottom = pos.y + size.y;
    //SetRect();
    Rectangle(hDC, pos.x, pos.y, pos.x + size.x , pos.y + size.y);
    FillRect(hDC, &myRect, brush);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps; 
    switch (uMsg) 
    {
    case WM_PAINT: 
    {
        hDC = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        auto hbmPB = CreateCompatibleBitmap(hDC, width, height);
        auto hdcPB = CreateCompatibleDC(hDC);
        SelectObject(hdcPB, hbmPB);
        Hbrush = CreatePatternBrush(hbmPB);
        if (true) 
        {
            //Draw filled rectangle
            AddRectangleFilled(Vector(10, 10), Vector(100, 100), Color(255, 0, 0));
            //Add rounded default rectangle
            AddRectangle(Vector(130, 10), Vector(100, 100), Color(255, 0, 0), 30, 1.f, PenStyles::SOLID);
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: 
        PostQuitMessage(NULL); 
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // ���� ������� ������
    }
    return NULL; // ���������� ��������
}
