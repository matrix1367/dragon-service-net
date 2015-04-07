#ifndef CMODELS_H
#define CMODELS_H
#include "Server.h"

class CModels
{
    public:
        static CModels& getInstance() {
            static CModels instance;
            return instance;
        }

        void Init(void);

    protected:
    private:
        CModels() {}
        virtual ~CModels() {}
        void operator= (CModels const&);

        CServer server;
};

#endif // CMODELS_H
