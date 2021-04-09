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
// Прототип функции обработки сообщений с пользовательским названием:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HDC hDC;
RECT rect;
HBRUSH Hbrush;
int WINAPI WinMain (HINSTANCE hInst, // дескриптор экземпляра приложения
    HINSTANCE hPrevInst, // не используем
    LPSTR lpCmdLine, // не используем
    int nCmdShow) // режим отображения окошка
{
    TCHAR szClassName[] = L"Мой класс"; // строка с именем класса
    HWND hMainWnd; // создаём дескриптор будущего окошка
    MSG msg; // создём экземпляр структуры MSG для обработки сообщений
    WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
    wc.cbSize = sizeof(wc); // размер структуры (в байтах)
    wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
    wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
    wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
    wc.lpszClassName = szClassName; // указатель на имя класса
    wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
    wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
    wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
    if (!RegisterClassEx(&wc)) {
        // в случае отсутствия регистрации класса:
        MessageBox(NULL, L"Class error", L"Error", MB_OK);
        return NULL; // возвращаем, следовательно, выходим из WinMain
    }
    // Функция, создающая окошко:
    hMainWnd = CreateWindow(
        szClassName, // имя класса
        L"Render", // имя окошка (то что сверху)
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, // режимы отображения окошка
        CW_USEDEFAULT, // позиция окошка по оси х
        NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
        CW_USEDEFAULT, // ширина окошка
        NULL, // высота окошка (раз дефолт в ширине, то писать не нужно)
        (HWND)NULL, // дескриптор родительского окна
        NULL, // дескриптор меню
        HINSTANCE(hInst), // дескриптор экземпляра приложения
        NULL); // ничего не передаём из WndProc
    if (!hMainWnd) {
        // в случае некорректного создания окошка (неверные параметры и тп):
        MessageBox(NULL, L"Class error", L"Error", MB_OK);
        return NULL;
    }
    ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
    UpdateWindow(hMainWnd); // обновляем окошко
    while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        TranslateMessage(&msg); // интерпретируем сообщения
        DispatchMessage(&msg); // передаём сообщения обратно ОС
    }
    return msg.wParam; // возвращаем код выхода из приложения
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
    SelectObject(hDC, pen); //делаем кисть активной
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
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // если закрыли окошко
    }
    return NULL; // возвращаем значение
}
