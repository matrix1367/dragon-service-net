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
    nid.hWnd             = CGUIManager::getInstance().hWndDialogMain;
    nid.uID              = ID_TRAY1;
    nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP |NIF_INFO ;
    nid.uCallbackMessage = WM_APP;
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpy( nid.szTip, "Weather" );
    lstrcpy( nid.szInfoTitle, title.c_str());
    lstrcpy( nid.szInfo, info.c_str() );

    DataWeether dataWeether;
    CModels::getInstance().GetWeether(CDSetting::getInstance().getSetting().cityName, dataWeether);

    ExtractIconEx( dataWeether.getPathIcon().c_str() , 0, NULL, &(nid.hIcon), 1 );
    strcpy(nid.szTip, "Weater");
    Shell_NotifyIcon( NIM_MODIFY, &nid );
}

void CGUIManager::CreateTemplateChart(int width, int height) {
            if(!templateChart) DeleteObject(templateChart);
            unsigned bmp[width*height];
            const unsigned w=0xFFFFFF, b=0x000000, y=0xFFFF00;
            for ( int i = 0 ; i < width*height; i++) bmp[i] = w;

            templateChart =  CreateBitmap(width, height, 32, 1, bmp);
            //delete buffer;
        }
