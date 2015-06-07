#include "GUIManager.h"

#include "Convert.h"
#include "Models.h"
#include "CDSetting.h"


CGUIManager::CGUIManager()
{
    //ctor
}

CGUIManager::~CGUIManager()
{
    //dtor
}


void CGUIManager::ShowMessage(std::string title, std::string info)
{
    NOTIFYICONDATA   nid;
    nid.cbSize = sizeof( NOTIFYICONDATA );
    nid.hWnd             = CGUIManager::getInstance().GetHWindowMain();
    nid.uID              = ID_TRAY1;
    nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP |NIF_INFO ;
    nid.uCallbackMessage = WM_APP;
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpy( nid.szTip, "Terminarz" );
    lstrcpy( nid.szInfoTitle, title.c_str());
    lstrcpy( nid.szInfo, info.c_str() );
     ExtractIconEx( "img\\schedul-icon.ico", 0, NULL, &(nid.hIcon), 1 );
    //ExtractIconEx( dataWeether.getPathIcon().c_str() , 0, NULL, &(nid.hIcon), 1 );
    strcpy(nid.szTip, "Terminarz");
    Shell_NotifyIcon( NIM_MODIFY, &nid );
}

void CGUIManager::SetHWindowMain(HWND hwnd)
{
    hWindowMain = hwnd;
}

HWND CGUIManager::GetHWindowMain()
{
    return hWindowMain;
}

