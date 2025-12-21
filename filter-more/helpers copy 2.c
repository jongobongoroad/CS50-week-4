#include "helpers.h"
#include "stdio.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            RGBTRIPLE c = image[i][j];
            float grey = (c.rgbtBlue + c.rgbtGreen + c.rgbtRed) / 3.0;
            image[i][j].rgbtBlue = round(grey);
            image[i][j].rgbtGreen = round(grey);
            image[i][j].rgbtRed = round(grey);
            // printf("%i %i %i\n", image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed = grey);
        }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflect[height][width];
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            reflect[i][width-j-1] = image[i][j];
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        image[i][j] = reflect[i][j];
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE neg_one = {-1,-1,-1};
    RGBTRIPLE temp[height][width];
    double avgRed, avgGreen, avgBlue;
    float counter;

    for(int r=0;r<height;r++)
        for(int c=0;c<width;c++)
            temp[r][c] = image[r][c];

    for(int r=0;r<height;r++)
        for(int c=0;c<width;c++)
        {
            RGBTRIPLE matrix[3][3] = {
                {temp[r-1][c-1], temp[r-1][c], temp[r-1][c+1]},
                {temp[r][c-1],   temp[r][c],   temp[r][c+1]},
                {temp[r+1][c-1], temp[r+1][c], temp[r+1][c+1]}
            };
            counter = 0;
            avgRed = 0;
            avgGreen = 0;
            avgBlue = 0;

            if(r==0)
            {
                matrix[0][0] = neg_one;
                matrix[0][1] = neg_one;
                matrix[0][2] = neg_one;
            } else if (r==height-1)
            {
                matrix[2][0] = neg_one;
                matrix[2][1] = neg_one;
                matrix[2][2] = neg_one;
            };
            if(c==0)
            {
                matrix[0][0] = neg_one;
                matrix[1][0] = neg_one;
                matrix[2][0] = neg_one;
            } else if (c==width-1)
            {
                matrix[0][2] = neg_one;
                matrix[1][2] = neg_one;
                matrix[2][2] = neg_one;
            };

            for (int i=0;i<3;i++)
                for (int j=0;j<3;j++)
                {
                    if (matrix[i][j].rgbtRed != neg_one.rgbtRed && matrix[i][j].rgbtGreen != neg_one.rgbtGreen && matrix[i][j].rgbtBlue != neg_one.rgbtBlue)
                    {
                        avgRed = avgRed + matrix[i][j].rgbtRed;
                        avgGreen = avgGreen + matrix[i][j].rgbtGreen;
                        avgBlue = avgBlue + matrix[i][j].rgbtBlue;
                        counter++;
                    }
                }

            avgRed = round(avgRed / counter);
            avgGreen = round(avgGreen / counter);
            avgBlue = round(avgBlue / counter);

            image[r][c].rgbtRed = avgRed;
            image[r][c].rgbtGreen = avgGreen;
            image[r][c].rgbtBlue = avgBlue;
        }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE zero = {0};
    RGBTRIPLE temp[height][width];

    for(int r=0;r<height;r++)
        for(int c=0;c<width;c++)
            temp[r][c] = image[r][c];

    for(int r=0;r<height;r++)
        for(int c=0;c<width;c++)
        {
            RGBTRIPLE matrix[3][3] = {
                {temp[r-1][c-1], temp[r-1][c], temp[r-1][c+1]},
                {temp[r][c-1],   temp[r][c],   temp[r][c+1]},
                {temp[r+1][c-1], temp[r+1][c], temp[r+1][c+1]}
            };
            int matrixgx[3][3] = {
                {-1,0,1},
                {-2,0,2},
                {-1,0,1}
            };
            int matrixgy[3][3] = {
                {-1,-2,-1},
                {0,0,0},
                {1,2,1}
            };

            if(r==0)
            {
                matrix[0][0] = zero;
                matrix[0][1] = zero;
                matrix[0][2] = zero;
            } else if (r==height-1)
            {
                matrix[2][0] = zero;
                matrix[2][1] = zero;
                matrix[2][2] = zero;
            };
            if(c==0)
            {
                matrix[0][0] = zero;
                matrix[1][0] = zero;
                matrix[2][0] = zero;
            } else if (c==width-1)
            {
                matrix[0][2] = zero;
                matrix[1][2] = zero;
                matrix[2][2] = zero;
            };

            long tempgx[3] = {0};
            long tempgy[3] = {0};
            for (int i=0;i<3;i++)
                for (int j=0;j<3;j++)
                {
                    tempgx[2] += (matrixgx[i][j] * matrix[i][j].rgbtBlue);
                    tempgx[1] += (matrixgx[i][j] * matrix[i][j].rgbtGreen);
                    tempgx[0] += (matrixgx[i][j] * matrix[i][j].rgbtRed);
                    tempgy[2] += (matrixgy[i][j] * matrix[i][j].rgbtBlue);
                    tempgy[1] += (matrixgy[i][j] * matrix[i][j].rgbtGreen);
                    tempgy[0] += (matrixgy[i][j] * matrix[i][j].rgbtRed);
                }

            long final_pixel_color[3];
            final_pixel_color[0] = sqrt(pow(tempgx[0],2)+pow(tempgy[0],2));
            final_pixel_color[1] = sqrt(pow(tempgx[1],2)+pow(tempgy[1],2));
            final_pixel_color[2] = sqrt(pow(tempgx[2],2)+pow(tempgy[2],2));

            if (final_pixel_color[0]>255)
                final_pixel_color[0] = 255;
            else if (final_pixel_color[0]<0)
                final_pixel_color[0] = 0;
            if (final_pixel_color[1]>255)
                final_pixel_color[1] = 255;
            else if (final_pixel_color[1]<0)
                final_pixel_color[1] = 0;
            if (final_pixel_color[2]>255)
                final_pixel_color[2] = 255;
            else if (final_pixel_color[2]<0)
                final_pixel_color[2] = 0;

            RGBTRIPLE pixel = zero;
            pixel.rgbtRed = final_pixel_color[0];
            pixel.rgbtGreen = final_pixel_color[1];
            pixel.rgbtBlue = final_pixel_color[2];
            image[r][c] = pixel;
        }
    return;
}
