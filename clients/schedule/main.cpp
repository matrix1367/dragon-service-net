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
#include "CDSetting.h"
#include "CDLog.h"
#include "TasksManager.h"
#include "EventManager.h"

#include "GUIManager.h"

#ifdef mCTRL
#include <winuser.h>
#include <mCtrl/chart.h>
#include <mCtrl/dialog.h>
#endif // mCTRL

HINSTANCE hInst;
HBITMAP hBitmap  = NULL;



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
      strcpy(nid.szTip, CModels::getInstance().GetTopicalWeather("NULL").c_str());

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
      ExtractIconEx( "img\\01d.ico", 0, NULL, &(nid.hIcon), 1 );
      strcpy(nid.szTip, "Weater");

      //SEND MESSAGE TO SYSTEM TRAY TO ADD ICON.--------------------------------------------
      Shell_NotifyIcon( NIM_ADD, &nid );
}

//===================================================================================
//RemoveTrayIcon
//===================================================================================
void RemoveTrayIcon( HWND hWnd, UINT uID ) {
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
            if (task->GetDateEnd() > 0) {
                CheckDlgButton(hwndDlg, IDD_DIALOG_TASK_CHECK_STOP, BST_CHECKED);
                EnableWindow(hDataE, true); EnableWindow(hTimeE, true);
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




PAINTSTRUCT ps;
HDC hdcDestination;

std::vector<int> listYi;
std::vector<int> listYiHeight;
int mediumYiHeight = 200;

std::vector<std::string> listXi;
std::vector<std::string> listXiDate;
std::vector<double> listData;

std::vector<double> listDataRain;
std::vector<double> listP;

void RefreshCheat(TypeParmT parm)
{
    DataWeetherNextHours dataWeether;
    CModels::getInstance().GetWeether( std::string(CDSetting::getInstance().getSetting().cityName), dataWeether);
    if (dataWeether.items.size() == 0) return;
    listYi.clear();
    listXi.clear();
    listData.clear();
    listDataRain.clear();
    listP.clear();
    for (unsigned int i = 0 ; i < dataWeether.items.size() ; i++ )
    {
        std::vector<int>::iterator it = find (listYi.begin(), listYi.end(), (int) dataWeether.items[i].main.temp);
        if (it == listYi.end())
        {
            listYi.push_back((int)dataWeether.items[i].main.temp);
        }
        char godzina[ 80 ];
        char day[ 80 ];
        struct tm *data = localtime( & dataWeether.items[i].dt );
        strftime( godzina, 80, "%H", data );
        strftime( day, 80, "%m-%d", data );
        listXi.push_back(godzina);
        listXiDate.push_back(day);

        listData.push_back(dataWeether.items[i].main.temp);

        listDataRain.push_back(dataWeether.items[i].rain);

        listP.push_back(1014 - dataWeether.items[i].main.pressure);

        //printf("data:: %s  temp :%f, rain:%f , p:%f\n" , godzina , dataWeether.items[i].main.temp, dataWeether.items[i].rain, 1014 - dataWeether.items[i].main.pressure);

    }
    std::sort(listYi.begin(), listYi.begin() + listYi.size());
    listYi.push_back( listYi.back() + 1);

    InvalidateRect(parm.handle, NULL, true);
    UpdateWindow(parm.handle);
}

int ConvertTempToPix(const double & currentTemp, const double& currentHeight) {
    unsigned int i = 0;
    for ( std::vector<int>::iterator it = listYi.begin(); it != listYi.end(); it++)
    {
        if (*it >  currentTemp ) {
          if (i < listYiHeight.size())  return listYiHeight[i]; else listYiHeight[listYiHeight.size()-1];
        }
        i++;
    }
    return listYiHeight.back();
  //  double prop = (listYi.back() - listYi.front()) / currentTemp;
  //  return currentHeight - (currentHeight / prop);
}

BOOL CALLBACK DlgNextHours(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifndef mCTRL
    static HBRUSH testBrush;

#endif // mCTRL
    switch(uMsg)
    {
#ifndef mCTRL
    case WM_PAINT:
    {
        PAINTSTRUCT ps_NH;
        HDC hdc;

        const unsigned int marginLeft = 60;
        const unsigned int marginRight = 150;
        const unsigned int marginTop = 50;
        const unsigned int marginButton  = 60;

        RECT rcClient;
        GetClientRect(hwndDlg, &rcClient);

        int  widthWindow = rcClient.right - rcClient.left;
        int heightWindow = rcClient.bottom - rcClient.top;


        hdc = BeginPaint(hwndDlg, &ps_NH);
        HPEN hPenOld;

        // Draw a red line
        HPEN hLinePen;
        COLORREF qLineColorBlack =  RGB(0, 0, 0);
        COLORREF qLineColorGrey = RGB(191, 191, 191);
        hLinePen = CreatePen(PS_SOLID, 2, qLineColorBlack);


        HPEN hLinePenGrey = CreatePen(PS_SOLID, 1, qLineColorGrey);
        HPEN hLinePenGreyOld = (HPEN) SelectObject(hdc, hLinePenGrey);

        const short int scale = 25;
        listYiHeight.clear();

        unsigned int i;
        unsigned short int y ;
        for (  i = 0 , y = heightWindow-marginButton- scale; y > marginTop ; y-= scale, i++ )
        {
            MoveToEx(hdc, marginLeft, y, NULL);
            LineTo(hdc, widthWindow-marginRight, y);
            listYiHeight.push_back(y);

            RECT rect;
            rect.left=marginLeft - scale;  rect.top=y-8;

            if (i < listYi.size()) DrawText( hdc, CDLog::ToString(listYi[i]).c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP   ) ;
        }

        int index = ((listYiHeight.size()/2)-1);
        if ( index > -1) {
             mediumYiHeight = listYiHeight[index];
        } else {
            mediumYiHeight = heightWindow/2;
        }

       // COLORREF qLineColorGreen =  RGB(0, 255, 0);
        // HPEN hLinePenGreen = CreatePen(PS_SOLID, 1, qLineColorGreen);
      //  HPEN hPenGreenOld  = (HPEN)

      SelectObject( hdc, GetStockObject( GRAY_BRUSH ) );

        unsigned short int x;
        for (i =0, x = marginLeft + scale; x < widthWindow - marginRight ; x+= scale, i++)
        {
            //SelectObject(hdc, hLinePenGrey);
            MoveToEx(hdc, x, marginTop, NULL);
            LineTo(hdc, x, heightWindow-marginButton);

            RECT rect;
            rect.left=x-scale;
            rect.top=heightWindow-marginButton+10;

            if (i < listXi.size()) DrawText( hdc, (listXi[i]).c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP   ) ;

            rect.top=heightWindow-marginButton+35;
            if (i < listXi.size() && (i % 5) == 0) DrawText( hdc, (listXiDate[i]).c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP   ) ;

            //SelectObject(hdc, hLinePenGreen);

            if (i < listDataRain.size()) Rectangle(hdc, x - 5, (heightWindow-marginButton -  1) - listDataRain[i]*12.5  , x + 5, heightWindow-marginButton - 1);

            if (i < listP.size()) Rectangle(hdc, x - 5, mediumYiHeight  + listP[i] * 12.5  , x + 5, mediumYiHeight);
        }
        SelectObject( hdc, GetStockObject( NULL_BRUSH ) );
        //draw data
        COLORREF qLineColorRed =  RGB(255, 0, 0);
        HPEN hLinePenRed = CreatePen(PS_SOLID, 2, qLineColorRed);
        HPEN hPenRedOld  = (HPEN)SelectObject(hdc, hLinePenRed);


        MoveToEx(hdc, marginLeft, ConvertTempToPix(listData[0], heightWindow - marginButton - marginTop)  , NULL);
        //printf("[LOG] H:%s, temp:%f , x:%d, y:%d\n",listXi[0].c_str(), listData[0] , marginLeft , ConvertTempToPix(listData[0], heightWindow - marginButton - marginTop));
        for (i =1, x = marginLeft+scale; x < widthWindow - marginRight ; x+= scale, i++)
        {

            if (i < listData.size()) LineTo(hdc, x, ConvertTempToPix(listData[i],heightWindow - marginButton - marginTop));
            //printf("[LOG] H:%s, temp:%f , x:%d, y:%d\n", listXi[i].c_str(), listData[i] , x , ConvertTempToPix(listData[i],heightWindow - marginButton - marginTop));
        }




        Rectangle(hdc,widthWindow - marginRight + 25, marginTop + 25, widthWindow - marginRight + 35, marginTop + 35);

        RECT rect;
        rect.left= widthWindow - marginRight + 45;
        rect.top= marginTop + 21;
        DrawText( hdc, "Poznan C", -1, &rect, DT_SINGLELINE | DT_NOCLIP   ) ;

        rect.left= widthWindow - marginRight + 15;
        rect.top= mediumYiHeight-8;
        DrawText( hdc, "1014 hPa", -1, &rect, DT_SINGLELINE | DT_NOCLIP   ) ;

        hPenOld = (HPEN)SelectObject(hdc, hLinePen);

        MoveToEx(hdc, marginLeft, marginTop, NULL);
        LineTo(hdc, marginLeft, heightWindow-marginButton);
        LineTo(hdc, widthWindow-marginRight, heightWindow-marginButton);

        MoveToEx(hdc, marginLeft, mediumYiHeight, NULL);
        LineTo(hdc, widthWindow-marginRight, mediumYiHeight);


        SelectObject(hdc, hPenOld);
        SelectObject(hdc, hLinePenGreyOld);
        SelectObject(hdc, hPenRedOld);
        DeleteObject(hLinePen);
        DeleteObject(hLinePenGrey);
        DeleteObject(hLinePenRed);

        EndPaint(hwndDlg, &ps_NH);
    }
    return true;

    case WM_CTLCOLORDLG:
          return (INT_PTR)( testBrush );
#endif // mCTRL

    case WM_INITDIALOG:
    {

#ifdef mCTRL
        SetupCommonChart(GetDlgItem(hwndDlg, IDC_CHART_LINE), dataWeether);
        #else
        testBrush = CreateSolidBrush( RGB( 255, 255, 255 ) );

        TypeParmT parm;
        parm.handle = hwndDlg;
        RefreshCheat(parm);

        Event event;
        event.paem1.handle = hwndDlg;
        event.typeEvent = EVENT_UPDATE_WEATHER_NEXT_HOURS;
        event.handleFunction = &RefreshCheat;
        CEventManager::getInstance().Subscribe(event);

#endif // mCTRL
        break;
    }
    return TRUE;



    case WM_CLOSE:
    {

        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {

        }
    }
    return TRUE;

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        mmi->ptMinTrackSize.x = 400;
        mmi->ptMinTrackSize.y = 300;
       // mmi->ptMaxTrackSize.x = 1200;
       // mmi->ptMaxTrackSize.y = 1000;
        return 0;
    }

    case WM_SIZE:
    {
        //int nWidth = LOWORD(lParam);
        //int nHeight = HIWORD(lParam);
        //   CGUIManager::getInstance().CreateTemplateChart(nWidth,nHeight);
        //   width = nWidth;
        //   height = nHeight;
       //    printf("w:%d, h:%d\n" , width, height);
        InvalidateRect(hwndDlg, NULL, true);
        UpdateWindow(hwndDlg);
    }
    return TRUE;
    }
    return FALSE;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT 	ps;
    	HDC 			hdc;
    	BITMAP 			bitmap;
    	HDC 			hdcMem;
        HGDIOBJ 		oldBitmap;

    	hdc = BeginPaint(hwndDlg, &ps);

    	hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, hBitmap);

        GetObject(hBitmap, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 20, 50, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

    	EndPaint(hwndDlg, &ps);

    }
    return true;
    case WM_INITDIALOG:
    {
        CGUIManager::getInstance().hWndDialogMain = hwndDlg;
        TypeParmT parm;
        parm.handle = hwndDlg;
        RefreshDlgMain(parm);

        SetClassLong( hwndDlg, GCL_HICON,( LONG ) LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_ICON1 ) ) );

        Event event;
        event.paem1.handle = hwndDlg;
        event.typeEvent = EVENT_UPDATE_WEETHER;
        event.handleFunction = &RefreshDlgMain;
        CEventManager::getInstance().Subscribe(event);

         hBitmap  = (HBITMAP)LoadImage(hInst, "img\\01d.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
         AddTrayIcon( hwndDlg, ID_TRAY1, WM_APP, 0 );
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
        if( wParam == ID_TRAY1 ) {
            if( lParam == WM_LBUTTONDOWN )
                ShowWindow( hwndDlg, SW_RESTORE );
            if ( lParam == WM_RBUTTONDOWN) {
                HMENU popMenu = LoadMenu(hInst,  MAKEINTRESOURCE(IDR_MENU_POP));
                popMenu =  GetSubMenu(popMenu, 0);
                POINT pp;
                GetCursorPos(&pp);
                TrackPopupMenu(popMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,	pp.x,pp.y, 0, hwndDlg, NULL);
            }
        }
    }
    break;

    case WM_SIZE:
        switch(wParam) {
        case SIZE_MINIMIZED:
          ShowWindow(hwndDlg, SW_HIDE);
          break;
        case SIZE_RESTORED:
          // Do whatever
          break;
        }

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case ID_MENU_NEXT_HOURS :
            {
#ifdef mCTRL
                if(!mcChart_Initialize()) {
                    MessageBox(NULL, "The function mcChart_Initialize() has failed. "
                               "Perhaps GDIPLUS.DLL is not available on your machine?",
                               "Error", MB_OK | MB_ICONERROR);
                    return 1;
                }

                mcDialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_NEXT_H), NULL, DlgNextHours, MC_DF_DEFAULTFONT);
#else
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_NEXT_H), NULL, (DLGPROC)DlgNextHours);
#endif // mCTRL
                break;
            }
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