/// Evita que dataStructure se cargue más de una vez durante el preprocesamiento
#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE

// Nodo generico
struct Node
{
    void* value;
    struct Node* next;
    struct Node* before;
};

// Estructura para crear lista de diferentes tipos de datos
struct List
{
    // Puntero al primer nodo
    struct Node* head;
    // Puntero al ultimo nodo
    struct Node* tail;
    // tamaño de la lista
    int size;

    // Remueve un elemento
    int (*removeNode)(struct List* , struct Node*);

    // Obtiene el primer elemento
    struct Node* (*getHead)(struct List*);

    // Obtiene el ultimo elemento
    struct Node* (*getTail)(struct List*);

    // Verifica si esta vacia la lista
    int (*isEmpty)(struct List*);

    // Destructor para liberar memoria
    void (*destructor)(struct List*);

    // Añade un elemento al la lista
    int (*addNode)(struct List*, void*);

    
};

// Metodos

// Verifica si la lista esta vacia
int isEmpty(struct List* list);

// Remueve elementos de la lista
int removeNode(struct List* list, struct Node* node);

// Libera la memoria reservada por la lista
void destructor(struct List* list);

// Obtiene cabecera de la lista
struct Node* getHead(struct List* list);

// Obtiene cola de la lista
struct Node* getTail(struct List* list);

// Añade elementos a la lista
int addNode(struct List* list, void* value);


// Crea instancias de la estructura List
// @return (List*)
struct List* createList();

// Estructura para tener los duplicados de un archivo. Tambien, se puede decir que es una categoria o partición 
struct FilesDuplicates
{
    // Archivo que define la categoria
    char* file; 
    // Lista de archivos duplicados de la categoria
    struct List* duplicates; // (char*)
};

// Estructura para la estadistica
struct FileStatistics
{
    int numberDuplicates;
    // Lista de categorias
    struct List* Files; // Nodos de tipo FilesDuplicates*
};

#endif