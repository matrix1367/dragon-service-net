/*
* API Key  d5141e09f65620429bf2ac06cd2a5a2c
* http://openweathermap.org
*/


#ifndef _WIN32_IE
#define  _WIN32_IE 0x600
#endif // _WIN32_IE

#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>

#include <stdio.h>
#include <vector>
#include <string>
#include "resource.h"
#include "Models.h"
#include "ScheduleManager.h"
#include "CDSetting.h"
#include "TasksManager.h"
#include "EventManager.h"

#include "GUIManager.h"

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
      strcpy       (nid.szTip, "Weater");

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
      ExtractIconEx( "F:\\projects\\dragon-service-net-master\\clients\\weether\\img\\01d.ico", 0, NULL, &(nid.hIcon), 1 );
      strcpy       (nid.szTip, "Weater");

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

int selectIndex = -1;
BOOL CALLBACK DlgTask(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        if (selectIndex > -1)
        {
            // CTask taskEdit

        }
        else
        {
            std::vector<std::string> strJob = CJobsManager::getInstance().GetStrAllNameJob();
            HWND hWndComboBox = GetDlgItem(hwndDlg, IDD_DIALOG_TASK_JOB);

            for (std::vector<std::string>::iterator it = strJob.begin(); it != strJob.end() ; it++)
            {
                SendMessage(hWndComboBox,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) (*it).c_str());
            }
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
            printf("IDR_MENU_SCHEDUL_EDIT_EDIT:\n");
            int res =  DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TASK), NULL, (DLGPROC)DlgTask);
            if (res == IDOK)
            {
                CEventManager::getInstance().Send(EVENT_ADD_SCHEDULE);
                //HWND listView = GetDlgItem(hwndDlg, DLG_SCHEDULE_LISTVIE);
                // RefreshListViewSchedule(listView);
            }
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
    HWND hwndDlg = parm.handle;
    DataWeether dataWeether;
    CModels::getInstance().GetWeether(CDSetting::getInstance().getSetting().cityName, dataWeether);
    HWND hNameCity = GetDlgItem(hwndDlg,DLG_MAIN_NAMECITY);
    HWND hNameTemperature = GetDlgItem(hwndDlg,DLG_MAIN_TEMPERATURE);
    HWND hNameCloud = GetDlgItem(hwndDlg,DLG_MAIN__CLOUD);
    HWND hNameWind = GetDlgItem(hwndDlg,DLG_MAIN_WIND);
    HWND hNamePressure = GetDlgItem(hwndDlg,DLG_MAIN_PRESSURE);
    HWND hNamePressureMax = GetDlgItem(hwndDlg,DLG_MAIN_PRESSURE_MAX);
    HWND hNamePressureMin = GetDlgItem(hwndDlg,DLG_MAIN_PRESSURE_MIN);
    HWND hNameHumidtly  = GetDlgItem(hwndDlg,DLG_MAIN_HUMIDITY);
    HWND hNameTemperatureMax = GetDlgItem(hwndDlg,DLG_MAIN_TEMPERATURE_MAX);
    HWND hNameTemperatureMin = GetDlgItem(hwndDlg,DLG_MAIN_TEMPERATURE_MIN);
    HWND hTime = GetDlgItem(hwndDlg,DLG_MAIN_TIME);

    struct tm * data;
    data = localtime( &  dataWeether.dateTimeUpdate );
    char godzina[ 80 ];
    strftime( godzina, 80, "Update: %H:%M:%S.", data );

    SetWindowText(hNameCity, dataWeether.nameCity);
    SetWindowText(hNameTemperature, (dataWeether.main.getStr_temp() + " C").c_str());
    SetWindowText(hNameTemperatureMax, (dataWeether.main.getStr_temp_max() + " C").c_str());
    SetWindowText(hNameTemperatureMin, (dataWeether.main.getStr_temp_min() + " C").c_str());
    SetWindowText(hNameCloud, dataWeether.description.c_str());
    SetWindowText(hNameWind, std::string( "speed: " + dataWeether.wind.getStr_speed()  + " m/s, deg: " + dataWeether.wind.getStr_deg() + " N").c_str()   );
    SetWindowText(hNamePressure, (dataWeether.main.getStr_pressure() + " hpa").c_str());
    SetWindowText(hNamePressureMax, (dataWeether.main.getStr_see_level() + " hpa").c_str());
    SetWindowText(hNamePressureMin, (dataWeether.main.getStr_grnd_level() + " hpa").c_str());
    SetWindowText(hNameHumidtly, (dataWeether.main.getStr_humidity() + " %").c_str());
    SetWindowText(hTime, godzina);

    std::string pathImg = "F:\\projects\\dragon-service-net-master\\clients\\weether\\img\\" + dataWeether.icon + ".bmp";
    std::string pathIcon = "F:\\projects\\dragon-service-net-master\\clients\\weether\\img\\" + dataWeether.icon + ".ico";

    hBitmap  = (HBITMAP)LoadImage(hInst, pathImg.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    InvalidateRect(hwndDlg, NULL, true);
    UpdateWindow(hwndDlg);
    UpdateTrayIcon(hwndDlg, ID_TRAY1, WM_APP, 0 ,pathIcon);
    CGUIManager::getInstance().ShowMessage("Aktualizacja danych", "Dane zosta³y pomyœlnie zaktualizowane");
}



PAINTSTRUCT ps;
HDC hdcDestination;


#include <gdiplus.h>


BOOL CALLBACK DlgNextHours(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg)
    {
    case WM_PAINT:
    {

        const unsigned int width = 400;
        const unsigned int height = 400;

        HDC hDC, MemDCExercising;
        PAINTSTRUCT Ps;
        HBITMAP bmpExercising;

        hDC = BeginPaint(hwndDlg, &Ps);

        const unsigned w=0xFFFFFF, b=0, y=0xFFFF00; // colors: white, black, yellow
        unsigned bmp[width*height] = { }; // example 8x8 face image

        for (unsigned int x =0 ; x < 160000 ; x++)
        {

                bmp[x] = w;

        }

        bmpExercising =  CreateBitmap(400, 400, 32, 1, bmp);

        MemDCExercising = CreateCompatibleDC(hDC);
        SelectObject(MemDCExercising, bmpExercising);
        BitBlt(hDC, 0, 0, 400, 400, MemDCExercising, 0, 0, SRCCOPY);
        DeleteDC(MemDCExercising);
        DeleteObject(bmpExercising);
        EndPaint(hwndDlg, &Ps);


        /*

            HDC hdcOkno = GetDC( hwndDlg );
            HBRUSH PedzelZiel, Pudelko;
            HPEN OlowekBialy, Piornik;

            POINT stary;


            PedzelZiel = CreateSolidBrush( 0xFFFFFF );
            OlowekBialy = CreatePen( PS_SOLID, 1, 0xFFFFFF );



            Pudelko =( HBRUSH ) SelectObject( hdcOkno, PedzelZiel );
            Piornik =( HPEN ) SelectObject( hdcOkno, OlowekBialy );


            Rectangle( hdcOkno, 0, 0, 800, 500 );




            SelectObject( hdcOkno, Pudelko );
          //  SelectObject( hdcOkno, Piornik );
           // SelectObject( hdcOkno, PiornikCzarny );




            DeleteObject( OlowekBialy );
            DeleteObject( PedzelZiel );

            ReleaseDC( hwndDlg, hdcOkno );
*/


    }
    return true;
    case WM_INITDIALOG:
    {
        CGUIManager::getInstance().CreateTemplateChart(300,300);
        DataWeetherNextHours dataWeether;
        CModels::getInstance().GetWeether( std::string(CDSetting::getInstance().getSetting().cityName), dataWeether);
        for (unsigned int i = 0 ; i < dataWeether.items.size() ; i++ )
        {
            printf("data:: %s\n" , dataWeether.items[i].dt_text.c_str());
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
        switch(LOWORD(wParam))
        {

        }
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

         hBitmap  = (HBITMAP)LoadImage(hInst, "F:\\projects\\dragon-service-net-master\\clients\\weether\\img\\01d.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
         AddTrayIcon( hwndDlg, ID_TRAY1, WM_APP, 0 );
    }
    return TRUE;

    case WM_CLOSE:
    {
        CScheduleManager::getInstance().Save();
        CDSetting::getInstance().Save();
        RemoveTrayIcon (hwndDlg, 1);
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_APP:
    {
        if( wParam == ID_TRAY1 )
        if( lParam == WM_LBUTTONDOWN )
             ShowWindow( hwndDlg, SW_RESTORE );

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
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_NEXT_H), NULL, (DLGPROC)DlgNextHours);
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
