#include "TasksManager.h"

CJobsManager::CJobsManager()
{

}

CJobsManager::~CJobsManager()
{
    //dtor
}

std::vector<std::string> CJobsManager::GetStrAllNameJob() {
    std::vector<std::string> result;
    result.push_back("Time");

    return result;
}

std::string CJobsManager::GetStrNameJob(int IdJob){
    switch (IdJob) {
        case ID_JOB_TIME : {
            return "Time";
        }
        default:
        {
             return "ID JOB ERROR";
        }
    }
}

int CJobsManager::GetIdJob(int indexJob) {
    if (indexJob == 0) {
        return ID_JOB_TIME;
    }
     return ID_JOB_TIME;
}
