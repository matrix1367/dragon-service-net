#ifndef CTASKSMANAGER_H
#define CTASKSMANAGER_H
#include "IJob.h"
#include "job/JobTime.h"
#include <vector>

class CJobsManager
{
    public:
        static CJobsManager& getInstance() {
            static CJobsManager insctance;
            return insctance;
        }

        IJob* GetJobs(int id) {
                switch (id) {
                    case ID_JOB_TIME : {
                        IJob* job = new CJobTime();
                        return job;
                    }
                    default :
                    {
                        IJob* job = new CJobTime();
                        return job;
                    }
                }
        }

        std::vector<std::string> GetStrAllNameJob();
        std::string GetStrNameJob(int IdJob);
        int GetIdJob(int indexJob);

    protected:
    private:
        CJobsManager();
        virtual ~CJobsManager();
        void operator= (CJobsManager const&);

};

#endif // CTASKSMANAGER_H
