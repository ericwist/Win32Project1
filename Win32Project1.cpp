// Win32Project1.cpp : Basic way to defeat Corporate employee monitoring
// How to defeat Big-Brother 1984-style monitoring for now...more later.
//
// This can be used "as-is" if your Corp just monitors your activity, if it needs a specific app 
// opened you can open it manually and THEN run this one.  If you want a more sophisticated 
// execution that opens a specific messaging or monitoring app then search for "ZZZ" or "TODO" 
// and do what it says...
// ***Build a release version of this code and run it when you leave your desk, 
// and want to be seen as active!
#include "stdafx.h"
#include "Win32Project1.h"
#include <TlHelp32.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//start of custom code
BOOL isThereAPreviousInstanceRunning() {
    BOOL ret = TRUE;
    int tries = 5;
    HANDLE hMutex = NULL;
    Idle(50);
#if _DEBUG
    Idle(200);
#endif
    while (tries > 0) {
        // We don't want to own the mutex.
        hMutex = CreateMutex(NULL, FALSE, szWindowClass);
        if (NULL == hMutex) {
            // Something bad happened, fail.
            // We don't know anything here so
            // just exit and the user will have to
            // double-click icon again
            break;
        }

        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            CloseHandle(hMutex);
            hMutex = NULL;
            // It's possible that the instance we found isn't coming up,
            // but rather is going down.  Try again.
            tries--;
            // Give the other instance time to finish if it's going down.
            Idle(100);
        }
        else {
            // We were the first one to create the mutex
            // so that makes us the main instance.  Leak
            // the mutex in this function so it gets cleaned
            // up by the OS when this instance exits.
            // Return so we continue to open program
            ret = FALSE;
            break;
        }
    }
    return ret;
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
    if (TRUE == isThereAPreviousInstanceRunning()) {
        // Do not initialize another one is already running
        return FALSE;
    }
    
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

//Check if skype(or monitoring app) is running
bool IsSkypeRunning()
{
    HANDLE hProcessSnapshot = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe32;
    hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnapshot, &pe32))
    {
        CloseHandle(hProcessSnapshot);
        return false;
    }

    do
    {
        //ZZZ TODO: edit this to point to the messaging program that records your activity
        //case sensitivity and length with spaces must be exact, 
        //or you can add some string manipulation if you want...
        //debug thru and check it
        if (!lstrcmp(pe32.szExeFile,_T("notepad.exe")))// this is usually "lync.exe"
        {
            CloseHandle(hProcessSnapshot);
            return true;
        }

    }
    while (Process32Next(hProcessSnapshot, &pe32));
    CloseHandle(hProcessSnapshot);
    return false;
}

//start skype or your monitoring program
bool startSkype()
{
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    //ZZZ TODO: edit this to point to the messaging program that records your activity
    //case sensitivity and length with spaces must be exact, 
    //or you can add some string manipulation if you want...
    //debug thru and check it
    wchar_t  *exe = L"C:\\Windows\\notepad.exe";//give full path of corporate monitoring program
    BOOL ret = CreateProcess(exe, L"", NULL, NULL, TRUE, DETACHED_PROCESS, NULL, NULL, &si, &pi);
    if (ret)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    DWORD err = GetLastError();
    return false;
}

void writeCode(HWND hDlg) {
    const char* letters = "void Reverse(CLinkedList **head)\n{\n \
// Initialize current, previous and\n \
// next pointers\n \
CLinkedList *curr = *head;\n \
CLinkedList *prev = nullptr, *next = nullptr;\n \
while (curr != nullptr)\n{\n \
\t// Store next\n \
\tnext = curr->next;\n \
\t// Reverse current node's pointer\n \
\tcurr->next = prev;\n \
\t// Move pointers one position ahead.\n \
\tprev = curr;\n \
\tcurr = next;\n \
}\n \
*head = prev;\n \
}";
    int i = 0;

    while (letters[i]) {
        ///
        Idle(2);
        SetActiveWindow(hDlg);
        SetForegroundWindow(hDlg);
        Idle(2);
        ////
        if (letters[i] > 32 && letters[i] < 43 || letters[i] > 122 || letters[i] == 60 || letters[i] == 62) {
            keybd_event(VK_SHIFT, 0xAA, 0, 0);
        }
        keybd_event(VkKeyScan(letters[i]), 0x9e, 0, 0);
        keybd_event(VkKeyScan(letters[i]), 0x9e, KEYEVENTF_KEYUP, 0);
        if (letters[i] > 32 && letters[i] < 43 || letters[i] > 122 || letters[i] == 60 || letters[i] == 62) {
            keybd_event(VK_SHIFT, 0xAA, KEYEVENTF_KEYUP, 0);
        }
        Sleep(18);
        ++i;
    }
    Sleep(10);
    keybd_event(VK_CONTROL, 0x9d, 0, 0);
    keybd_event(VkKeyScan('A'), 0x9e, 0, 0);
    keybd_event(VkKeyScan('A'), 0x9e, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0x9d, KEYEVENTF_KEYUP, 0);
    Sleep(50);
    keybd_event(VK_BACK, 0x8a, 0, 0);
    keybd_event(VK_BACK, 0x8a, KEYEVENTF_KEYUP, 0);
}

int	CRotateDlg::m_flag = 0;
CCriticalSection CRotateDlg::m_critSect;

//thread
int CRotateDlg::Run()
{
    //In case I started Skype from the WndProc, wait a bit...
    Idle(20000);//20 seconds
    do
    {
        //In case I closed the app by mistake, check if it is still running
        if (!IsSkypeRunning())
        {
            startSkype();
            //wait for it to start
            Idle(20000);//20 seconds
        }

        wchar_t buffer[10000];
        HWND hDlg = GetDesktopWindow();
        for (hDlg = GetWindow(hDlg, GW_CHILD); hDlg; hDlg = GetNextWindow(hDlg, GW_HWNDNEXT))
        {
            GetWindowText(hDlg, buffer, 10000);
            if (!buffer[0])
            {
                continue;
            }
            // ZZZ TODO: This is the Window name as it appears across the top of the Window bar, 
            // this must be exact, you can debug through to find the exact name. 
            CString s = "Notepad";
            CString str = buffer;
            // ZZZ TODO: In this case the win name string is "Untitled - Notepad", so I just 
            // looked for the right side portion equal to the size of Notepad the string.  
            // It could be different in your case so you have to debug through everything 
            // and add a watch to see what it is.
            CString str2 = str.Right(s.GetLength());
            if (!lstrcmp(str2, s))
            {
                Idle(5);
                SetActiveWindow(hDlg);
                SetForegroundWindow(hDlg);
                Sleep(5);
                Idle(50);
                writeCode(hDlg);
                //WM_KEYDOWN: SHIFT
                keybd_event(VK_MENU, 0, 0, 0);
                keybd_event(VK_TAB, 0, 0, 0);
                Idle(1000);
                keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
                Idle(50);
                keybd_event(VK_TAB, 0, 0, 0);
                Idle(50);
                // stop pressing "Alt-Tab"
                keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
                break;
            }
        }
        WaitForSingleObject(m_pEvent->m_hObject, 50000);// wait 100 seconds(100000), change if imeout is different on your company's monitoring program.
        ResetEvent(m_pEvent->m_hObject);
    } while (GetFlag() == 0);
    return(0);
}

CRotateDlg *rot = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    break;
    case WM_CREATE:
    {
        if (!IsSkypeRunning())
        {
            startSkype();
        }
        rot = new CRotateDlg();
        rot->m_pEvent = new CEvent(FALSE, FALSE);
        rot->CreateThread(CREATE_SUSPENDED);
        rot->m_bAutoDelete = false;
        rot->SetFlag(0);
        rot->ResumeThread();
    }
    break;
    case WM_CLOSE:
    {
        rot->SetFlag(1);
        SetEvent(*rot->m_pEvent);
        DestroyWindow(hWnd);
        break;
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
              break;
            case IDM_EXIT:
                rot->SetFlag(1);
                SetEvent(*rot->m_pEvent);
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
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (rot != nullptr)
        {
            delete rot;
        }
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
}
