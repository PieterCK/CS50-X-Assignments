#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void)
{
    string answer = get_string("What is my name? ");
    printf("Hello %s\n", answer);
}
