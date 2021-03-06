#pragma once

#include "resource.h"

//force this app to Idle and process messages from other apps and the system
//only use this from separate thread, not in main thread.
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
    static CCriticalSection  m_critSect;
public:

    CRotateDlg() { return; };
    virtual ~CRotateDlg() { return; };
    virtual BOOL InitInstance() { return TRUE; };
    virtual int Run();
    CEvent *m_pEvent;
    //flag stuff with locks
    void SetFlag(int flag)
    {
        m_critSect.Lock();
        m_flag = flag;
        m_critSect.Unlock();
    }
    int GetFlag()
    {
        int ret;
        m_critSect.Lock();
        ret = m_flag;
        m_critSect.Unlock();
        return ret;
    }
};