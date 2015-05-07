#include "TasksManager.h"
#include "JobAlertWeether.h"
#include "JobTime.h"




CJobsManager::CJobsManager()
{

}

CJobsManager::~CJobsManager()
{
    //dtor
}

std::vector<std::string> CJobsManager::GetStrAllNameJob() {
    std::vector<std::string> result;
    result.push_back(CJobTime::name);
    result.push_back(CJobAlertWeether::name);
    return result;
}

std::string CJobsManager::GetStrNameJob(int IdJob){
    switch (IdJob) {
        case ID_JOB_TIME : {
            return CJobTime::name;
        }
        case ID_JOB_ALERT_WATHER : {
            return CJobAlertWeether::name;
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
    } else if (indexJob == 1) {
        return ID_JOB_ALERT_WATHER;
    }
     return ID_JOB_TIME;
}

int CJobsManager::GetIndexJob(int IdJob) {
    if (IdJob == ID_JOB_TIME) {
        return 0;
    } else if (IdJob == ID_JOB_ALERT_WATHER) {
        return 1;
    }
     return 0;
}
