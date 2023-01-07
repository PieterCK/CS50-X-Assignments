#include <cs50.h>
#include <stdio.h>

void failed(void)
{
    printf("INVALID\n");
}
int main(void)
{
long crednum;
    do
    {
        crednum = get_long("Credit Card: \n");
    }
    while(crednum < 1 );
    long A = 0;
    long B = 0;
    int N1 = 1;
    long CR = crednum;
    do
    {
        // Luhn's Algorithm
        long n = CR / 10;
        long M = CR % 10 *0.1;
        long O = (n - M) * 10;
        long P = CR - O;
        long PA = P * 2;
        CR = n - M;
        if(PA > 9 && N1 == 0)
        {A = A + PA - 9;}
        else if(PA < 10 && N1 == 0)
        {A = A + PA;}
        else
        {B = B + P;}
        if(N1 == 0 )
        {N1++;}
        else
        {N1--;}
    }
    while(CR > 0);
    long C = A + B;
    //checks the number of digits, leading numbers, and validity of Luhn's Algo to conclude which credit card provider it is.
    if((crednum >= 340000000000000 && crednum <= 349999999999999 && C % 10 == 0) || (crednum >= 370000000000000 && crednum <= 379999999999999 && C % 10 == 0))
    {
        printf("AMEX\n");
    }
    else if(crednum >=5100000000000000 && crednum <= 5599999999999999 && C % 10 == 0)
    {
        printf("MASTERCARD\n");
    }
    else if((crednum >=4000000000000 && crednum <=49999999999999 && C % 10 == 0) || (crednum >=4000000000000000 && crednum <=4999999999999999 && C % 10 == 0))
    {
        printf("VISA\n");
    }
    else
    {   printf("INVALID\n");}
}