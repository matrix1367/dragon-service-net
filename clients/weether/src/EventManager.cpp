#include "EventManager.h"

CEventManager::CEventManager()
{
    //ctor
}

CEventManager::~CEventManager()
{
    //dtor
}

void CEventManager::Send(TYPE_EVENT event)
{
    this->OnEvent(event);
}

void CEventManager::Subscribe(Event eventData)
{
    events.push_back(eventData);
}

void CEventManager::UnSubscribe(Event eventData)
{
    //nie testowaane
    for ( unsigned int i = 0 ; i < events.size(); i++) {
        if (events[i].handleFunction == eventData.handleFunction) {
            events.erase(events.begin() + i);
            break;
        }
    }
}

void CEventManager::OnEvent(TYPE_EVENT event)
{
    for (unsigned int i = 0 ; i < events.size(); i++) {
        if (events[i].typeEvent == event) {
            events[i].handleFunction(events[i].paem1);
        }
    }
}
