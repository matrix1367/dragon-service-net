#ifndef CGUIMANAGER_H
#define CGUIMANAGER_H

#ifndef _WIN32_IE
#define  _WIN32_IE 0x600
#endif // _WIN32_IE

#include <windows.h>
#include <string>
#include "../resource.h"



class CGUIManager
{
    public:
      static CGUIManager& getInstance() {
         static CGUIManager instance;
         return instance;
      }
      void ShowMessage(std::string title, std::string info);
        void SetHWindowMain(HWND hwnd);
        HWND GetHWindowMain();


    protected:
    private:
        CGUIManager();
        virtual ~CGUIManager();
        HWND hWindowMain;
};

#endif // CGUIMANAGER_H
