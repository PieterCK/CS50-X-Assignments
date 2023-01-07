#include <cs50.h>
#include <stdio.h>
void wall(int M)
{
    for (int Z = 0; Z < M; Z++)
    {
        printf("#");
    }
}
void spaces(int M1)
{
    for (int Z1 = 0; Z1 < M1; Z1++)
    {
        printf(" ");
    }
}
int main(void)
{
    int ask;
    do
    {
        ask = get_int("Height: ");
    } while (ask < 1 || ask > 8);

    for (int n = 1; n < ask + 1; n++)
    {
        spaces(ask - n);
        wall(n);
        printf("  ");
        wall(n);
        printf("\n");
    }
}