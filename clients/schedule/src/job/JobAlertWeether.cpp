#include "JobAlertWeether.h"
#include "WebTool.h"
#include  "GUIManager.h"

std::string  CJobAlertWeether::name = "Alert weether";

CJobAlertWeether::CJobAlertWeether() : IJob(ID_JOB_ALERT_WATHER)
{
    strcpy(m_parm1.str,"Poznań");
}

CJobAlertWeether::~CJobAlertWeether()
{
    //dtor
}

void Convert(const std::string& str1 , std::string & str2) {
  //  int wchars_num =  MultiByteToWideChar( CP_UTF8 , 0 , str1.c_str() , -1, NULL , 0 );
    //wchar_t* wstr = new wchar_t[wchars_num];
  //  char* wstr = new char[wchars_num];
   // MultiByteToWideChar( CP_UTF8 , 0 , str1.c_str() , -1, wstr , wchars_num );
    // do whatever with wstr
    //delete[] wstr;
  //  str2 = std::string(wstr);

   for (unsigned int ii = 0 ; ii < str1.length() ; ii++) {
       if (str1[ii] == -59 ) {
            continue;
       } else if (str1[ii] == -60) {
            continue;
       } else if (str1[ii] == -61) {
            continue;
       } else if (str1[ii] == -124)
       {
           str2+= "ñ"; //ni
       } else if (str1[ii] == -68) {
            str2+= "¿"; //z
       } else if (str1[ii] == -77) {
            str2+= "ó"; //o
       } else if  (str1[ii] == -123) {
           str2+= "¹"; //a
       } else if  (str1[ii] == -126) {
           str2+= "³"; //l
       }
       else {
        str2 += str1[ii];
       }
   }


}

void RemoveHtmlTag(std::string &text) {
    const std::string tagBegin = "<";
    const std::string tagEnd = ">";

    std::string::size_type indexBegin= text.find(tagBegin,0);
    while (indexBegin != std::string::npos)
    {
        std::string::size_type indexEnd = text.find(tagEnd, indexBegin);
        if (indexBegin != std::string::npos && indexEnd != std::string::npos) {
            text.erase(indexBegin, indexEnd - indexBegin+1);
        }
        indexBegin= text.find(tagBegin,0);
    }
}

std::string CJobAlertWeether::Parser(std::string page)
{
    std::string result = "Error page parser";
    std::string city = "";

    const std::string strSt = "target=\"_blank\">";
    std::string::size_type indexSt =0;
	indexSt = page.find(strSt, indexSt+1);

	if (indexSt != std::string::npos) {
        const std::string aEnd = "</a>";
        std::string::size_type indexEnd = page.find(aEnd, indexSt);
        if (indexEnd != std::string::npos)
		{
			city = page.substr(indexSt+ strSt.length(), indexEnd-indexSt - strSt.length());
			std::string convertCity ="" ;
			Convert(city, convertCity);
			//printf("img http: %s\n" , city.c_str());

			const std::string tabSt = "\">";
			const std::string tabEnd = "</dl>";
			std::string::size_type indexTabSt;
			std::string::size_type indexTabEnd;
			result = "Alert: " + convertCity ;

			do {
               indexTabSt = page.find(tabSt, indexEnd+1);
              // printf("<>>>>indexTabSt>>>>>>:%d", indexTabSt);
               if (indexTabSt != std::string::npos) {
                indexTabEnd = page.find(tabEnd, indexTabSt) ;
                std::string sub = page.substr(indexTabSt+ tabSt.length(), indexTabEnd-indexTabSt - tabSt.length());
                RemoveHtmlTag(sub);
                std::string subConvert = "";
                Convert(sub, subConvert);
               // for (unsigned int ii = 0 ; ii < sub.length() ; ii++) {
              //      printf("%c %d ; " ,sub[ii] , sub[ii]);
               // }

               //  printf("\n\n");
                indexEnd = indexTabEnd;
                result+= std::string(", ") + subConvert.c_str() ;

               }
			} while (std::string::npos != indexTabSt);


		}
	}
    //printf("<<<<%s>>>>", result.c_str());
    return result;
}

TYPE_MESSAGE CJobAlertWeether::GetTypeMessage(std::string description)
{
    const std::string warningStr = "I stopnia";
    std::string::size_type indexTabSt = description.find(warningStr, 0);
    if (indexTabSt != std::string::npos) {
        const std::string warningStrII = "II stopnia";
        std::string::size_type indexTabSt = description.find(warningStrII, 0);
        if (indexTabSt != std::string::npos) {
            const std::string waringStrIII = "III stopnia";
            std::string::size_type indexTabSt = description.find(waringStrIII, 0);
             if (indexTabSt != std::string::npos) {
                    CGUIManager::getInstance().ShowMessage("Alert pogodowy", std::string("Zagrożenie pogodowe III sopnia w miejscowości ") + m_parm1.str);
                 return TYPE_MESSAGE_MORE_IMPORTANT_SEND;
             }
             CGUIManager::getInstance().ShowMessage("Alert pogodowy", std::string("Zagrożenie pogodowe II sopnia w miejscowości ") + m_parm1.str);
            return TYPE_MESSAGE_IMPORTANT_SEND;
        }
        CGUIManager::getInstance().ShowMessage("Alert pogodowy", std::string("Zagrożenie pogodowe I sopnia w miejscowości ") + m_parm1.str);
        return  TYPE_MESSAGE_SEND;
    }

    return TYPE_MESSAGE_NO_SEND;
}



CMessage CJobAlertWeether::Run()
{
    CWebTool webTool;
    std::string page = webTool.getPage(std::string(std::string("http://burze.dzis.net/ramka.php?miejscowosc=") + std::string(m_parm1.str) + std::string("&promien=25")).c_str() );
   // Parser(page);

    std::string description = Parser(page);
    CMessage message("Alert pogodowy", description);
    message.SetTypeTask(this->GetTypeMessage(description));
    return message;
}