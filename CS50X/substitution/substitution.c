#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //check validity
    string Key = argv[1];

    if(argc !=2)
    {
        printf("./substitution Key 1\n");
        return 1;
    }
    if(Key[26] !='\0')
    {
        printf("Key Should be exactly 26 Characters\n");
        return 1;
    }
    int TrackNum = 0;
    int CheckNum;
    int ScoreNum;
    do
    {
        ScoreNum = 0;
        CheckNum = 0;
        do
        {
            if(tolower(Key[TrackNum]) == tolower(Key[CheckNum]))
            {ScoreNum++;}
            CheckNum++;
        }
        while(Key[CheckNum] != '\0');
        if(ScoreNum > 1)
        {
            printf("Key Should Only Contain Unique Characters\n");
            return 1;
        }
        if(isalpha(Key[TrackNum]))
        {}
        else
        {
            printf("Key Should be in Alphabets Only\n");
            return 1;
        }
        TrackNum++;
    }
    //cypher
    while(Key[TrackNum] != '\0');
    string plaintext = get_string("plaintext: ");
    int n = 0;
    int P;
    do
    {
        int A=0;
        P = plaintext[n];
        if(isalpha(P) && isupper(P))
        {
            A=tolower(P)-97;
            plaintext[n]=toupper(Key[A]);
        }
        else if(isalpha(P) && islower(P))
        {
            A=P-97;
            plaintext[n]=tolower(Key[A]);
        }
        else
        {plaintext[n]= P;}
    n++;
    }
    while(P != '\0');
    printf("ciphertext: %s\n", plaintext);
}

