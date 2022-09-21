#include <stdio.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale by averaging bgr values
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float b = image[i][j].rgbtBlue;
            float g = image[i][j].rgbtGreen;
            float r = image[i][j].rgbtRed;
            int f = round((r + g + b) / 3);
            image[i][j].rgbtBlue = f;
            image[i][j].rgbtGreen = f;
            image[i][j].rgbtRed = f;
        }
    }
    return;
}

// Reflect image horizontally by copying left-most pixel into a temp var, moving right over, then copying temp to right
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtBlue = b;
            image[i][width - j - 1].rgbtGreen = g;
            image[i][width - j - 1].rgbtRed = r;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a value queue to hold blur values temporarily until done checking the original pixels.
    int bqueue[height][width];
    int gqueue[height][width];
    int rqueue[height][width];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Confirm we are still in the matrix
            if (i < height)
            {
                // Initialize counter d, and summations b, g and r. Used below to add everything and divide.
                float d = 1;
                int b = image[i][j].rgbtBlue;
                int g = image[i][j].rgbtGreen;
                int r = image[i][j].rgbtRed;

                // Top left (pixel of a 3 x 3 grid centered on the pixel)
                if (i && j)
                {
                    b += image[i - 1][j - 1].rgbtBlue;
                    g += image[i - 1][j - 1].rgbtGreen;
                    r += image[i - 1][j - 1].rgbtRed;
                    d++;
                }
                // Top...
                if (i)
                {
                    b += image[i - 1][j].rgbtBlue;
                    g += image[i - 1][j].rgbtGreen;
                    r += image[i - 1][j].rgbtRed;
                    d++;
                }
                // Top right...
                if (i && j < width - 1)
                {
                    b += image[i - 1][j + 1].rgbtBlue;
                    g += image[i - 1][j + 1].rgbtGreen;
                    r += image[i - 1][j + 1].rgbtRed;
                    d++;
                }
                // Left...
                if (j)
                {
                    b += image[i][j - 1].rgbtBlue;
                    g += image[i][j - 1].rgbtGreen;
                    r += image[i][j - 1].rgbtRed;
                    d++;
                }
                // Right... Middle values were used to initialize b, g, and r...
                if (j < width - 1)
                {
                    b += image[i][j + 1].rgbtBlue;
                    g += image[i][j + 1].rgbtGreen;
                    r += image[i][j + 1].rgbtRed;
                    d++;
                }
                // Bottom left...
                if (i < height - 1 && j)
                {
                    b += image[i + 1][j - 1].rgbtBlue;
                    g += image[i + 1][j - 1].rgbtGreen;
                    r += image[i + 1][j - 1].rgbtRed;
                    d++;
                }
                // Bottom...
                if (i < height - 1)
                {
                    b += image[i + 1][j].rgbtBlue;
                    g += image[i + 1][j].rgbtGreen;
                    r += image[i + 1][j].rgbtRed;
                    d++;
                }
                // Bottom Right...
                if (i < height - 1 && j < width - 1)
                {
                    b += image[i + 1][j + 1].rgbtBlue;
                    g += image[i + 1][j + 1].rgbtGreen;
                    r += image[i + 1][j + 1].rgbtRed;
                    d++;
                }
                // Load values into the value queues
                bqueue[i][j] = round(b / d);
                gqueue[i][j] = round(g / d);
                rqueue[i][j] = round(r / d);
                // Load values from queue to image two rows up from blur calculations
                if (i > 1)
                {
                    image[i - 2][j].rgbtBlue = bqueue[i - 2][j];
                    image[i - 2][j].rgbtGreen = gqueue[i - 2][j];
                    image[i - 2][j].rgbtRed = rqueue[i - 2][j];
                }
            }
        }
    }
    // Load remaining value queue values to image
    for (int i = height - 2; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = bqueue[i][j];
            image[i][j].rgbtGreen = gqueue[i][j];
            image[i][j].rgbtRed = rqueue[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    int b = 0;
    int g = 0;
    int r = 0;
    int bqueue[height][width];
    int gqueue[height][width];
    int rqueue[height][width];

    // Find edges GxGy kernel

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            double bx = 0;
            double gx = 0;
            double rx = 0;
            double by = 0;
            double gy = 0;
            double ry = 0;
            // Top left pixel
            if (i && j)
            {
                // Gx
                bx += (image[i - 1][j - 1].rgbtBlue * -1);
                gx += (image[i - 1][j - 1].rgbtGreen * -1);
                rx += (image[i - 1][j - 1].rgbtRed * -1);
                // Gy
                by += (image[i - 1][j - 1].rgbtBlue * -1);
                gy += (image[i - 1][j - 1].rgbtGreen * -1);
                ry += (image[i - 1][j - 1].rgbtRed * -1);
            }
            // Top pixel
            if (i)
            {
                // Gy
                by += (image[i - 1][j].rgbtBlue * -2);
                gy += (image[i - 1][j].rgbtGreen * -2);
                ry += (image[i - 1][j].rgbtRed * -2);
            }
            // Top right pixel
            if (i && j < width - 1)
            {
                // Gx
                bx += image[i - 1][j + 1].rgbtBlue;
                gx += image[i - 1][j + 1].rgbtGreen;
                rx += image[i - 1][j + 1].rgbtRed;
                // Gy
                by += (image[i - 1][j + 1].rgbtBlue * -1);
                gy += (image[i - 1][j + 1].rgbtGreen * -1);
                ry += (image[i - 1][j + 1].rgbtRed * -1);
            }
            // Left pixel
            if (j)
            {
                // Gx
                bx += (image[i][j - 1].rgbtBlue * -2);
                gx += (image[i][j - 1].rgbtGreen * -2);
                rx += (image[i][j - 1].rgbtRed * -2);
            }
            // Right pixel
            if (j < width - 1)
            {
                // Gx
                bx += (image[i][j + 1].rgbtBlue * 2);
                gx += (image[i][j + 1].rgbtGreen * 2);
                rx += (image[i][j + 1].rgbtRed * 2);
            }
            // Bottom left pixel
            if (i < height - 1 && j)
            {
                // Gx
                bx += (image[i + 1][j - 1].rgbtBlue * -1);
                gx += (image[i + 1][j - 1].rgbtGreen * -1);
                rx += (image[i + 1][j - 1].rgbtRed * -1);
                // Gy
                by += image[i + 1][j - 1].rgbtBlue;
                gy += image[i + 1][j - 1].rgbtGreen;
                ry += image[i + 1][j - 1].rgbtRed;
            }
            // Bottom pixel
            if (i < height - 1)
            {
                // Gy
                by += (image[i + 1][j].rgbtBlue * 2);
                gy += (image[i + 1][j].rgbtGreen * 2);
                ry += (image[i + 1][j].rgbtRed * 2);
            }
            // Bottom right pixel
            if (i < height - 1 && j < width - 1)
            {
                // Gx
                bx += image[i + 1][j + 1].rgbtBlue;
                gx += image[i + 1][j + 1].rgbtGreen;
                rx += image[i + 1][j + 1].rgbtRed;
                // Gy
                by += image[i + 1][j + 1].rgbtBlue;
                gy += image[i + 1][j + 1].rgbtGreen;
                ry += image[i + 1][j + 1].rgbtRed;
            }

            bqueue[i][j] = round(sqrt((bx * bx) + (by * by)));
            gqueue[i][j] = round(sqrt((gx * gx) + (gy * gy)));
            rqueue[i][j] = round(sqrt((rx * rx) + (ry * ry)));
            if (bqueue[i][j] > 255)
            {
                bqueue[i][j] = 255;
            }
            if (gqueue[i][j] > 255)
            {
                gqueue[i][j] = 255;
            }
            if (rqueue[i][j] > 255)
            {
                rqueue[i][j] = 255;
            }
            if (i > 1)
            {
                image[i - 2][j].rgbtBlue = bqueue[i - 2][j];
                image[i - 2][j].rgbtGreen = gqueue[i - 2][j];
                image[i - 2][j].rgbtRed = rqueue[i - 2][j];
            }
        }
    }
    for (i = height - 2; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = bqueue[i][j];
            image[i][j].rgbtGreen = gqueue[i][j];
            image[i][j].rgbtRed = rqueue[i][j];
        }
    }
    return;
}
