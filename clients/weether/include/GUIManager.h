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
        HWND hWndDialogMain;

        HBITMAP templateChart;

        void CreateTemplateChart(int width, int height) {
            char * buffer = new char[ width * height * 3];
            for ( int i = 0 ; i < width*height * 3; i++) buffer[i] = 50;

            templateChart = CreateBitmap(width, height, 1 , 24, buffer);
            //delete buffer;
        }

    protected:
    private:
        CGUIManager();
        virtual ~CGUIManager();

};

#endif // CGUIMANAGER_H
