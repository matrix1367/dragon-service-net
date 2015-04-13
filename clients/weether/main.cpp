#ifndef _WIN32_IE
  #define  _WIN32_IE 0x0400
#endif // _WIN32_IE

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "Models.h"
#include "ScheduleManager.h"
#include "CDSetting.h"

HINSTANCE hInst;

void RefreshListViewSchedule(HWND listView) {
    //HWND listView = parm.handle;
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
    int i = 0;
    ListView_DeleteAllItems(listView);
    std::list<CTask> schedules = CScheduleManager::getInstance().GetSchedule();
    for (std::list<CTask>::iterator it= schedules.begin(); it != schedules.end(); it++) {
        char buffer[5];
        itoa(i+1,buffer,10);

        lvi.pszText = const_cast<char * >(std::string(buffer).c_str());
        lvi.iItem = i;
        lvi.iSubItem = 0;

        ListView_InsertItem( listView, & lvi );
        ListView_SetItemText( listView, i, 1, const_cast<char *>(it->GetStrName().c_str()));
        ListView_SetItemText( listView, i, 2, const_cast<char *>(it->GetStrDateStart().c_str()));
        ListView_SetItemText( listView, i, 3, const_cast<char *>(it->GetStrDateEnd().c_str()));
        ListView_SetItemText( listView, i, 4, const_cast<char *>(it->GetStrInterval().c_str()));
        i++;
    }
}

BOOL CALLBACK DlgSchedule(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND listView = GetDlgItem(hwndDlg,DLG_SCHEDULE_LISTVIE);

        char lp[] = {"lp"};
        char name[] = {"Nazwa"};
        char id[] = {"Data pocz¹tkowa"};
        char adressIP[] = {"Data koñcowa"};
        char status[] = {"Interwa³"};

        LVCOLUMN lvc;
        lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

        lvc.iSubItem = 0;
        lvc.cx = 30;
        lvc.pszText = lp;
        ListView_InsertColumn( listView, 0, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 120;
        lvc.pszText = name;
        ListView_InsertColumn( listView, 1, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 120;
        lvc.pszText = id;
        ListView_InsertColumn( listView, 2, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 100;
        lvc.pszText = adressIP;
        ListView_InsertColumn( listView, 3, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 120;
        lvc.pszText = status;
        ListView_InsertColumn( listView, 4, & lvc );

        RefreshListViewSchedule(listView);

        ListView_SetExtendedListViewStyleEx(listView, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

    }
    return TRUE;

    case WM_CLOSE:
    {
        CScheduleManager::getInstance().Save();

        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_RBUTTONDOWN:
            {
                POINT cursor;
                GetCursorPos(&cursor);
                TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
            }
            break;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        }
    }
    return TRUE;

    case WM_SIZE:
    {
        HWND listView = GetDlgItem(hwndDlg,DLG_SCHEDULE_LISTVIE);
        MoveWindow(listView, 0, 0, LOWORD(lParam), HIWORD(lParam), true);
    }
    return true;
    }
    return FALSE;
}


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
    }
    return TRUE;

    case WM_CLOSE:
    {
        CScheduleManager::getInstance().Save();
        CDSetting::getInstance().Save();
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
              case ID_MENU_SCHEDULE: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SCHEDULE), NULL, (DLGPROC)DlgSchedule);
            }
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

    CDSetting::getInstance().Load();
    CModels::getInstance().Init();
    CScheduleManager::getInstance().Load();
    CScheduleManager::getInstance().Start();
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
