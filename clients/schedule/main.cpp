#ifndef _WIN32_IE
#define  _WIN32_IE 0x600
#endif // _WIN32_IE

#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>

#include <stdio.h>
#include <vector>
#include <algorithm>

#include <string>
#include "resource.h"
#include "Models.h"
#include "ScheduleManager.h"
#include "ClientManagerSchedule.h"

#include "CDSetting.h"
#include "CDLog.h"
#include "TasksManager.h"
#include "EventManager.h"

#include "GUIManager.h"



HINSTANCE hInst;




void UpdateTrayIcon(HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon, std::string icon)
{
    //CREATE SYSTEN TRAY ICON.---------------------------------------------------------------------
    NOTIFYICONDATA   nid;
    nid.cbSize = sizeof( NOTIFYICONDATA );
    nid.hWnd             = hWnd;
    nid.uID              = uID;
    nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP  ;
    nid.uCallbackMessage = uCallbackMsg;
    nid.dwInfoFlags = NIIF_WARNING;

    ExtractIconEx( icon.c_str(), 0, NULL, &(nid.hIcon), 1 );
    strcpy(nid.szTip, "Schedule");

    //SEND MESSAGE TO SYSTEM TRAY TO ADD ICON.--------------------------------------------
    Shell_NotifyIcon( NIM_MODIFY, &nid );
}

//===================================================================================
//AddTrayIcon
//===================================================================================
void AddTrayIcon( HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon )
{
    //CREATE SYSTEN TRAY ICON.---------------------------------------------------------------------
    NOTIFYICONDATA  nid;
    nid.hWnd             = hWnd;
    nid.uID              = uID;
    nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = uCallbackMsg;
    ExtractIconEx( "img\\schedul-icon.ico", 0, NULL, &(nid.hIcon), 1 );
    strcpy(nid.szTip, "Weater");

    //SEND MESSAGE TO SYSTEM TRAY TO ADD ICON.--------------------------------------------
    Shell_NotifyIcon( NIM_ADD, &nid );
}

//===================================================================================
//RemoveTrayIcon
//===================================================================================
void RemoveTrayIcon( HWND hWnd, UINT uID )
{
    NOTIFYICONDATA  nid;
    nid.hWnd = hWnd;
    nid.uID  = uID;
    Shell_NotifyIcon( NIM_DELETE, &nid );
}

std::string GetWindowText(HWND hwndDlg, int id)
{
    TCHAR buffDataOd[1024];
    HWND hwndComponnent =       GetDlgItem(hwndDlg, id);
    GetWindowText(hwndComponnent, buffDataOd, 1024);
    return std::string(buffDataOd);
}

time_t ConvertStringToTime_T(std::string date, std::string time)
{
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
        if (time.size() ==  8)
        {
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


BOOL CALLBACK DlgTask(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        // printf("WM_INITDIALOG DlgTask: select item : %d\n", lParam);
        std::vector<std::string> strJob = CJobsManager::getInstance().GetStrAllNameJob();
        HWND hWndComboBox = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_JOB);

        for (std::vector<std::string>::iterator it = strJob.begin(); it != strJob.end() ; it++)
        {
            SendMessage(hWndComboBox,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) (*it).c_str());
        }


        if (lParam > 0)
        {
            // CTask taskEdit
            CTask* task =  CScheduleManager::getInstance().GetTask(lParam-1);

            HWND hName = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_NAME);
            HWND hDataS = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_DATE_ST);
            HWND hTimeS = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_TIME_ST);
            HWND hDataE = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_DATE_END);
            HWND hTimeE = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_TIME_END);
            HWND hInterval = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_INTERVAL);
            HWND hSetting = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_SETTING);
            HWND hButtonOK = GetDlgItem(hwndDlg,IDOK);
            HWND hButtonSave = GetDlgItem(hwndDlg,IDSAVE);
            HWND hLabelID = GetDlgItem(hwndDlg, IDD_ID_TASK);

            // HWND hCheckBoxInterval = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL);
            // HWND hCheckBoxDataEnd = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP);

            SetWindowText( hLabelID, task->GetStrID().c_str());
            SetWindowText( hName, task->GetStrName().c_str());
            SetWindowText( hInterval, task->GetStrInterval().c_str() );
            SetWindowText( hSetting, task->GetStrString().c_str() );
            SYSTEMTIME dataStart = task->GetDataStart();
            SYSTEMTIME dataEnd = task->GetDataEnd();
            SendMessage( hDataS, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataStart );
            SendMessage( hTimeS, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataStart );
            SendMessage( hDataE, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataEnd );
            SendMessage( hTimeE, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataEnd );

            if (task->GetInterval() > 0)
            {
                CheckDlgButton(hwndDlg, IDD_DIALOG_TASK_CHECK_INTERVAL, BST_CHECKED);
                EnableWindow(hInterval, true);
            }
            if (task->GetDateEnd() > 0)
            {
                CheckDlgButton(hwndDlg, IDD_DIALOG_TASK_CHECK_STOP, BST_CHECKED);
                EnableWindow(hDataE, true);
                EnableWindow(hTimeE, true);
            }

            SendMessage((HWND) hWndComboBox, (UINT) CB_SETCURSEL,(WPARAM) CJobsManager::getInstance().GetIndexJob(task->GetIdJob()), (LPARAM) 0);

            ShowWindow(hButtonOK,SW_HIDE);
            ShowWindow(hButtonSave,SW_SHOW);
        }
        else
        {

            SendMessage((HWND) hWndComboBox, (UINT) CB_SETCURSEL,(WPARAM) 0, (LPARAM) 0);
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

            if ( LOWORD( wParam ) == IDD_DIALOG_TASK_CHECK_INTERVAL )
            {
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
            std::string settingJob = GetWindowText(hwndDlg,IDD_DIALOG_TASK_SETTING);

            //printf("name %s, data %s, time %s, date %s, time %s, interval %s", name.c_str(), dateSt.c_str(), timeSt.c_str(), dateEnd.c_str(), timeEnd.c_str(), interval.c_str());

            //indexJob = GetWindowText(hwndDlg,IDD_DIALOG_TASK_NAME);
            int intInterval = 0;
            time_t tDateSt = ConvertStringToTime_T(dateSt, timeSt);
            time_t tDateEnd = 0;
            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                tDateEnd = ConvertStringToTime_T(dateEnd, timeEnd);
            }

            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                intInterval = atoi(interval.c_str());
            }
            HWND hWndComboBox = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_JOB);
            int index = SendMessage((HWND) hWndComboBox, (UINT) CB_GETCURSEL,(WPARAM) 0, (LPARAM) 0);

            CScheduleManager::getInstance().AddTask(name, tDateSt, tDateEnd , intInterval, CJobsManager::getInstance().GetIdJob(index),settingJob );
            EndDialog(hwndDlg, IDOK);
            break;
        }
        case IDSAVE:
        {
            //edycja
            std::string IdTask = GetWindowText(hwndDlg,IDD_ID_TASK);
            std::string name = GetWindowText(hwndDlg,IDD_DIALOG_TASK_NAME);
            std::string dateSt = GetWindowText(hwndDlg,IDD_DIALOG_TASK_DATE_ST);
            std::string timeSt = GetWindowText(hwndDlg,IDD_DIALOG_TASK_TIME_ST);
            std::string dateEnd = GetWindowText(hwndDlg,IDD_DIALOG_TASK_DATE_END);
            std::string timeEnd = GetWindowText(hwndDlg,IDD_DIALOG_TASK_TIME_END);
            std::string interval = GetWindowText(hwndDlg,IDD_DIALOG_TASK_INTERVAL);
            std::string settingJob = GetWindowText(hwndDlg,IDD_DIALOG_TASK_SETTING);
            int intInterval = 0;
            time_t tDateSt = ConvertStringToTime_T(dateSt, timeSt);
            time_t tDateEnd = 0;
            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                tDateEnd = ConvertStringToTime_T(dateEnd, timeEnd);
            }

            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                intInterval = atoi(interval.c_str());
            }
            HWND hWndComboBox = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_JOB);
            int index = SendMessage((HWND) hWndComboBox, (UINT) CB_GETCURSEL,(WPARAM) 0, (LPARAM) 0);
            long long idTask = atoll(IdTask.c_str());
            CScheduleManager::getInstance().EditTask(idTask ,name, tDateSt, tDateEnd , intInterval, CJobsManager::getInstance().GetIdJob(index),settingJob);
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

void RefreshListViewSchedule(TypeParmT parm)
{
    HWND listView = parm.handle;
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
    int i = 0;
    ListView_DeleteAllItems(listView);
    std::list<CTask> schedules = CScheduleManager::getInstance().GetSchedule();
    for (std::list<CTask>::iterator it= schedules.begin(); it != schedules.end(); it++)
    {
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
        ListView_SetItemText( listView, i, 5, const_cast<char *>( CJobsManager::getInstance().GetStrNameJob(it->GetIdJob()).c_str() ));
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
        char id[] = {"Data poczatkowa"};
        char adressIP[] = {"Data koncowa"};
        char status[] = {"Interwal"};
        char jobName[] = {"Nazwa zadania"};

        LVCOLUMN lvc;
        lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

        lvc.iSubItem = 0;
        lvc.cx = 30;
        lvc.pszText = lp;
        ListView_InsertColumn( listView, 0, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 150;
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

        lvc.iSubItem = 0;
        lvc.cx = 120;
        lvc.pszText = jobName;
        ListView_InsertColumn( listView, 5, & lvc );


        TypeParmT parm;
        parm.handle = listView;
        RefreshListViewSchedule(parm);

        Event event;
        event.paem1.handle = listView;
        event.typeEvent = EVENT_ADD_SCHEDULE;
        event.handleFunction = &RefreshListViewSchedule;
        CEventManager::getInstance().Subscribe(event);
        event.typeEvent = EVENT_DELETE_SCHEDULE;
        CEventManager::getInstance().Subscribe(event);

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
        if (iSelected != -1)
        {
            TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT_ACTIVE)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
        }
        else
        {
            TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
        }

    }
    break;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDR_MENU_SCHEDUL_EDIT_ADD:
        {
            printf("IDR_MENU_SCHEDUL_EDIT_ADD:\n");
            int res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), NULL, (DLGPROC)DlgTask);
            if (res == IDOK)
            {
                CEventManager::getInstance().Send(EVENT_ADD_SCHEDULE);
                //HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                //RefreshListViewSchedule(listView);
            }
            break;
        }
        case IDR_MENU_SCHEDUL_EDIT_EDIT:
        {

            HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
            int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
            //printf("IDR_MENU_SCHEDUL_EDIT_EDIT: select item : %d\n", iPos);



            //int res =  DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), hwndEdit, (DLGPROC)DlgTask);
            CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), hwndDlg, (DLGPROC)DlgTask, iPos+1);
            //   if (res == IDOK)
            //  {
            //       CEventManager::getInstance().Send(EVENT_ADD_SCHEDULE);
            //       //HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
            //       // RefreshListViewSchedule(listView);
            //    }
            break;
        }
        case IDR_MENU_SCHEDUL_EDIT_DELETE:
        {
            printf("IDR_MENU_SCHEDUL_EDIT_DELETE:\n");
            HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
            int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
            while (iPos != -1)
            {
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

BOOL CALLBACK DlgClientTerm(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
      if (lParam > 0)
        {
            // CTask taskEdit
            CTask* task =  CScheduleManager::getInstance().GetTask(lParam-1);

            HWND hName = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_NAME);
            HWND hDataS = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_DATE_ST);
            HWND hTimeS = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_TIME_ST);
            HWND hDataE = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_DATE_END);
            HWND hTimeE = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_TIME_END);
            HWND hInterval = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_INTERVAL);
            HWND hSetting = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_SETTING);
            HWND hButtonOK = GetDlgItem(hwndDlg,IDOK);
            HWND hButtonSave = GetDlgItem(hwndDlg,IDSAVE);
            HWND hLabelID = GetDlgItem(hwndDlg, IDD_ID_TASK);

            // HWND hCheckBoxInterval = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL);
            // HWND hCheckBoxDataEnd = GetDlgItem(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP);

            SetWindowText( hLabelID, task->GetStrID().c_str());
            SetWindowText( hName, task->GetStrName().c_str());
            SetWindowText( hInterval, task->GetStrInterval().c_str() );
            SetWindowText( hSetting, task->GetStrString().c_str() );
            SYSTEMTIME dataStart = task->GetDataStart();
            SYSTEMTIME dataEnd = task->GetDataEnd();
            SendMessage( hDataS, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataStart );
            SendMessage( hTimeS, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataStart );
            SendMessage( hDataE, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataEnd );
            SendMessage( hTimeE, DTM_SETSYSTEMTIME, 0,( LPARAM ) &dataEnd );

            if (task->GetInterval() > 0)
            {
                CheckDlgButton(hwndDlg, IDD_DIALOG_TASK_CHECK_INTERVAL, BST_CHECKED);
                EnableWindow(hInterval, true);
            }
            if (task->GetDateEnd() > 0)
            {
                CheckDlgButton(hwndDlg, IDD_DIALOG_TASK_CHECK_STOP, BST_CHECKED);
                EnableWindow(hDataE, true);
                EnableWindow(hTimeE, true);
            }


            ShowWindow(hButtonOK,SW_HIDE);
            ShowWindow(hButtonSave,SW_SHOW);
        }
        else
        {


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
            if ( LOWORD( wParam ) == IDD_DIALOG_TASK_CHECK_INTERVAL )
            {
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
            time_t tDateEnd = ConvertStringToTime_T(dateEnd, timeEnd);


            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                intInterval = atoi(interval.c_str());
            }
            HWND hWndComboBox = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_JOB);
            int index = SendMessage((HWND) hWndComboBox, (UINT) CB_GETCURSEL,(WPARAM) 0, (LPARAM) 0);

             EndDialog(hwndDlg, IDOK);
            break;
        }
        case IDSAVE:
        {
            //edycja
            std::string IdTask = GetWindowText(hwndDlg,IDD_ID_TASK);
            std::string name = GetWindowText(hwndDlg,IDD_DIALOG_TASK_NAME);
            std::string dateSt = GetWindowText(hwndDlg,IDD_DIALOG_TASK_DATE_ST);
            std::string timeSt = GetWindowText(hwndDlg,IDD_DIALOG_TASK_TIME_ST);
            std::string dateEnd = GetWindowText(hwndDlg,IDD_DIALOG_TASK_DATE_END);
            std::string timeEnd = GetWindowText(hwndDlg,IDD_DIALOG_TASK_TIME_END);
            std::string interval = GetWindowText(hwndDlg,IDD_DIALOG_TASK_INTERVAL);
            std::string settingJob = GetWindowText(hwndDlg,IDD_DIALOG_TASK_SETTING);
            int intInterval = 0;
            time_t tDateSt = ConvertStringToTime_T(dateSt, timeSt);
            time_t tDateEnd = 0;
            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_STOP ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                tDateEnd = ConvertStringToTime_T(dateEnd, timeEnd);
            }

            if (SendDlgItemMessage(hwndDlg,IDD_DIALOG_TASK_CHECK_INTERVAL ,BM_GETCHECK,0,0)==BST_CHECKED)
            {
                intInterval = atoi(interval.c_str());
            }
            HWND hWndComboBox = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_JOB);
            int index = SendMessage((HWND) hWndComboBox, (UINT) CB_GETCURSEL,(WPARAM) 0, (LPARAM) 0);
            long long idTask = atoll(IdTask.c_str());
            CScheduleManager::getInstance().EditTask(idTask ,name, tDateSt, tDateEnd , intInterval, CJobsManager::getInstance().GetIdJob(index),settingJob);
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


BOOL CALLBACK DlgSetting(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hEditIP = GetDlgItem(hwndDlg,IDD_DIALOG_SETTING_IP);
        HWND hEditPort = GetDlgItem(hwndDlg,IDD_DIALOG_SETTING_PORT);
        HWND hEditCity = GetDlgItem(hwndDlg,IDD_DIALOG_SETTING_CITY);
        SetWindowText( hEditIP, CDSetting::getInstance().getSetting().ipServer );
        SetWindowText( hEditPort, CDSetting::getInstance().getSetting().portServer );
        SetWindowText( hEditCity, CDSetting::getInstance().getSetting().cityName );
    }
    return TRUE;

    case WM_CLOSE:
    {
        //CScheduleManager::getInstance().Save();
        CDSetting::getInstance().Save();
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDOK:
        {
            std::string ip = GetWindowText(hwndDlg,IDD_DIALOG_SETTING_IP);
            std::string port = GetWindowText(hwndDlg,IDD_DIALOG_SETTING_PORT);
            std::string cityName = GetWindowText(hwndDlg,IDD_DIALOG_SETTING_CITY);
            CDSetting::getInstance().SetIp(ip);
            CDSetting::getInstance().SetPort(port);
            CDSetting::getInstance().SetCityName(cityName);
            EndDialog(hwndDlg, 0);
            break;
        }
        case IDCANCEL:
        {
            EndDialog(hwndDlg, 0);
            break;
        }
        }
    }
    return TRUE;
    }
    return FALSE;
}

void RefreshDlgMain(TypeParmT parm)
{

}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
            SetClassLong( hwndDlg, GCL_HICON,( LONG ) LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_ICON1 ) ) );
            AddTrayIcon( hwndDlg, ID_TRAY1, WM_APP, 0 );

            HWND listView = GetDlgItem(hwndDlg,DLG_MAIN_LISTVIE);

            char lp[] = {"Czas"};
            char name[] = {"Nazwa"};
            char id[] = {"Data poczatkowa"};
            char adressIP[] = {"Data koncowa"};
            char status[] = {"Interwal"};


            LVCOLUMN lvc;
            lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

            lvc.iSubItem = 0;
            lvc.cx = 150;
            lvc.pszText = lp;
            ListView_InsertColumn( listView, 0, & lvc );

            lvc.iSubItem = 0;
            lvc.cx = 150;
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


            ListView_SetExtendedListViewStyleEx(listView, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
        }
        return TRUE;

        case WM_CLOSE:
        {
            CScheduleManager::getInstance().Save();
            CDSetting::getInstance().Save();
            RemoveTrayIcon (hwndDlg, ID_TRAY1);
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_APP:
        {
            if( wParam == ID_TRAY1 )
            {
                if( lParam == WM_LBUTTONDOWN )
                    ShowWindow( hwndDlg, SW_RESTORE );
                if ( lParam == WM_RBUTTONDOWN)
                {
                    HMENU popMenu = LoadMenu(hInst,  MAKEINTRESOURCE(IDR_MENU_POP));
                    popMenu =  GetSubMenu(popMenu, 0);
                    POINT pp;
                    GetCursorPos(&pp);
                    TrackPopupMenu(popMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,	pp.x,pp.y, 0, hwndDlg, NULL);
                }
            }
        }
        return TRUE;

        case WM_SIZE:
        {
            HWND listView = GetDlgItem(hwndDlg,DLG_MAIN_LISTVIE);
            MoveWindow(listView, 0, 0, LOWORD(lParam), HIWORD(lParam), true);

            switch(wParam)
            {
            case SIZE_MINIMIZED:
                ShowWindow(hwndDlg, SW_HIDE);
                break;
            case SIZE_RESTORED:
                // Do whatever
                break;
            }
            break;
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_MENU_SETTING:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SETTING), NULL, (DLGPROC)DlgSetting);
                break;
            }
            case ID_MENU_SCHEDULE:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SCHEDULE), NULL, (DLGPROC)DlgSchedule);
                break;
            }
            case ID_MENU_EXIT:
            {
                CScheduleManager::getInstance().Save();
                CDSetting::getInstance().Save();
                RemoveTrayIcon (hwndDlg, ID_TRAY1);
                EndDialog(hwndDlg, 0);
                break;
            }
            case IDR_MENU_SCHEDUL_EDIT_ADD:
            {
                printf("IDR_MENU_SCHEDUL_EDIT_ADD:\n");
                int res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TERM), NULL, (DLGPROC)DlgClientTerm);
                if (res == IDOK)
                {
                    CEventManager::getInstance().Send(EVENT_ADD_SCHEDULE);
                }
                break;
            }
            case IDR_MENU_SCHEDUL_EDIT_EDIT:
            {
    /*
                HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
                CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), hwndDlg, (DLGPROC)DlgTask, iPos+1);
    */
                break;
            }
            case IDR_MENU_SCHEDUL_EDIT_DELETE:
            {
                /*
                printf("IDR_MENU_SCHEDUL_EDIT_DELETE:\n");
                HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
                while (iPos != -1)
                {
                    CScheduleManager::getInstance().RemoveTask(iPos);
                    ListView_DeleteItem(listView,iPos);
                    iPos = ListView_GetNextItem(listView, iPos, LVNI_SELECTED);
                }
            */
                break;
            }

            }
        }
        return TRUE;

        case WM_CONTEXTMENU:
        {

                int iSelected = -1;
                iSelected = SendMessage(GetDlgItem(hwndDlg,DLG_MAIN_LISTVIE), LVM_GETNEXTITEM, -1,LVNI_SELECTED);

                POINT cursor;
                GetCursorPos(&cursor);
                if (iSelected != -1)
                {
                    TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT_ACTIVE)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
                }
                else
                {
                    TrackPopupMenu((HMENU) GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_SCHEDUL_EDIT)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwndDlg, NULL);
                }

                break;
            }
        return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

    CDSetting::getInstance().Load();
    CModels::getInstance().Init();

    CClientManagerSchedule::getInstance().Load();
    CClientManagerSchedule::getInstance().Start();

    CScheduleManager::getInstance().Load();
    CScheduleManager::getInstance().Start();

    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
