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

std::string GetWindowText(HWND hwndDlg, int id) {
    TCHAR buffDataOd[1024];
    HWND hwndComponnent =       GetDlgItem(hwndDlg, id);
    GetWindowText(hwndComponnent, buffDataOd, 1024);
    return std::string(buffDataOd);
}

time_t ConvertStringToTime_T(std::string date, std::string time) {
    time_t result = 0;
    struct tm * timeinfo=  localtime( & result );
    if (date.size() == 10)
    {

        std::string strYear = date.substr(0,4);
        std::string strMonth = date.substr(5,2);
        std::string strDay = date.substr(8,2);
        timeinfo->tm_year = atoi(strYear.c_str()) - 1900;
        timeinfo->tm_mon  = atoi(strMonth.c_str()) - 1;
        timeinfo->tm_mday   = atoi(strDay.c_str());
        if (time.size() ==  8) {
            std::string strH = time.substr(0,2);
            std::string strM = time.substr(3,2);
            std::string strS = time.substr(6,2);
            timeinfo->tm_hour = atoi(strH.c_str());
            timeinfo->tm_min = atoi(strM.c_str());
            timeinfo->tm_sec = atoi(strS.c_str());
            //printf("CONVERT DATA: %s:%s:%s" , strH.c_str(), strM.c_str(), strS.c_str());
        }
    }
    result = mktime(timeinfo);
    return result;
}

int selectIndex = -1;
BOOL CALLBACK DlgTask(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        if (selectIndex > -1) {
           // CTask taskEdit

        }
    }
    return TRUE;

    case WM_CLOSE:
    {

        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        if ( HIWORD( wParam ) == BN_CLICKED )
        {
            if ( LOWORD( wParam ) == IDD_DIALOG_TASK_CHECK_STOP )
            {
                HWND dataP = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_DATE_END);
                HWND timeP = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_TIME_END);
                bool checkboxActive = (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP ,BM_GETCHECK,0,0)==BST_CHECKED);
                    EnableWindow(dataP,checkboxActive);
                    EnableWindow(timeP,checkboxActive);
            }

            if ( LOWORD( wParam ) == IDD_DIALOG_TASK_CHECK_INTERVAL ) {
                bool checkboxActive = (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL ,BM_GETCHECK,0,0)==BST_CHECKED);
                HWND editI = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_INTERVAL);
                EnableWindow(editI,checkboxActive);
            }
        }
        switch(LOWORD(wParam))
        {

            case IDOK:
            {
                std::string name = GetWindowText(hwndDlg,IDD_DIALOG_TASK_NAME);
                std::string dateSt = GetWindowText(hwndDlg,IDD_DIALOG_TASK_DATE_ST);
                std::string timeSt = GetWindowText(hwndDlg,IDD_DIALOG_TASK_TIME_ST);
                std::string dateEnd = GetWindowText(hwndDlg,IDD_DIALOG_TASK_DATE_END);
                std::string timeEnd = GetWindowText(hwndDlg,IDD_DIALOG_TASK_TIME_END);
                std::string interval = GetWindowText(hwndDlg,IDD_DIALOG_TASK_INTERVAL);

                //printf("name %s, data %s, time %s, date %s, time %s, interval %s", name.c_str(), dateSt.c_str(), timeSt.c_str(), dateEnd.c_str(), timeEnd.c_str(), interval.c_str());

                //indexJob = GetWindowText(hwndDlg,IDD_DIALOG_TASK_NAME);
                int intInterval = 0;
                time_t tDateSt = ConvertStringToTime_T(dateSt, timeSt);
                time_t tDateEnd = 0;
                if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP ,BM_GETCHECK,0,0)==BST_CHECKED)  {
                    tDateEnd = ConvertStringToTime_T(dateEnd, timeEnd);
                }

                if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL ,BM_GETCHECK,0,0)==BST_CHECKED)  {
                    intInterval = atoi(interval.c_str());
                }

                CScheduleManager::getInstance().AddTask(name, tDateSt, tDateEnd , intInterval );
                EndDialog(hwndDlg, IDOK);
                break;
            }
            case IDCANCEL:
            {
                EndDialog(hwndDlg, IDCANCEL);
                break;
            }
        }
    }
    return TRUE;
    }
    return FALSE;
}

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
        lvc.cx = 120;
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

    case WM_CONTEXTMENU:
            {

                int iSelected = -1;
                iSelected = SendMessage(GetDlgItem(hwndDlg,DLG_SCHEDULE_LISTVIE), LVM_GETNEXTITEM, -1,LVNI_SELECTED);

                POINT cursor;
                GetCursorPos(&cursor);
                if (iSelected != -1) {
                    TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT_ACTIVE)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
                } else {
                    TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
                }

            }
            break;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            case IDR_MENU_SCHEDUL_EDIT_ADD: {
                printf("IDR_MENU_SCHEDUL_EDIT_ADD:\n");
                int res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), NULL, (DLGPROC)DlgTask);
                if (res == IDOK) {
                    HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                    RefreshListViewSchedule(listView);
                }
                break;
            }
            case IDR_MENU_SCHEDUL_EDIT_EDIT: {
                printf("IDR_MENU_SCHEDUL_EDIT_EDIT:\n");
                int res =  DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), NULL, (DLGPROC)DlgTask);
                if (res == IDOK) {
                    HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                    RefreshListViewSchedule(listView);
                }
                break;
            }
            case IDR_MENU_SCHEDUL_EDIT_DELETE: {
                printf("IDR_MENU_SCHEDUL_EDIT_DELETE:\n");
                HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
                while (iPos != -1) {
                    CScheduleManager::getInstance().RemoveTask(iPos);
                    ListView_DeleteItem(listView,iPos);
                    iPos = ListView_GetNextItem(listView, iPos, LVNI_SELECTED);
                }
                //RefreshListViewSchedule(listView);
                break;
            }
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
