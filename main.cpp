//#define UNICODE

#ifndef _WIN32_IE
  #define  _WIN32_IE 0x0400
#endif // _WIN32_IE

#include <windows.h>
#include <CommCtrl.h>

#include <stdio.h>
#include "resource.h"
#include "Models.h"
#include "EventManager.h"
#include "MessageManager.h"
#include "CDSetting.h"

HINSTANCE hInst;

std::string GetWindowText(HWND hwndDlg, int id) {
    TCHAR buffDataOd[1024];
    HWND hwndComponnent =       GetDlgItem(hwndDlg, id);
    GetWindowText(hwndComponnent, buffDataOd, 1024);
    return std::string(buffDataOd);
}

void RefreshListViewMessage(TypeParmT parm)
{
    HWND listView = parm.handle;
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
    int i = 0;
    ListView_DeleteAllItems(parm.handle);
    std::list<CMessage> message = CMessageManager::GetInstance().GetMessages();
    for (std::list<CMessage>::reverse_iterator it= message.rbegin(); it != message.rend(); it++) {
        char buffer[5];
        itoa(message.size()-1,buffer,10);

        lvi.pszText = const_cast<char * >(std::string(buffer).c_str());
        lvi.iItem = i;
        lvi.iSubItem = 0;

        ListView_InsertItem( listView, & lvi );
        ListView_SetItemText( listView, i, 1, const_cast<char *>(it->GetStrID().c_str()));
        ListView_SetItemText( listView, i, 2, const_cast<char *>(it->GetName().c_str()));
        ListView_SetItemText( listView, i, 3, const_cast<char *>(it->GetDescription().c_str()));

        i++;
    }
}

void AddLastItemListViewMessage(TypeParmT parm)
{
    HWND listView = parm.handle;
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
    std::list<CMessage> message = CMessageManager::GetInstance().GetMessages();
    int i = 0;
    char buffer[5];
    itoa(message.size()-1,buffer,10);

    lvi.pszText = const_cast<char * >(std::string(buffer).c_str());
    lvi.iItem = i;
    lvi.iSubItem = 0;

    ListView_InsertItem( listView, & lvi );
    ListView_SetItemText( listView, i, 1, const_cast<char *>(message.back().GetStrID().c_str()));
    ListView_SetItemText( listView, i, 2, const_cast<char *>(message.back().GetName().c_str()));
    ListView_SetItemText( listView, i, 3, const_cast<char *>(message.back().GetDescription().c_str()));

}

BOOL CALLBACK IddQueue(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND listView = GetDlgItem(hwndDlg,IDD_QUEUE_LISTVIEW);

        char lp[] = {"lp"};
        char nameClient[] = {"Klient"};
        char title[] = {"Tytu³"};
        char description[] = {"Opis"};

        LVCOLUMN lvc;
        lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

        lvc.iSubItem = 0;
        lvc.cx = 30;
        lvc.pszText = lp;
        ListView_InsertColumn( listView, 0, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 100;
        lvc.pszText = nameClient;
        ListView_InsertColumn( listView, 1, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 150;
        lvc.pszText = title;
        ListView_InsertColumn( listView, 2, & lvc );

        lvc.iSubItem = 0;
        lvc.cx = 250;
        lvc.pszText = description;
        ListView_InsertColumn( listView, 3, & lvc );

        ListView_SetExtendedListViewStyleEx(listView, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

        Event event;
        event.paem1.handle = listView;
        event.typeEvent = EVENT_GET_MESSAGE;
        event.handleFunction = &AddLastItemListViewMessage;
        CEventManager::getInstance().Subscribe(event);

        TypeParmT parm;
        parm.handle = listView;
        RefreshListViewMessage(parm);
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

    case WM_SIZE:
    {
        HWND listView = GetDlgItem(hwndDlg,IDD_QUEUE_LISTVIEW);
        MoveWindow(listView, 0, 0, LOWORD(lParam), HIWORD(lParam), true);
    }
    return true;

    }
    return FALSE;
}


void RefreshListViewClients(TypeParmT parm) {
    HWND listView = parm.handle;
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
    int i = 0;
    ListView_DeleteAllItems(parm.handle);
    std::list<SClient> clients = CModels::getInstance().GetClients();
    for (std::list<SClient>::iterator it= clients.begin(); it != clients.end(); it++) {
        char buffer[5];
        itoa(i+1,buffer,10);

        lvi.pszText = const_cast<char * >(std::string(buffer).c_str());
        lvi.iItem = i;
        lvi.iSubItem = 0;

        ListView_InsertItem( listView, & lvi );
        ListView_SetItemText( listView, i, 1, const_cast<char *>(it->GetStrName().c_str()));
        ListView_SetItemText( listView, i, 2, const_cast<char *>(it->GetStrSocketHandle().c_str()));
        ListView_SetItemText( listView, i, 3, const_cast<char *>(it->GetSrtIP().c_str()));
        ListView_SetItemText( listView, i, 4, const_cast<char *>(it->GetStrStatus().c_str()));
        i++;
    }
}

void InitComponents(HWND hwnd) {
    HWND listView = GetDlgItem(hwnd,DLG_MAIN_LISTVIEW);

    char lp[] = {"lp"};
    char name[] = {"Nazwa"};
    char id[] = {"ID"};
    char adressIP[] = {"IP"};
    char status[] = {"Status"};

    LVCOLUMN lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    lvc.iSubItem = 0;
    lvc.cx = 30;
    lvc.pszText = lp;
    ListView_InsertColumn( listView, 0, & lvc );

    lvc.iSubItem = 0;
    lvc.cx = 100;
    lvc.pszText = name;
    ListView_InsertColumn( listView, 1, & lvc );

    lvc.iSubItem = 0;
    lvc.cx = 100;
    lvc.pszText = id;
    ListView_InsertColumn( listView, 2, & lvc );

    lvc.iSubItem = 0;
    lvc.cx = 100;
    lvc.pszText = adressIP;
    ListView_InsertColumn( listView, 3, & lvc );

    lvc.iSubItem = 0;
    lvc.cx = 100;
    lvc.pszText = status;
    ListView_InsertColumn( listView, 4, & lvc );

    TypeParmT parm;
    parm.handle = listView;
    RefreshListViewClients(parm);

    Event event;
    event.paem1.handle = listView;
    event.typeEvent = EVENT_CLIENT_CONNECTION;
    event.handleFunction = &RefreshListViewClients;
    CEventManager::getInstance().Subscribe(event);
    event.typeEvent = EVENT_CLIENT_DISCONNECT;
    CEventManager::getInstance().Subscribe(event);
}

BOOL CALLBACK IddSetting(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hEditIP = GetDlgItem(hwndDlg,IDD_DIALOG_SETTING_IP);
        HWND hEditPort = GetDlgItem(hwndDlg,IDD_DIALOG_SETTING_PORT);
        SetWindowText( hEditIP, CDSetting::getInstance().getSetting().ipServer );
        SetWindowText( hEditPort, CDSetting::getInstance().getSetting().portServer );
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
            case IDOK: {
                std::string ip = GetWindowText(hwndDlg,IDD_DIALOG_SETTING_IP);
                std::string port = GetWindowText(hwndDlg,IDD_DIALOG_SETTING_PORT);
                CDSetting::getInstance().SetIp(ip);
                CDSetting::getInstance().SetPort(port);
                EndDialog(hwndDlg, 0);
                break;
            }
            case IDCANCEL: {
                EndDialog(hwndDlg, 0);
                break;
            }
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
    case WM_INITDIALOG:
    {
        InitComponents(hwndDlg);
    }
    return TRUE;

    case WM_CLOSE:
    {
        CDSetting::getInstance().Save();
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            case ID_QUEUE: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_QUEUE), NULL, (DLGPROC)IddQueue);
                break;
            }
            case ID_MENU_SETTING:
                {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SETTING), NULL, (DLGPROC)IddSetting);
                    break;
                }
            case ID_MENU_EXIT:
                {
                    CDSetting::getInstance().Save();
                    EndDialog(hwndDlg, 0);
                    break;
                }
        }
    }
    return TRUE;

    case WM_SIZE:
    {
        HWND listView = GetDlgItem(hwndDlg,DLG_MAIN_LISTVIEW);
        MoveWindow(listView, 0, 0, LOWORD(lParam), HIWORD(lParam), true);
    }
    return true;

    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    CDSetting::getInstance().Load();
    CModels::getInstance().Init();
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
