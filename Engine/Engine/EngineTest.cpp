#include "Engine.h"


using namespace Engine;

GameEngine engine;
SmartPtr<GameObject> object;
Vector2<double> gravity;


void TestKeyDown()
{
    DEBUG_PRINT("Key down Call back!!!!!\n");
}
void TestKeyUp()
{
    DEBUG_PRINT("Key up Call back!!!!!\n");
}



#pragma region Desktop App Code

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
    engine.CreatGameObject("Mo", 1.0, Vector2<double>(10, 10), Vector2<double>::Zero, Vector2<double>::Zero);
    object = engine.gameObjectList[0];
    gravity = Vector2<double>::Down;

    Matrix4<double> test;
    test.UnitTest();




    Matrix4<double> m = Matrix4<double>(
        1, 1, 1, - 1,
        1, 1, -1, 1,
        1, -1, 1, 1,
        -1, 1, 1, 1);

    Matrix4<int> n = Matrix4<int>(
        5, 2, 3, 1,
        3, 4, 3, 2,
        4, 6, 4, 2,
        7, 3, 4, 4);

    Matrix4<double> p = n.GetInverse<double>();
    n.Invert();

    Matrix3<int> o = Matrix3<int>(
        1, 2, 3,
        0, 1, 4,
        5, 6, 0);

    o.Invert();

    Vector2<int> test1 = Vector2<int>(1, 1);
    Vector2<int> test2 = test1;


    /**/

    // Run the message loop.
    //MSG msg = { };
    //while (GetMessage(&msg, NULL, 0, 0) > 0)
    //{
    //    const PhysicObject* physicObject = engine.physicManager->FindPhysicObj(object);
    //    Debug::print<float>("Before, total force: %f \n", physicObject->force.y());
    //    engine.Update(hwnd);
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}

    engine.inputManager->AddOnKeyDownCallback(KeyCode::A, &TestKeyDown);
    engine.inputManager->AddOnKeyUpCallback(KeyCode::A, &TestKeyUp);

    do
    {

        engine.Update(hwnd);

        if (engine.inputManager->GetKeyUp(KeyCode::O))
        {
            engine.physicManager->AddForceToObj(object, gravity);
            DEBUG_PRINT("Add %dN Force! \n", 1);
        }
        else if (engine.inputManager->GetKeyUp(KeyCode::P))
        {
            engine.physicManager->AddForceToObj(object, -1 * gravity);
            DEBUG_PRINT("Remove %dN Force! \n", 1);
        }


        Sleep(100);

    } while (!engine.quitRequest);

    engine.inputManager->RemoveOnKeyDownCallback(KeyCode::A, &TestKeyDown);
    engine.inputManager->RemoveOnKeyUpCallback(KeyCode::A, &TestKeyUp);

    /* Game engine code */
    engine.Destroy();
    object.~SmartPtr();
    /**/


    _CrtDumpMemoryLeaks();

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
    //case WM_INPUT:
    //case WM_KEYDOWN:
    //{
    //    if (wParam == 0x4F)
    //    {
    //        engine.physicManager->AddForceToObj(object, gravity);
    //        DEBUG_PRINT("Add %dN Force! \n", 1);
    //    }
    //    else if (wParam == 0x50)
    //    {
    //        engine.physicManager->AddForceToObj(object, -1 * gravity);
    //        DEBUG_PRINT("Remove %dN Force! \n", 1);
    //    }
    //    break;
    //}
    case WM_KEYUP:
    {
        if (wParam == 0x50)
            //engine.isKeyDown = false;
        break;
    }
    case WM_SETTEXT:
    {
        InvalidateRect(hwnd, NULL, FALSE);
        hdc = BeginPaint(hwnd, &ps);


        std::wstring text;
        const wchar_t* ptr;
        int len;
        text = L"Press 'o' key to add force to gameobject, press 'p' to remove force!";
        ptr = text.c_str();
        len = static_cast<int>(wcslen(ptr));
        TextOut(hdc, 60, 30, ptr, len);

        text = L"DeltaTime: " + std::to_wstring(Time::DeltaTime);
        ptr = text.c_str();
        len = static_cast<int>(wcslen(ptr));
        TextOut(hdc, 60, 60, ptr, len);

        /* Game engine code */
        const PhysicObject* physicObject = engine.physicManager->FindPhysicObj(object);
        assert(physicObject != nullptr);
        /**/

        text = L"GameObject State: "
            L"Pos: (" + to_wstring(object->position[0]) + L"," + to_wstring(object->position[1]) + L"), " +
            L"Vel: (" + to_wstring(physicObject->velocity[0]) + L"," + to_wstring(physicObject->velocity[1]) + L")";
        ptr = text.c_str();
        len = static_cast<int>(wcslen(ptr));
        TextOut(hdc, 60, 80, ptr, len);

        double dragFactor = 0.1f;
        double dragDirX = physicObject->velocity[0] == 0 ? 0 : -1 * physicObject->velocity[0] / abs(physicObject->velocity[0]);
        double dragDirY = physicObject->velocity[1] == 0 ? 0 : -1 * physicObject->velocity[1] / abs(physicObject->velocity[1]);
        Vector2<double> dragForce = dragFactor * physicObject->velocity * physicObject->velocity * Vector2<double>(dragDirX, dragDirY);
        Vector2<double> totalForce = physicObject->force;
        text = //L"DragForce: (" + to_wstring(dragForce.x()) + L"," + to_wstring(dragForce.y()) + L"), "
            L"TotalForce: (" + to_wstring(totalForce[0]) + L"," + to_wstring(totalForce[1]) + L")";
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



#pragma endregion






#pragma region Console Code

/* Debug start */
class Test
{
public:
    vector<Vector2<float>> temp1 = { Vector2<float>::Zero, Vector2<float>::Down, Vector2<float>::Zero };

    Test(){}

};

void TestCase(Test target, Vector2<float>& out)
{
    for (Vector2<float>& item : target.temp1)
    {
        if (item == Vector2<float>::Down)
            out = item;
    }

}
/* Debug End */


int main()
{
    printf("Console App \n\n");

    engine.GameLoop();
    engine.Destroy();




    //SmartPtr<GameObject> temp1 = SmartPtr<GameObject>(new GameObject("L1"));
    //SmartPtr<GameObject> temp2 = SmartPtr<GameObject>(new GameObject("L2"));
    //SmartPtr<Vector2> temp3 = SmartPtr<Vector2>(new Vector2());
    //temp1 = temp2;


    //Test* temp4 = Test::Instance();
    //printf("a: %d \n", temp4->a);
    //temp4->a = 2;
    //printf("a: %d \n", temp4->a);



    //Vector2* temp5 = new Vector2();
    //Vector2* temp6 = temp5;

    //printf("P5 %p \n", temp5);
    //printf("P6 %p \n", temp6);

    //delete temp5;

    //if (temp5 == nullptr)
    //    printf("P5 is nullptr \n");
    //else
    //    printf("P5 %p \n", temp5);

    //if (temp6 == nullptr)
    //    printf("P6 is nullptr \n");
    //else
    //    printf("P6 %p", temp6);

    int res = _CrtDumpMemoryLeaks();
    if (res == TRUE)
        printf("\n #### Detedct Memory Leak!! \n");
    else
        printf("\n #### No Memory Leak!! \n");

    return 0;
}

#pragma endregion