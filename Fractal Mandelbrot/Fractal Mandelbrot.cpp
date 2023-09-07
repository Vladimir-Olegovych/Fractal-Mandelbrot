#include <windows.h>

#include <random>
#include <cmath>

#include <gdiplus.h>

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

//const
const UINT16 height = GetSystemMetrics(SM_CYSCREEN) - 30;
const UINT16 width = GetSystemMetrics(SM_CXSCREEN);

const UINT16 posX = 500;
const UINT16 posY = 500;

//functions
void draw(HDC hdc);
//Class
class Complex 
{
public:
    double re;
    double im;
    Complex(double are, double aim): re(are), im(aim){}
    Complex operator+(const Complex& v)
    {
        return Complex(re + v.re, im + v.im);
    }
};
Complex sqr(const Complex& v)
{
    return Complex(v.re * v.re - v.im * v.im, 2 * v.re * v.im);
}
double abs(const Complex &v) {
    return sqrt(v.re * v.re + v.im * v.im);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    //GDI+
    GdiplusStartupInput gdiPlusInput;
    ULONG_PTR gdiPlusToken;
    GdiplusStartup(&gdiPlusToken, &gdiPlusInput, nullptr);
    //Screen
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wc = { };
    MSG msg = { };
    HWND hwnd;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance = hInstance;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WindowProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wc);

    hwnd = CreateWindowEx(
        0,                         // Optional window styles.
        CLASS_NAME,                // Window class
        L"(Fractal andelbrot)",   // Window text
        WS_OVERLAPPEDWINDOW,       // Window style

        0,  //CW_USEDEFAULT
        0,  //CW_USEDEFAULT
        width,
        height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiPlusToken);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    HDC hdc;
    PAINTSTRUCT ps;

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {
        //ESC
        if (wParam == 0x1B) {
            exit(0);
            break;
        }
        //Tab
        if (wParam == 0x09) {
            break;
        }
    }
    case WM_PAINT: {
        //Render
        hdc = BeginPaint(hwnd, &ps);

        draw(hdc);

        EndPaint(hwnd, &ps);
        //End Render
    }
                 return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
void draw(HDC hdc) {
    Graphics gf(hdc);
    SolidBrush brush(Color::Black);

    for (int y = 0; y < posY; ++y) {
        for (int x = 0; x < posX; ++x)
        {
            Complex z(0, 0);
            int i = 0;
            while (i < 1000 && abs(z) < 2)
            {
                z = sqr(z) + Complex(1.0 * (x - posX/2)/150, 1.0 * (y - posY/2)/150);
                ++i;
            }
            if (abs(z) >= 2)
            {
                float col = (50.0 - i) / 50.0;
                brush.SetColor(Color(col, col, col));
                gf.FillEllipse(&brush, x, y, 2, 2);
            }
        }
    }
}
