#ifndef CTASKSMANAGER_H
#define CTASKSMANAGER_H
#include "IJob.h"
#include <vector>

class CJobsManager
{
    public:
        static CJobsManager& getInstance() {
            static CJobsManager insctance;
            return insctance;
        }
        IJob* GetJob( int id);
    protected:
    private:
        CJobsManager();
        virtual ~CJobsManager();
        void operator= (CJobsManager const&);
        std::vector<IJob*> jobs;
};

#endif // CTASKSMANAGER_H
