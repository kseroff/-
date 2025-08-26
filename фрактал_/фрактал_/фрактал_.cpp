// фрактал_.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "фрактал_.h"
#include <Windows.h>
#include <complex>
#include <vector>

using namespace std;

#define MAX_LOADSTRING 100

const int WIDTH = 800;
const int HEIGHT = 600;

// Определяем максимальное количество итераций для вычисления фрактала
const int MAX_ITERATIONS = 100;

// Функция, которая возвращает цвет пикселя в зависимости от количества итераций
COLORREF GetPixelColor(int iterations) {
    if (iterations == MAX_ITERATIONS) {
        return RGB(0, 0, 0);  // Черный цвет, если точка входит в множество Мандельброта
    }
    else {
        int r = (iterations * 9);
        int g = (iterations * 7);
        int b = (iterations * 13);
        return RGB(r, g, b);  // Возвращаем цвет в зависимости от количества итераций
    }
}

// Функция для вычисления значения z по формуле z = z*z*z + z*z + c
complex<double> ComputeZ(complex<double> z,complex<double> c) {
    //return z * z * z + z * z + c;
    //return z * z + c;
    return z * z * z * z * z + c;
}

// Функция для рисования фрактала
void DrawFractal(HDC hdc) {
    /*
    const double real_min = -0.75;  // Минимальное значение действительной части
    const double real_max = 0.1;   // Максимальное значение действительной части
    const double imag_min = 0.5;  // Минимальное значение мнимой части
    const double imag_max = 1.1;   // Максимальное значение мнимой части
    */
    const double real_min = -2.0;  // Минимальное значение действительной части
    const double real_max = 2.0;   // Максимальное значение действительной части
    const double imag_min = 1.5;  // Минимальное значение мнимой части
    const double imag_max = -1.5;   // Максимальное значение мнимой части

    // Создаем вектор для хранения цветов пикселей
    vector<vector<COLORREF>> pixelColors(WIDTH, vector<COLORREF>(HEIGHT));

    // Проходимся по каждому пикселю и вычисляем значение z для соответствующей точки c
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            double real = real_min + (real_max - real_min) * x / (double)WIDTH;
            double imag = imag_min + (imag_max - imag_min) * y / (double)HEIGHT;

            complex<double> z(0, 0);
            complex<double> c(real, imag);

            int iterations = 0;
            while (std::abs(z) < 2 && iterations < MAX_ITERATIONS) {
                z = ComputeZ(z, c);
                iterations++;
            }

            //pixelColors[x][y] = GetPixelColor(iterations);
            SetPixel(hdc, x, y, GetPixelColor(iterations));
        }
    }

    //Рисуем каждый пиксель на холсте
    //for (int x = 0; x < WIDTH; x++) {
    //    for (int y = 0; y < HEIGHT; y++) {
    //        SetPixel(hdc, x, y, pixelColors[x][y]);
    //    }
    //}
}

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC..
            
            DrawFractal(hdc);

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

// Обработчик сообщений для окна "О программе".
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
}

