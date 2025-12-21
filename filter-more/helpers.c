#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE c = image[i][j];
            float grey = (c.rgbtBlue + c.rgbtGreen + c.rgbtRed) / 3.0;
            image[i][j].rgbtBlue = round(grey);
            image[i][j].rgbtGreen = round(grey);
            image[i][j].rgbtRed = round(grey);
            // printf("%i %i %i\n", image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed
            // = grey);
        }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflect[height][width];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            reflect[i][width - j - 1] = image[i][j];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            image[i][j] = reflect[i][j];
    return;
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++)
        {
            double rgb[3] = {0};
            float counter = 0;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    int pixel_x = r + i - 1;
                    int pixel_y = c + j - 1;
                    if (height > pixel_x && width > pixel_y && 0 <= pixel_x && 0 <= pixel_y)
                    {
                        rgb[0] += image[pixel_x][pixel_y].rgbtRed;
                        rgb[1] += image[pixel_x][pixel_y].rgbtGreen;
                        rgb[2] += image[pixel_x][pixel_y].rgbtBlue;
                        counter++;
                    }
                }
            temp[r][c].rgbtRed = round(rgb[0] / counter);
            temp[r][c].rgbtGreen = round(rgb[1] / counter);
            temp[r][c].rgbtBlue = round(rgb[2] / counter);
        }
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++)
            image[r][c] = temp[r][c];
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++)
            temp[r][c] = image[r][c];

    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++)
        {
            int matrixgx[3][3] =
            {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
            };
            int matrixgy[3][3] =
            {
                {-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}
            };
            int rgb_gx[3] = {0};
            int rgb_gy[3] = {0};
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    int pixel_x = r + i - 1;
                    int pixel_y = c + j - 1;
                    if (height > pixel_x && width > pixel_y && 0 <= pixel_x && 0 <= pixel_y)
                    {
                        rgb_gx[0] += (temp[pixel_x][pixel_y].rgbtRed * matrixgx[i][j]);
                        rgb_gx[1] += (temp[pixel_x][pixel_y].rgbtGreen * matrixgx[i][j]);
                        rgb_gx[2] += (temp[pixel_x][pixel_y].rgbtBlue * matrixgx[i][j]);
                        rgb_gy[0] += (temp[pixel_x][pixel_y].rgbtRed * matrixgy[i][j]);
                        rgb_gy[1] += (temp[pixel_x][pixel_y].rgbtGreen * matrixgy[i][j]);
                        rgb_gy[2] += (temp[pixel_x][pixel_y].rgbtBlue * matrixgy[i][j]);
                    }
                }
            int rgb[3];
            for (int i = 0; i < 3; i++)
            {
                rgb[i] = round(sqrt(pow(rgb_gx[i],2)+pow(rgb_gy[i],2)));
                if(rgb[i]>255)
                    rgb[i] = 255;
                else if(rgb[i]<0)
                    rgb[i] = 0;
            }
            image[r][c].rgbtRed = rgb[0];
            image[r][c].rgbtGreen = rgb[1];
            image[r][c].rgbtBlue = rgb[2];
        }
    return;
}
