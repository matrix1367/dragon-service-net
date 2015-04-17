#include "TasksManager.h"
#include "JobTime.h"

CJobsManager::CJobsManager()
{
    //ctor
}

CJobsManager::~CJobsManager()
{
    //dtor
}

IJob* CJobsManager::GetJob(int id) {
    switch (id) {
        case ID_JOB_TIME : {
            CJobTime job;
            jobs.push_back(&job);
            return &job;
        }
        default :
            {
                return 0;
            }
    }
}
