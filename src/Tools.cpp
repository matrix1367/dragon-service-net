#include "Tools.h"

std::vector <std:: string > split( char * napis, const char * ograniczniki  ) {
   std:: vector <std:: string > podzielony_napis;

    for( char * pch = strtok( napis, ograniczniki ); pch != NULL; pch = strtok( NULL, ograniczniki ) )
         podzielony_napis.push_back( pch );

    return podzielony_napis;
}
