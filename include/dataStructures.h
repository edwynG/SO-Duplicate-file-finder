/// Evita que dataStructure se cargue mas de una vez durante el preprocesamiento
#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE

//? Nodo generico
struct Node
{
    void *value;
    struct Node *next;
    struct Node *before;
};

//? Lista generica
// Estructura para crear lista de diferentes tipos de datos
struct List
{
    //* puntero al primer nodo
    struct Node *head;
    struct Node *tail;

    //* funcion para remover un elemento
    int (*removeNode)(struct List *, struct Node *);

    //* funcion para obtener el primer elemento
    struct Node *(*getHead)(struct List *);

    //* funcion para obtener el ultimo elemento
    struct Node *(*getTail)(struct List *);

    //* verifica si esta vacia la lista
    int (*isEmpty)(struct List *);

    //* destructor para liberar memoria
    void (*destructor)(struct List *);

    //* Añade un elemento al la lista
    int (*addNode)(struct List *, void *);

    //* Añade elementos de otra lista y los elimina de ella
    int (*moveNodeToMy)(struct List *, struct List *);
};

//* Metodos

int isEmpty(struct List *list);
int removeNode(struct List *list, struct Node *node);
void destructor(struct List *list);
struct Node *getHead(struct List *list);
struct Node *getTail(struct List *list);
int addNode(struct List *list, void *value);

//* metodo para mover nodos de otra listas a la lista propia.
// Mueve los elementos y vacia la lista
// @param to from
// @return int (0 o 1)
int moveNodeToMy(struct List *me, struct List *from);


//! Función para crear una lista
// Esta función tiene el proposito de crear instancias de la estructura List
// @return List*
struct List *createList(); //! USAR SI SE DESEA CREAR UNA LISTA


#endif