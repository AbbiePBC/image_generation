/ RGBTRIPLE copy[height][width];
    int var_red_x, var_green_x, var_blue_x; //defining variables to store modified x values
    int var_red_y, var_green_y, var_blue_y;
    
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
            var_red_x = 0, var_green_x = 0, var_blue_x = 0;
            var_red_y = 0, var_green_y = 0, var_blue_y = 0;
            
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
                        var_red_x =+ image[i + a][j + b].rgbtRed * array_Gx[a + 1][b + 1];
                        var_green_x =+ image[i + a][j + b].rgbtGreen * array_Gx[a + 1][b + 1];
                        var_blue_x =+ image[i + a][j + b].rgbtBlue * array_Gx[a + 1][b + 1];
                        var_red_y =+ image[i + a][j + b].rgbtRed * array_Gy[a + 1][b + 1];
                        var_green_x =+ image[i + a][j + b].rgbtGreen * array_Gy[a + 1][b + 1];
                        var_blue_y =+ image[i + a][j + b].rgbtBlue * array_Gy[a + 1][b + 1];
  
                    }
                }
            }
        
            copy[i][j].rgbtRed = round(sqrt(var_red_x * var_red_x ) + (var_red_y * var_red_y));
            copy[i][j].rgbtGreen = round(sqrt(var_green_x * var_green_x)  + (var_green_y * var_green_y)); 
            copy[i][j].rgbtBlue = round(sqrt(var_blue_x * var_blue_x) + (var_blue_y * var_blue_y));
            
            if (copy[i][j].rgbtRed > 255)
            {
                copy[i][j].rgbtRed  = 255;
            }
            if (copy[i][j].rgbtGreen > 255)
            {
                copy[i][j].rgbtGreen  = 255;
            }
            if (copy[i][j].rgbtBlue > 255)
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