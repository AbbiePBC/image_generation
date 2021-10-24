#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float averagef = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.00;
            int average = round(averagef);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //first we store the colours in each place
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            //now we reallocate the pixels
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            //now we set the result to the colours we found
            image[i][width - 1 - j].rgbtRed = red;
            image[i][width - 1 - j].rgbtGreen = green;
            image[i][width - 1 - j].rgbtBlue = blue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //at the edges, we need to average over 6 pixels, and at edges,only over 4. so when i-1 = -1 and i+1>height, this is not allowed
            //we need to find the values of j-1, j, and j+1 for each pixel and for each of i, i+1, i-1
            //we can do this by creating a mini loop
            //we cannot change the values themselves however, as this will affect future calculations
            float sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;
            //this resets the value of summation every time we do the calculation for a new pixel
            for (int a = -1; a < 2; a ++)
            {
                for (int b = -1; b < 2; b ++)
                {
                    if (i + a < 0 || i + a > height - 1 || j + b < 0 || j + b > width - 1)
                        //a and b run through -1, 0, 1
                        //can't put that equal to height so have to say more than height - 1
                        //if i is zero, and a is -1, total is less than zero and we don't want to include -1 position so this can be skipped.
                        //if i is height -1 and a is 1, total is more than height, and we don't include this
                        //same for width
                    {
                        continue;
                    }
                    else
                    {
                        sumRed += image[i + a][j + b].rgbtRed;
                        sumGreen += image[i + a][j + b].rgbtGreen;
                        sumBlue += image[i + a][j + b].rgbtBlue;
                        count ++;
                    }
                }
            }
            copy[i][j].rgbtRed = round(sumRed / count);
            copy[i][j].rgbtBlue = round(sumBlue / count);
            copy[i][j].rgbtGreen = round(sumGreen / count);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
int min(int num1, int num2); //min function for dealing with data types

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int var_red_x, var_green_x, var_blue_x; //defining variables to store modified x values
    int var_red_y, var_green_y, var_blue_y;
    int rounded_red, rounded_green, rounded_blue;

    int array_Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int array_Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int i = 0; i < height; i++) //for every pixel in height
    {
        for (int j = 0; j < width; j++) //for every pixel in width
        {
            var_red_x = var_green_x = var_blue_x = var_red_y = var_green_y = var_blue_y = 0;
            rounded_red = rounded_green = rounded_blue = 0;

            for (int a = -1; a < 2; a ++) //pixel
            {
                for (int b = -1; b < 2; b ++) //pixel
                {
                    if (i + a < 0 || i + a > height - 1 || j + b < 0
                        || j + b > width - 1) //ignore all values if outside range (i.e. same as setting = 0)
                    {
                        continue;
                    }
                    else
                    {
                        //due to my definition of a, this can be -ve, so add 1 for index[0]
                        const RGBTRIPLE pixel = image[i + a][j + b];
                        var_red_x += ((int)pixel.rgbtRed * array_Gx[a + 1][b + 1]);
                        var_green_x += ((int)pixel.rgbtGreen * array_Gx[a + 1][b + 1]);
                        var_blue_x += ((int)pixel.rgbtBlue * array_Gx[a + 1][b + 1]);
                        var_red_y += ((int)pixel.rgbtRed * array_Gy[a + 1][b + 1]);
                        var_green_y += ((int)pixel.rgbtGreen * array_Gy[a + 1][b + 1]);
                        var_blue_y += ((int)pixel.rgbtBlue * array_Gy[a + 1][b + 1]);
                        
                    }
                }
            }

            rounded_red = (int)round(sqrt((var_red_x * var_red_x) + (var_red_y * var_red_y)));
            rounded_green = (int)round(sqrt((var_green_x * var_green_x)  + (var_green_y * var_green_y)));
            rounded_blue = (int)round(sqrt((var_blue_x * var_blue_x) + (var_blue_y * var_blue_y)));
            //rgbt stored as 1 byte, int as 4 bytes => to avoid overflow of rgbt, if value exceeds 255, set this as the maximum
            copy[i][j].rgbtRed = min(255, rounded_red); 
            copy[i][j].rgbtGreen = min(255, rounded_green);
            copy[i][j].rgbtBlue = min(255, rounded_blue);
            
            if (copy[i][j].rgbtRed > 254)
            {
                copy[i][j].rgbtRed  = 255;
            }
            if (copy[i][j].rgbtGreen > 254)
            {
                copy[i][j].rgbtGreen  = 255;
            }
            if (copy[i][j].rgbtBlue > 254)
            {
                copy[i][j].rgbtBlue  = 255;
            }
            
            
            
        }
    }

    
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }
    return;
    
}

int min(int num1, int num2) 
{
    return (num1 > num2) ? num2 : num1;
}