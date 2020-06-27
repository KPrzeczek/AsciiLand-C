#include "v2.h"

V2 V2_Make(int x, int y)
{
    V2 temp = { x, y };
    return temp;
}

int V2_Comp(V2 a, V2 b)
{
    return (a.x == b.x &&
            a.y == b.y);
}

void V2_Add(V2 *a, V2 b)
{
    a->x += b.x;
    a->y += b.y;
}

void V2_Sub(V2 *a, V2 b)
{
    a->x -= b.x;
    a->y -= b.y;
}

void V2_Mul(V2 *a, V2 b)
{
    a->x *= b.x;
    a->y *= b.y;
}

void V2_Div(V2 *a, V2 b)
{
    a->x /= b.x;
    a->y /= b.y;
}

void V2_Print(V2 a)
{
    printf("{ %d, %d }", a.x, a.y);
}

void V2_PrintLn(V2 a)
{
    printf("{ %d, %d }\n", a.x, a.y);
}