#ifndef VEC2_H
#define VEC2_H

#include <stdio.h>

/*
** Simple Vec2 struct along with some util funcs
*/

typedef struct V2 {
    int x;
    int y;
} V2;

V2 V2_Make(int x, int y);
int V2_Comp(V2 a, V2 b);

void V2_Add(V2 *a, V2 b);
void V2_Sub(V2 *a, V2 b);
void V2_Mul(V2 *a, V2 b);
void V2_Div(V2 *a, V2 b);

void V2_Print(V2 a);
void V2_PrintLn(V2 a);

#endif //VEC2_H
