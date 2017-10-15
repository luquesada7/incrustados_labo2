#ifndef TASK_HPP_
#define TASK_HPP_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Strct.hpp"

#define NO_ERR 0
#define RET_ERR 1
#define NUMBER_OF_SLOTS 255
#define NULL            0

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern Task *g_aTaskPointers[NUMBER_OF_SLOTS];

class Task
{
  public:
    Task();
    uint8_t m_u8TaskID;
    virtual uint8_t run(void){return(0);};
    virtual uint8_t setup(void){return(0);};
    virtual uint8_t readMessage(st_Message *l_stNewMessage){return(0);};
    virtual uint8_t sendMessage(st_Message *l_stNewMessage){return(0);};
    char *getKey(void){return m_pKey;};
    uint8_t setKey(char *l_pKey){return(0);};
    bool getMssgFlag(){return m_bMssgFlag;};
    bool getRunFlag(){return m_bRunFlag;};
    bool IsTaskFinished(void){return m_bIsFinished;};
    //uint8_t             GetTaskPriority(void) {return m_u8Priority;};
    //void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
  private:
    char *m_pKey;
    st_Message m_stMssg;
    static uint8_t m_u8NextTaskID;
    bool m_RunFlag; // - Flag: Task needs to be executed in next frame
    bool m_MssgFlag; // - Flag: Task has a message to send
  protected:
    bool m_bIsFinished;
};

#endif /* TASK_HPP_ */
