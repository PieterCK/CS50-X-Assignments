#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
typedef uint8_t BYTE;
const int block_size = 512;
int main(int argc, char *argv[])
{
    // make sure argc is only 2
    if (argc != 2)
    {
        printf("Usage : ./recover IMAGE\n");
        return 1;
    }

    // fread the inputed file
    FILE *input = fopen(argv[1], "r");
    FILE *output = NULL;
    char filename[8];

    if (input == NULL)
    {
        printf("Usage : ./recover IMAGE\n");
        return 1;
    }

    BYTE buffer[block_size];
    int n = 0;
    bool jpg_bytes = false;

    while (fread(buffer, sizeof(BYTE), block_size, input))
    {if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {if (output != NULL)
            {fclose(output);}
            sprintf(filename, "%03i.jpg", n);
            output = fopen(filename, "w");n++;
            jpg_bytes = true;
        }
        if (jpg_bytes == true)
        {fwrite(buffer, sizeof(BYTE), block_size, output);}
    }
    fclose(input);
    fclose(output);
}
