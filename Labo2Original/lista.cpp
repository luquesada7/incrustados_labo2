#include <iostream>
#include "Strct.hpp"
using namespace std;

void insertNode(st_Node *&st_pLinkedList, st_Message l_stNewMessage);
void eraseFirstNode(st_Node *&st_pLinkedList, st_Message &l_stNewMessage);

int main()
{
    st_Node *st_pLinkedList = NULL;
    st_Message l_stNewMessage;
    st_Message l_stNewMessage1 = {.std_u8DestTaskID = 1, .std_u16IntData = 2};
    st_Message l_stNewMessage2 = {.std_u8DestTaskID = 2, .std_u16IntData = 1};
    insertNode(st_pLinkedList, l_stNewMessage1);
    insertNode(st_pLinkedList, l_stNewMessage2);

    while(st_pLinkedList != NULL)
    {
        eraseFirstNode(st_pLinkedList, l_stNewMessage);
        cout<<l_stNewMessage.std_u8DestTaskID<< " -> ";
    }

    return 0;
}

// ##############################################
// Function that inserts new node on linked list
// - creates new node
// - saves message structure to node
// - links node to list ordering them in ascending
//   order with the DestTaskID of the message
// ##############################################

void insertNode(st_Node *&st_pLinkedList, st_Message l_stNewMessage){
    st_Node *l_stNewNode = new st_Node();
    l_stNewNode->std_stMssg = l_stNewMessage;

    st_Node *l_stAux1 = st_pLinkedList;
    st_Node *l_stAux2;

    // -while list is not empty and DestTaskID of first node is smaller than the new message DestTaskID
    while((l_stAux1 != NULL) && (l_stAux1->std_stMssg.std_u8DestTaskID < l_stNewMessage.std_u8DestTaskID)){
        l_stAux2 = l_stAux1; //- points to first node
        l_stAux1 = l_stAux1->std_pnext; //- new position
    }

    if(st_pLinkedList == l_stAux1)
    {
        st_pLinkedList = l_stNewNode; //- adding node at the beggining of the list
    }else
    {
        l_stAux2->std_pnext = l_stNewNode; // -adding node in the middle or back of the list
    }

    l_stNewNode->std_pnext = l_stAux1;

    cout<<"\tElemento con std_u8DestTaskID "<<l_stNewMessage.std_u8DestTaskID<<" y std_u16IntData "<<l_stNewMessage.std_u16IntData<<" insertado a st_pLinkedList correctamente\n";
}

// ##############################################
// Function that erases a node on a linked list
// - Creates a temporary node to store the node
//   on the list that will be deleted
// - Pointer to list then points to the second
//   node
// - Temporary node is deleted
// ##############################################

void eraseFirstNode(st_Node *&st_pLinkedList, st_Message &l_stMessage){
    st_Node *l_stAux = st_pLinkedList;
    l_stMessage = l_stAux->std_stMssg;
    st_pLinkedList = l_stAux->std_pnext;
    delete l_stAux;
}
