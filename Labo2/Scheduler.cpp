#include "Scheduler.hpp"

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    m_u8TaskCounter = 0;
    for (int index1 = 0; index1 < NUMBER_OF_SLOTS; index1++) {
        m_aSchedule[index1].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }
    return;
}

// ##############################################
// Function that attaches functions, inserts the
// task into the schedule slots
// ##############################################

uint8_t Scheduler::attach(Task *i_ToAttach, uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    st_TaskInfo l_st_StructToAttach;

    l_st_StructToAttach.pToAttach = i_ToAttach;
    l_st_StructToAttach.pToKey = i_ToAttach->getKey();
    l_st_StructToAttach.u64ticks = this->m_u64ticks;
    l_st_StructToAttach.u64TickInterval = 0;
	l_st_StructToAttach.u64TickIntervalInitValue = i_u64TickInterval;

    if((m_u8OpenSlots>0) && (m_u8NextSlot < NUMBER_OF_SLOTS)) {
        m_aSchedule[m_u8NextSlot] =  l_st_StructToAttach;
        m_u8OpenSlots--;
        m_u8NextSlot++;
        g_aTaskPointers[m_u8NextSlot] = i_ToAttach;
        m_aTaskInfoStructs[m_u8NextSlot] = l_st_StructToAttach;
    } else {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}

// ##########################################
// Function that executes current schedule
// ##########################################

uint8_t Scheduler::run(void)
{
    int l_iNextTaskSlot = 0U;
    Task *l_pNextTask = NULL;
    uint8_t l_u8ReturnCode = NO_ERR;

    while (l_iNextTaskSlot < NUMBER_OF_SLOTS) {
      l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);

      if (l_pNextTask != NULL) {
        if (m_aSchedule[l_iNextTaskSlot].u64TickInterval == 0) {
          l_pNextTask->run();
        }
        m_aSchedule[l_iNextTaskSlot].u64TickInterval ++;

        if (m_aSchedule[l_iNextTaskSlot].u64TickInterval > m_aSchedule[l_iNextTaskSlot].u64TickIntervalInitValue) {
          m_aSchedule[l_iNextTaskSlot].u64TickInterval = 0;
        }
      }

      l_iNextTaskSlot++;
    }
    CollectMessages();
    DistributeMessages();
    CalculateNextSchedule();

    // TODO: m_aSchedule = m_aNextSchedule
    //       Ressolve TaskID

    return l_u8ReturnCode;
}

// ##############################################
// Function that executes setup for all the tasks
// ##############################################

uint8_t Scheduler::setup(void)
{
    int l_iNextTaskSlot = 0U;
    Task *l_pNextTask = NULL;
    uint8_t l_u8ReturnCode = NO_ERR;

    while (l_iNextTaskSlot < NUMBER_OF_SLOTS) {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
        if (l_pNextTask != ((uintptr_t) 0)) {
            l_pNextTask->setup();
        }
        l_iNextTaskSlot++;
    }

    return l_u8ReturnCode;
}

uint8_t Scheduler::CalculateNextSchedule(void)
{
    Task * l_pNextTask = NULL;
    uint8_t l_u8Schedule = 0;
    char *l_pKey = NULL;
    st_TaskInfo l_st_StructToAdd;

    memset(m_aSchedule, 0, NUMBER_OF_SLOTS*sizeof(struct st_TaskInfo));


    for (int l_iCounter = 0; l_iCounter < NUMBER_OF_SLOTS; l_iCounter++) {
        l_pNextTask = g_aTaskPointers[l_iCounter];
        if (l_pNextTask->getRunFlag()) {
            l_pKey = l_pNextTask.getKey();
            for(int l_iCounter = 0; l_iCounter < NUMBER_OF_SLOTS; l_iCounter++){
                l_st_StructToAdd = m_aTaskInfoStructs[l_iCounter];
                if(l_st_StructToAdd.pToKey == l_pKey){
                    m_aSchedule[l_u8Schedule] = l_st_StructToAdd;
                    l_u8Schedule++;
                }
            }
        }
    }
    return(NO_ERR);
}

uint8_t Scheduler::CollectMessages(void)
{
    Task *l_pNextSender = NULL;

    for(int l_iCounter; l_iCounter < NUMBER_OF_SLOTS; l_iCounter++) {
        l_pNextSender = g_aTaskPointers[l_iCounter];
        if (l_pNextSender->getMssgFlag()) {
            st_Message l_stNewMessage;
            l_pNextSender->sendMessage(&l_stNewMessage);
            InsertNode(m_pLinkedList, l_stNewMessage);
        }
    }
    return(NO_ERR);
}

uint8_t Scheduler::DistributeMessages(void)
{
    st_Message l_stNewMessage;

    while(m_pLinkedList != NULL) {
            DistributeEraseFirstNode(m_pLinkedList, l_stNewMessage);
    }
    return(NO_ERR);
}

// ##############################################
// Function that inserts new node on linked list
// - creates new node
// - saves message structure to node
// - links node to list ordering them in ascending
//   order with the DestTaskID of the message
// ##############################################

uint8_t Scheduler::InsertNode(st_Node *&l_pLinkedList, st_Message l_stNewMessage)
{
    st_Node *l_stNewNode = new st_Node();
    l_stNewNode->std_stMssg = l_stNewMessage;

    st_Node *l_stAux1 = l_pLinkedList;

    l_pLinkedList = l_stNewNode; // -adding node at the beginning of the list
    l_stNewNode->std_pnext = l_stAux1;

    return(NO_ERR);
}

// ##############################################
// Function that erases a node on a linked list
// - Creates a temporary node to store the node
//   on the list that will be deleted
// - Current message is distributed to Task
// - Pointer to list then points to the second
//   node
// - Temporary node is deleted
// ##############################################

uint8_t Scheduler::DistributeEraseFirstNode(st_Node *&l_pLinkedList, st_Message &l_stNewMessage)
{
    st_Node *l_stAux = l_pLinkedList;
    l_stMessage = l_stAux->std_stMssg;
    char *l_pKey = l_stMessage.std_pDestKey;
    Task *l_pTaskPointer = NULL;

    for(int l_iCounter = 0; l_iCounter < NUMBER_OF_SLOTS; l_iCounter++){
        l_pTaskPointer = g_aTaskPointers[l_iCounter];
        if(l_pTaskPointer->getKey() == l_pKey){
            l_pTaskPointer->readMessage(l_stMessage);
        }
    }
    l_pLinkedList = l_stAux->std_pnext;
    delete l_stAux;

    return(NO_ERR);
}
