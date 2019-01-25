#pragma once

#include "resource.h"

//force windows to Idle and process messages from other apps and the system
inline bool Idle(DWORD ticks = 0)
{
    MSG   msg;
    DWORD max_time = ticks + GetTickCount();
    BOOL  bret = true;

    while (GetTickCount()<max_time)
    {
        while ((bret = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)))
        {
            if (bret)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        Sleep(5);
    }
    return true;
}

//Create a simple thread class using MFC
class CRotateDlg : public CWinThread
{
    static int m_flag; //shared resource
    static CCriticalSection  _critSect;
    static int m_X;
    static int m_Y;
public:

    CRotateDlg() { return; };
    virtual ~CRotateDlg() { return; };
    virtual BOOL InitInstance() { return TRUE; };
    virtual int Run();
    CEvent *m_pEvent;
    //flag stuff with locks
    void SetFlag(int flag)
    {
        _critSect.Lock();
        m_flag = flag;
        _critSect.Unlock();
    }

    int GetFlag()
    {
        int ret;
        _critSect.Lock();
        ret = m_flag;
        _critSect.Unlock();
        return ret;
    }
};