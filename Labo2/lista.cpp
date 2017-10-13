#include <iostream>
using namespace std;

struct mensaje{
    int TaskID;
    int Hola;
};

struct l_stNode{
    mensaje data;
    l_stNode *siguiente;
};

void insertNode(l_stNode *&lista, mensaje nuevoMensaje);
void eraseList(l_stNode *&lista, mensaje &nuevoMensaje);

int main()
{
    l_stNode *lista = NULL;
    mensaje nuevoMensaje;
    mensaje nuevoMensaje1 = {.TaskID = 1, .Hola = 2};
    mensaje nuevoMensaje2 = {.TaskID = 2, .Hola = 1};
    insertNode(lista, nuevoMensaje1);
    insertNode(lista, nuevoMensaje2);

    while(lista != NULL)
    {
        eraseList(lista, nuevoMensaje);
        cout<<nuevoMensaje.TaskID<< " -> ";
    }
    return 0;
}

void insertNode(l_stNode *&lista, mensaje nuevoMensaje){
    l_stNode *l_stNewNode = new l_stNode();
    l_stNewNode->data = nuevoMensaje;

    l_stNode *aux1 = lista;
    l_stNode *aux2;

    while((aux1 != NULL) && (aux1->data.TaskID < nuevoMensaje.TaskID)){
        aux2 = aux1;
        aux1 = aux1->siguiente;
    }

    if(lista == aux1)
    {
        lista = l_stNewNode;
    }else
    {
        aux2->siguiente = l_stNewNode;
    }

    l_stNewNode->siguiente = aux1;

    cout<<"\tElemento con TaskID "<<nuevoMensaje.TaskID<<" y Hola "<<nuevoMensaje.Hola<<" insertado a lista correctamente\n";
}

void eraseList(l_stNode *&lista, mensaje &nuevoMensaje){
    l_stNode *aux = lista;
    nuevoMensaje = aux->data;
    lista = aux->siguiente;
    delete aux;
}
