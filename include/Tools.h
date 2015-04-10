#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED
#include <vector>

//source function http://cpp0x.pl/artykuly/?id=64
std::vector <std:: string > split( char * napis, const char * ograniczniki = "\n\t " ) {
   std:: vector <std:: string > podzielony_napis;

    for( char * pch = strtok( napis, ograniczniki ); pch != NULL; pch = strtok( NULL, ograniczniki ) )
         podzielony_napis.push_back( pch );

    return podzielony_napis;
}

#endif // TOOLS_H_INCLUDED
