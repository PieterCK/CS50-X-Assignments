#include "helpers.h"
#include "math.h"
#include <stdlib.h>
#include <cs50.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int h = 0;
    int w;
    int buffer;
    // printf("1st red BYTE after -g : %u\n Last red BYTE after -g : %u\n", image[0][0].rgbtRed, image[0][width-1].rgbtRed);
    while (h < height)
    {
        w = 0;
        while (w < width)
        {
            buffer = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = buffer;
            image[h][w].rgbtGreen = buffer;
            image[h][w].rgbtBlue = buffer;
            w++;
        }
        h++;
    }
    // printf("1st red BYTE after -g : %u\n Last red BYTE after -g : %u\n", image[0][0].rgbtRed, image[0][width-1].rgbtRed);
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int h = 0;
    int w;
    int buffer[3];
    // printf("1st red BYTE before -r : %u\n Last red BYTE before -r : %u\n", image[0][0].rgbtRed, image[0][width-1].rgbtRed);
    while (h < height)
    {
        w = 0;
        int inverse_w = (width - 1) - w;
        while (inverse_w > w)
        {
            buffer[0] = image[h][w].rgbtRed;
            buffer[1] = image[h][w].rgbtGreen;
            buffer[2] = image[h][w].rgbtBlue;
            image[h][w].rgbtRed = image[h][inverse_w].rgbtRed;
            image[h][w].rgbtGreen = image[h][inverse_w].rgbtGreen;
            image[h][w].rgbtBlue = image[h][inverse_w].rgbtBlue;
            image[h][inverse_w].rgbtRed = buffer[0];
            image[h][inverse_w].rgbtGreen = buffer[1];
            image[h][inverse_w].rgbtBlue = buffer[2];
            w++;
            inverse_w = (width - 1) - w;
        }
        h++;
    }
    // printf("1st red BYTE after -r : %u\n Last red BYTE after -r : %u\n", image[0][0].rgbtRed, image[0][width-1].rgbtRed);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    typedef struct
    {
        BYTE F_rgbtBlue;
        BYTE F_rgbtGreen;
        BYTE F_rgbtRed;
    } RGBTRIPLE2;

    RGBTRIPLE2 F_image[height][width];
    long h;
    long w;
    float sum = 0;
    long bffr_R = 0;
    long bffr_G = 0;
    long bffr_B = 0;
    for (h = 0; h < height; h++)
    {
        for (w = 0; w < width; w++)
        {
            for (long trk_h = 0; trk_h < 3; trk_h++)
            {
                for (long trk_w = 0; trk_w < 3; trk_w++)
                {
                    int box_h = h - 1 + trk_h;
                    int box_w = w - 1 + trk_w;
                    if (box_h > -1 && box_h < height && box_w > -1 && box_w < width)
                    {
                        bffr_R = bffr_R + image[box_h][box_w].rgbtRed;
                        bffr_G = bffr_G + image[box_h][box_w].rgbtGreen;
                        bffr_B = bffr_B + image[box_h][box_w].rgbtBlue;
                        sum++;
                    }
                }
            }
            F_image[h][w].F_rgbtRed = round(bffr_R / sum);
            F_image[h][w].F_rgbtGreen = round(bffr_G / sum);
            F_image[h][w].F_rgbtBlue = round(bffr_B / sum);
            sum = 0;
            bffr_R = 0;
            bffr_G = 0;
            bffr_B = 0;
        }
    }
    h = 0;
    while (h < height)
    {
        w = 0;
        while (w < width)
        {
            image[h][w].rgbtRed = F_image[h][w].F_rgbtRed;
            image[h][w].rgbtGreen = F_image[h][w].F_rgbtGreen;
            image[h][w].rgbtBlue = F_image[h][w].F_rgbtBlue;
            w++;
        }
        h++;
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    typedef struct
    {
        BYTE F_rgbtBlue;
        BYTE F_rgbtGreen;
        BYTE F_rgbtRed;
    } RGBTRIPLE2;
    RGBTRIPLE2 F_image[height][width];

    // Register Gx & Gy value into an array named Gx_Sc & Gy_Sc for later use.
    int Gx_val[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy_val[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int Gx_Sc[3][3];
    int Gy_Sc[3][3];
    int m = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int n = 0; n < 3; n++)
        {
            Gx_Sc[i][n] = Gx_val[m];
            Gy_Sc[i][n] = Gy_val[m];
            // printf("%i", Gx_Sc[i][n]);
            // printf("%i", Gy_Sc[i][n]);
            m++;
        }
        // printf("\n");
    }
    // h tracks height, w tracks width
    int h = 0;
    long w;

    // Gx_(R,G,B) & Gy_(R,G,B) is used as a buffer to store calculated value.
    long Gx_R;
    long Gx_G;
    long Gx_B;
    long Gy_R;
    long Gy_G;
    long Gy_B;

    // no reason why im using while-loop for h and then switch to for-loop the others
    while (h < height)
    {
        for (w = 0; w < width; w++)
        {
            Gx_R = 0;
            Gx_G = 0;
            Gx_B = 0;
            Gy_R = 0;
            Gy_G = 0;
            Gy_B = 0;

            // trk_h & trk_w is used to point to the 3x3 grind with the current pixle in the middle.
            //  their values can range 0 - 3
            for (long trk_h = 0; trk_h < 3; trk_h++)
            {
                for (long trk_w = 0; trk_w < 3; trk_w++)
                {
                    // box_h & box_w is used to track the actual location of the pixle inside the image where trk is currently pointing
                    //  their values can range from -1 to +1 height / width.
                    int box_h = h - 1 + trk_h;
                    int box_w = w - 1 + trk_w;

                    // if box_h & box_w is within height & width, proceed to calculate.
                    if (box_h > -1 && box_h < height && box_w > -1 && box_w < width)
                    {
                        // Gx_(R,G,B) is the sum rgbtvalue * Gx values in 3x3 grid.(same ofr Gx_(R,G,B))
                        Gx_R = Gx_R + (image[box_h][box_w].rgbtRed * Gx_Sc[trk_h][trk_w]);
                        Gx_G = Gx_G + (image[box_h][box_w].rgbtGreen * Gx_Sc[trk_h][trk_w]);
                        Gx_B = Gx_B + (image[box_h][box_w].rgbtBlue * Gx_Sc[trk_h][trk_w]);
                        Gy_R = Gy_R + (image[box_h][box_w].rgbtRed * Gy_Sc[trk_h][trk_w]);
                        Gy_G = Gy_G + (image[box_h][box_w].rgbtGreen * Gy_Sc[trk_h][trk_w]);
                        Gy_B = Gy_B + (image[box_h][box_w].rgbtBlue * Gy_Sc[trk_h][trk_w]);
                    }
                }
            }
            if (sqrt(pow(Gx_R, 2) + pow(Gy_R, 2)) < 255)
            {
                F_image[h][w].F_rgbtRed = round(sqrt(pow(Gx_R, 2) + pow(Gy_R, 2)));
            }
            else
            {
                F_image[h][w].F_rgbtRed = 255;
            }
            if (sqrt(pow(Gx_G, 2) + pow(Gy_G, 2)) < 255)
            {
                F_image[h][w].F_rgbtGreen = round(sqrt(pow(Gx_G, 2) + pow(Gy_G, 2)));
            }
            else
            {
                F_image[h][w].F_rgbtGreen = 255;
            }
            if (sqrt(pow(Gx_B, 2) + pow(Gy_B, 2)) < 255)
            {
                F_image[h][w].F_rgbtBlue = round(sqrt(pow(Gx_B, 2) + pow(Gy_B, 2)));
            }
            else
            {
                F_image[h][w].F_rgbtBlue = 255;
            }
        }
        h++;
    }

    // register image with F_image(filtered with edge pixels)
    h = 0;
    while (h < height)
    {
        w = 0;
        while (w < width)
        {
            image[h][w].rgbtRed = F_image[h][w].F_rgbtRed;
            image[h][w].rgbtGreen = F_image[h][w].F_rgbtGreen;
            image[h][w].rgbtBlue = F_image[h][w].F_rgbtBlue;
            w++;
        }
        h++;
    }
    return;
}
