#ifndef CMODELS_H
#define CMODELS_H
#include "Client.h"

class CModels
{
    public:
        static CModels& getInstance() {
            static CModels instance;
            return instance;
        }

        void Init(void);
        CClient& GetClient();

    protected:
    private:
        CModels() {}
        virtual ~CModels() {}
        void operator= (CModels const&);

        CClient client;
};

#endif // CMODELS_H
