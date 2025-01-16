/// Evita que dataStructure se cargue mas de una vez durante el preprocesamiento
#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE

// Nodo generico
struct Node
{
    void *value;
    struct Node *next;
    struct Node *before;
};

// Estructura para crear lista de diferentes tipos de datos
struct List
{
    //* puntero al primer nodo
    struct Node *head;
    struct Node *tail;

    //* Remueve un elemento
    int (*removeNode)(struct List *, struct Node *);

    //* Obtiene el primer elemento
    struct Node *(*getHead)(struct List *);

    //* Obtiene el ultimo elemento
    struct Node *(*getTail)(struct List *);

    //* Verifica si esta vacia la lista
    int (*isEmpty)(struct List *);

    //* Destructor para liberar memoria
    void (*destructor)(struct List *);

    //* Añade un elemento al la lista
    int (*addNode)(struct List *, void *);

    //* Añade elementos de otra lista y los elimina de ella
    int (*moveNodeToMine)(struct List *, struct List *);
};

// Metodos

// Verifica si la lista esta vacia
int isEmpty(struct List *list);

// Remueve elementos de la lista
int removeNode(struct List *list, struct Node *node);

// Libera la memoria reservada por la lista
void destructor(struct List *list);

// Obtiene cabecera de la lista
struct Node *getHead(struct List *list);

// Obtiene cola de la lista
struct Node *getTail(struct List *list);

// Añade elementos a la lista
int addNode(struct List *list, void *value);

// Mueve los elementos y vacia la lista
// @param from
// @return (int)
int moveNodeToMine(struct List *me, struct List *from);

// Crea instancias de la estructura List
// @return (List*)
struct List *createList();

// Estructura para archivos duplicados
struct FilesDuplicates
{
    char *file;
    // Lista para llevar registro de los archivos duplicados
    struct List *duplicates; // * (char*)
};

// Estructura para la estadistica
struct FileStatistics
{
    int numberDuplicate;
    // Lista de Archivos duplicados
    struct List *Files; // * FilesDuplicates*
};

#endif