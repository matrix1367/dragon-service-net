#include "Models.h"

void CModels::Init(void) {
    if(server.Init()) {
        server.Start();
    }
}
