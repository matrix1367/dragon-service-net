#ifndef CMODELS_H
#define CMODELS_H
#include <list>
#include "Server.h"

class CModels
{
    public:
        static CModels& getInstance() {
            static CModels instance;
            return instance;
        }

        void Init(void);
        std::list<SClient> GetClients();
        void RmoveMessage(unsigned int index);
    protected:
    private:
        CModels() {}
        virtual ~CModels() {}
        void operator= (CModels const&);


};

#endif // CMODELS_H
