# ifndef _List_H

/* this definition is temporary */
typedef int element_type;

struct node;
typedef struct node *ptr2node;
typedef ptr2node list;
typedef ptr2node position;

list make_empty(list l);

int is_empty(list l);
int is_last(list l);

position find(list l);
position find_previous(list l);

// typedef int ElementType;

// struct Node;
// typedef struct Node *PtrToNode;
// typedef PtrToNode List;
// typedef PtrToNode Position;

// List MakeEmpty(List L);

// int IsEmpty(List L);
// int IsLast(Position P, List L);

// Position Find(ElementType X, List L);
// Position FindPrevious(ElementType X, List L);

// void Delete(ElementType X, List L);
// void Insert(ElementType X, List L, Position P);

// void DeleteList(List L);

// Position Header(List L);
// Position First(List L);
// Position Advance(Position P);
// ElementType Retrieve(Position P);

# endif /* _List_H */