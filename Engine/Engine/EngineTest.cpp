#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <string>
#include "Engine.h"

using namespace std;
using namespace Engine;

GameEngine engine;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPWSTR pCmdLine, 
    _In_ int nCmdShow)
{

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"GameEngine test",             // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);


    /* Game engine code */
    engine = GameEngine();
    engine.gameObjectList.push_back(GameObject("Mo", 1, Vector2(10, 10), Vector2(0, 0)));


    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        engine.Update(hwnd);

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN:
        {
            if (wParam == 0x50)
                engine.isKeyDown = true;

            break;
        }
        case WM_KEYUP:
        {
            if (wParam == 0x50)
                engine.isKeyDown = false;
            break;
        }
        case WM_SETTEXT:
        {
            InvalidateRect(hwnd, NULL, FALSE);
            hdc = BeginPaint(hwnd, &ps);


            std::wstring text;
            const wchar_t* ptr;
            int len;
            text = L"Hold 'p' key down to keep add force to gameobject!";
            ptr = text.c_str();
            len = static_cast<int>(wcslen(ptr));
            TextOut(hdc, 60, 30, ptr, len);

            text = L"DeltaTime: " + std::to_wstring(engine.timeManager.GetDeltaTime());
            ptr = text.c_str();
            len = static_cast<int>(wcslen(ptr));
            TextOut(hdc, 60, 60, ptr, len);

            GameObject object = engine.gameObjectList[0];
            text = L"GameObject State: " 
                L"Pos: (" + to_wstring(object.position.x()) + L"," + to_wstring(object.position.y()) + L")" +
                L"Vel: (" + to_wstring(object.velocity.x()) + L"," + to_wstring(object.velocity.y()) + L")";
            ptr = text.c_str();
            len = static_cast<int>(wcslen(ptr));
            TextOut(hdc, 60, 80, ptr, len);

            Vector2 gravity = Vector2(0.0, -1.0);
            float dragFactor = 0.1f;
            float dragDirX = object.velocity.x() == 0 ? 0 : -1 * object.velocity.x() / abs(object.velocity.x());
            float dragDirY = object.velocity.y() == 0 ? 0 : -1 * object.velocity.y() / abs(object.velocity.y());
            Vector2 dragForce = dragFactor * object.velocity * object.velocity * Vector2(dragDirX, dragDirY);
            Vector2 totalForce = gravity + dragForce;
            text = L"DragForce: (" + to_wstring(dragForce.x()) + L"," + to_wstring(dragForce.y()) + L")"
                L"TotalForce: (" + to_wstring(totalForce.x()) + L"," + to_wstring(totalForce.y()) + L")";
            ptr = text.c_str();
            len = static_cast<int>(wcslen(ptr));
            TextOut(hdc, 60, 100, ptr, len);


            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CLOSE:
        {
            if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
                DestroyWindow(hwnd);
            return 0;
        }
        default:                      
            return DefWindowProc(hwnd, uMsg, wParam, lParam);

    }
    return 0;
}


int main()
{
    //engine = GameEngine();
    //engine.gameObjectList.push_back(GameObject("Mo", 1.0, Vector2(10, 10), Vector2(0, 0)));
    //engine.GameLoop();

    



    SmartPtr<GameObject> temp1 = SmartPtr<GameObject>(new GameObject("L1"));
    SmartPtr<GameObject> temp2 = SmartPtr<GameObject>(new GameObject("L2"));
    SmartPtr<Vector2> temp3 = SmartPtr<Vector2>(new Vector2());



    printf("%d \n", (*temp1).mass);


    return 0;
}