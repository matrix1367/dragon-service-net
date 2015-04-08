#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H
#include <vector>
#include <windows.h>

union TypeParmT
{
    HWND handle;
};


typedef void ( * OnEventT ) (TypeParmT) ;

enum TYPE_EVENT {
    EVENT_CLIENT_CONNECTION,
    EVENT_CLIENT_DISCONNECT
};


typedef struct EventT {
    TYPE_EVENT typeEvent;
    OnEventT handleFunction;
    TypeParmT paem1;
} Event;


class CEventManager
{
    public:
       static CEventManager& getInstance() {
        static CEventManager instance;
        return instance;
       }
       void Send(TYPE_EVENT event);
       void Subscribe(Event eventData);
       void UnSubscribe(Event eventData);

    protected:
    private:
        std::vector<Event> events;
        CEventManager();
        virtual ~CEventManager();
         void OnEvent(TYPE_EVENT event);
};

#endif // CEVENTMANAGER_H
