#include "Models.h"

void CModels::Init(void) {
    if(client.Init())
    {
        client.Connect();
    }
}

CClient& CModels::GetClient()
{
    return client;
}
