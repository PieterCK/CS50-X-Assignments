#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int Count_Words(string Input);
int Count_Letters(string Input);
int Count_Sentences(string Input);
int Determine_Formula(float Wo, float Le, float Sen);
int main(void)
{
//Ask for Input
    string Input = get_string("Text: ");
//Val =
    int Word = Count_Words( Input);
    int Letter = Count_Letters( Input);
    int Sentences = Count_Sentences( Input);
    int Val = Determine_Formula(Word, Letter, Sentences);

    if(Val < 1)
    {
      printf("Before Grade 1\n");
    }
    else if(Val > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", Val);
    }
}
//Count words inside "Input" (A)
int Count_Words(string Input)
{
    int Count1 = 0;
    int Count11 = 1;
    do
    {
        if(Input[Count1] == 32)
        {Count11++;}
        Count1++;
    }
    while(Input[Count1] != '\0');
    printf("# of Words %i\n", Count11);
    return Count11;
}
//Count letters inside "Input" (B) Not Yet
int Count_Letters(string Input)
{
    int Count2 = 0;
    int Count22 = 0;
    do
    {
        int W1;
        W1 = tolower(Input[Count2]);
        if(W1>96 && W1<123)
        {Count22++;}
        Count2++;
    }
    while(Input[Count2] != '\0');
    printf("# of Letters %i\n", Count22);
    return Count22;
}
//Count sentences inside "Input"(C) Not Yet
int Count_Sentences(string Input)
{
    int Count3 = 0;
    int Count33 = 0;
    do
    {
        if(Input[Count3] == 33 || Input[Count3] == 46 || Input[Count3] == 63)
        {Count33++;}
        Count3++;
    }
    while(Input[Count3] != '\0');
    printf("# of Sentences %i\n", Count33);
    return Count33;
}
//Calculate Formula
int Determine_Formula(float Wo, float Le, float Sen)
    {
    float I = (0.0588 * (Le / Wo * 100))-(0.296 * (Sen / Wo * 100))-15.8;
    return round(I);
    }

