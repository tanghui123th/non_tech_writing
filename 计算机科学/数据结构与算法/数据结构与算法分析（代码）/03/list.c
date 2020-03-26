#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct Node
{
    ElementType Element;
    Position Next;
};

int IsEmpty(List L)
{
    /* use dummy head */
    return L->Next == NULL;
}

int IsLast(Position P, List L)
{
    return P->Next == NULL;
}

Position Find(ElementType X, List L)
{
    /* If not found, return NULL */
    Position P;

    P = L->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;
    return P;
}

Position FindPrevious(ElementType X, List L)
{
    /* return last element if x is not found */
    Position P;

    P = L;
    while (P->Next != NULL && P->Next->Element != X)
        P = P->Next;
    return P;
}

void Insert(ElementType X, List L, Position P)
{
    Position TmpCell;

    TmpCell = malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        printf("error in %s: %s, Out of space!!!", __FILE__, __FUNCTION__);
    TmpCell->Element = X;
    TmpCell->Next = P->Next;
    P->Next = TmpCell;
}

void Delete(ElementType X, List L)
{
    /* delete first X 
     * if X not in List, do nothing
     */
    Position P, TmpCell;

    P = FindPrevious(X, L);
    if (!IsLast(P, L)) {
        TmpCell = P->Next;
        P->Next = TmpCell->Next;
        free(TmpCell);
    }
}

void DeleteList(List L)
{
    Position P, Tmp;

    P = L->Next;
    while (P != NULL) {
        Tmp = P->Next;
        free(P);
        P = Tmp;
    }
}