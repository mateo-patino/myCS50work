#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Find the average of the RGB values in the orginal image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Retrieve RGB channel values
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;

            // Compute average
            float a = ((float)blue + green + red) / 3.0f;
            int average = round(a);

            // Assign average to RGB channel by reference
            BYTE *b = &image[i][j].rgbtBlue;
            BYTE *g = &image[i][j].rgbtGreen;
            BYTE *r = &image[i][j].rgbtRed;
            *b = average;
            *g = average;
            *r = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Swap symmetrically for even widths or asymmetrically for odd
    int center_column = 0;
    for (int i = 0; i < height; i++)
    {
        // Swap symmetrically for even widths
        if ((width % 2) == 0)
        {
            center_column = (width / 2) - 1;
            for (int j = 0; j <= center_column; j++)
            {
                RGBTRIPLE copy = image[i][j];
                image[i][j] = image[i][(width - 1) - j];
                image[i][(width - 1) - j] = copy;
            }
        }
        else
        {
            // Swap asymmetrically for odd widths
            center_column = (width + 1) / 2;
            for (int j = 0; j < (center_column - 1); j++)
            {
                RGBTRIPLE copy = image[i][j];
                image[i][j] = image[i][(width - 1) - j];
                image[i][(width - 1) - j] = copy;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy that is never modified and that you use to find the averages.
    RGBTRIPLE copy[height][width];

    // Need to different loops because one would create uncopied pixels accessable in neighborhood.
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            BYTE *blue = &image[row][column].rgbtBlue;
            BYTE *green = &image[row][column].rgbtGreen;
            BYTE *red = &image[row][column].rgbtRed;
            // Left-side column
            if ((column - 1 < 0))
            {
                // Top-left corner pixel
                if (row - 1 < 0)
                {
                    *blue = round(((float)copy[row][column].rgbtBlue + copy[1][0].rgbtBlue + copy[0][1].rgbtBlue + copy[1][1].rgbtBlue) / 4.0);
                    *green = round(((float)copy[row][column].rgbtGreen + copy[1][0].rgbtGreen + copy[0][1].rgbtGreen + copy[1][1].rgbtGreen) / 4.0);
                    *red = round(((float)copy[row][column].rgbtRed + copy[1][0].rgbtRed + copy[0][1].rgbtRed + copy[1][1].rgbtRed) / 4.0);
                }
                // Bottom-left corner
                else if ((row + 1) > (height - 1))
                {
                    *blue = round(((float)copy[row][column].rgbtBlue + copy[row][1].rgbtBlue + copy[row - 1][0].rgbtBlue + copy[row - 1][1].rgbtBlue) / 4.0);
                    *green = round(((float)copy[row][column].rgbtGreen + copy[row][1].rgbtGreen + copy[row - 1][0].rgbtGreen + copy[row - 1][1].rgbtGreen) / 4.0);
                    *red = round(((float)copy[row][column].rgbtRed + copy[row][1].rgbtRed + copy[row - 1][0].rgbtRed + copy[row - 1][1].rgbtRed) / 4.0);
                }
                // Other pixels on left column
                else
                {
                    // Compute current column first, then column to the right (c + 1). Typecast last operand to ensure it's a float.
                    *blue = round((copy[row][column].rgbtBlue + copy[row - 1][column].rgbtBlue + copy[row + 1][column].rgbtBlue + copy[row - 1][column + 1].rgbtBlue +
                    copy[row][column + 1].rgbtBlue + (float)copy[row + 1][column + 1].rgbtBlue) / 6.0);

                    *green = round((copy[row][column].rgbtGreen + copy[row - 1][column].rgbtGreen + copy[row + 1][column].rgbtGreen + copy[row - 1][column + 1].rgbtGreen +
                    copy[row][column + 1].rgbtGreen + (float)copy[row + 1][column + 1].rgbtGreen) / 6.0);

                    *red = round((copy[row][column].rgbtRed + copy[row - 1][column].rgbtRed + copy[row + 1][column].rgbtRed + copy[row - 1][column + 1].rgbtRed +
                    copy[row][column + 1].rgbtRed + (float)copy[row + 1][column + 1].rgbtRed) / 6.0);
                }
            }
            // Right-side column
            else if ((column + 1) > (width - 1))
            {
                // Top-right corner
                if ((row - 1) < 0)
                {
                    *blue = round((copy[row][column].rgbtBlue + copy[0][width - 2].rgbtBlue + copy[1][width - 1].rgbtBlue + (float)copy[1][width - 2].rgbtBlue / 4.0));
                    *green = round((copy[row][column].rgbtGreen + copy[0][width - 2].rgbtGreen + copy[1][width - 1].rgbtGreen + (float)copy[1][width - 2].rgbtGreen / 4.0));
                    *red = round((copy[row][column].rgbtRed + copy[0][width - 2].rgbtRed + copy[1][width - 1].rgbtRed + (float)copy[1][width - 2].rgbtRed / 4.0));
                }
                // Bottom-right corner
                else if ((row + 1) > (height - 1))
                {
                    *blue = round((copy[row][column].rgbtBlue + copy[height - 1][width - 2].rgbtBlue + copy[height - 2][width - 1].rgbtBlue + (float)copy[height - 2][width - 2].rgbtBlue) / 4.0);
                    *green = round((copy[row][column].rgbtGreen + copy[height - 1][width - 2].rgbtGreen + copy[height - 2][width - 1].rgbtGreen + (float)copy[height - 2][width - 2].rgbtGreen) / 4.0);
                    *red = round((copy[row][column].rgbtRed + copy[height - 1][width - 2].rgbtRed + copy[height - 2][width - 1].rgbtRed + (float)copy[height - 2][width - 2].rgbtRed) / 4.0);
                }
                // Other pixels on right column.
                else
                {
                    // Compute current column first, then column to the left (c - 1).
                    *blue = round((copy[row][column].rgbtBlue + copy[row - 1][column].rgbtBlue + copy[row + 1][column].rgbtBlue + copy[row][column - 1].rgbtBlue +
                    copy[row - 1][column - 1].rgbtBlue + (float)copy[row + 1][column - 1].rgbtBlue) / 6.0);

                    *green = round((copy[row][column].rgbtGreen + copy[row - 1][column].rgbtGreen + copy[row + 1][column].rgbtGreen + copy[row][column - 1].rgbtGreen +
                    copy[row - 1][column - 1].rgbtGreen + (float)copy[row + 1][column - 1].rgbtGreen) / 6.0);

                    *red = round((copy[row][column].rgbtRed + copy[row - 1][column].rgbtRed + copy[row + 1][column].rgbtRed + copy[row][column - 1].rgbtRed +
                    copy[row - 1][column - 1].rgbtRed + (float)copy[row + 1][column - 1].rgbtRed) / 6.0);

                }
            }
            // Top-edge pixels. Corner pixels are excluded thanks to the previous if statements.
            else if ((row - 1) < 0)
            {
                // Compute values of current row first, then go to next row.
                *blue = round((copy[row][column].rgbtBlue + copy[row][column - 1].rgbtBlue + copy[row][column + 1].rgbtBlue + copy[row + 1][column].rgbtBlue +
                copy[row + 1][column - 1].rgbtBlue + (float)copy[row + 1][column + 1].rgbtBlue) / 6.0);

                *green = round((copy[row][column].rgbtGreen + copy[row][column - 1].rgbtGreen + copy[row][column + 1].rgbtGreen + copy[row + 1][column].rgbtGreen +
                copy[row + 1][column - 1].rgbtGreen + (float)copy[row + 1][column + 1].rgbtGreen) / 6.0);

                *red = round((copy[row][column].rgbtRed + copy[row][column - 1].rgbtRed + copy[row][column + 1].rgbtRed + copy[row + 1][column].rgbtRed +
                copy[row + 1][column - 1].rgbtRed + (float)copy[row + 1][column + 1].rgbtRed) / 6.0);
            }
            // Bottom-edge pixels
            else if ((row + 1) > (height - 1))
            {
                // Compute values of current row first, then move on to row - 1 (row above)
                *blue = round((copy[row][column].rgbtBlue + copy[row][column - 1].rgbtBlue + copy[row][column + 1].rgbtBlue + copy[row - 1][column].rgbtBlue +
                copy[row - 1][column - 1].rgbtBlue + (float)copy[row - 1][column + 1].rgbtBlue) / 6.0);

                *green = round((copy[row][column].rgbtGreen + copy[row][column - 1].rgbtGreen + copy[row][column + 1].rgbtGreen + copy[row - 1][column].rgbtGreen +
                copy[row - 1][column - 1].rgbtGreen + (float)copy[row - 1][column + 1].rgbtGreen) / 6.0);

                *red = round((copy[row][column].rgbtRed + copy[row][column - 1].rgbtRed + copy[row][column + 1].rgbtRed + copy[row - 1][column].rgbtRed +
                copy[row - 1][column - 1].rgbtRed + (float)copy[row - 1][column + 1].rgbtRed) / 6.0);
            }
            // If the current pixel didn't satisfy the previous statements, then it can't be located on any of the edge arrays, meaning it must be on an inner layer and
            // have 3x3 pixel neighborhood.
            else
            {
                // Compute current row first, then move to above (r - 1) and below (r + 1) rows
                *blue = round((copy[row][column].rgbtBlue + copy[row][column - 1].rgbtBlue + copy[row][column + 1].rgbtBlue + copy[row - 1][column].rgbtBlue +
                copy[row - 1][column - 1].rgbtBlue + copy[row - 1][column + 1].rgbtBlue + copy[row + 1][column].rgbtBlue + copy[row + 1][column - 1].rgbtBlue +
                (float) copy[row + 1][column + 1].rgbtBlue) / 9.0);

                *green = round((copy[row][column].rgbtGreen + copy[row][column - 1].rgbtGreen + copy[row][column + 1].rgbtGreen + copy[row - 1][column].rgbtGreen +
                copy[row - 1][column - 1].rgbtGreen + copy[row - 1][column + 1].rgbtGreen + copy[row + 1][column].rgbtGreen + copy[row + 1][column - 1].rgbtGreen +
                (float) copy[row + 1][column + 1].rgbtGreen) / 9.0);

                *red = round((copy[row][column].rgbtRed + copy[row][column - 1].rgbtRed + copy[row][column + 1].rgbtRed+ copy[row - 1][column].rgbtRed +
                copy[row - 1][column - 1].rgbtRed + copy[row - 1][column + 1].rgbtRed + copy[row + 1][column].rgbtRed + copy[row + 1][column - 1].rgbtRed +
                (float) copy[row + 1][column + 1].rgbtRed) / 9.0);
            }
        }
    }
}

// NOTE NOTE NOTE: Remember you changed the member names of RGBTRIPLE to red, green, and blue instead of rgbtRed, rgbtGreen, and rgbtBlue.
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    int blueGx = 0, blueGy = 0;
    int greenGx = 0, greenGy = 0;
    int redGx = 0, redGy = 0;

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            BYTE *blue = &image[row][column].rgbtBlue;
            BYTE *green = &image[row][column].rgbtGreen;
            BYTE *red = &image[row][column].rgbtRed;

            if ((column - 1) < 0)
            {
                // Top-left corner
                if ((row - 1) < 0)
                {
                    // This corner only has a non-zero four-pixel neighborhood in the Gx kernel, so we only consider the four lower left values. Of these values only the two
                    // right ones are non-zero, so we only consider those in the computation. The Gy kernel has a similar behavior with same-row pixels.
                    blueGx = (2 * copy[row][column + 1].rgbtBlue) + copy[row + 1][column + 1].rgbtBlue;
                    blueGy = (2 * copy[row + 1][column].rgbtBlue) + copy[row + 1][column + 1].rgbtBlue;

                    greenGx = (2 * copy[row][column + 1].rgbtGreen) + copy[row + 1][column + 1].rgbtGreen;
                    greenGy = (2 * copy[row + 1][column].rgbtGreen) + copy[row + 1][column + 1].rgbtGreen;

                    redGx = (2 * copy[row][column + 1].rgbtRed) + copy[row + 1][column + 1].rgbtRed;
                    redGy = (2 * copy[row + 1][column].rgbtRed) + copy[row + 1][column + 1].rgbtRed;
                }
                // Bottom-leftcorner
                else if ((row + 1) > (height - 1))
                {
                    blueGx = (2 * copy[row][column + 1].rgbtBlue) + copy[row - 1][column + 1].rgbtBlue;
                    blueGy = (-2 * copy[row - 1][column].rgbtBlue) - copy[row - 1][column + 1].rgbtBlue;

                    greenGx = (2 * copy[row][column + 1].rgbtGreen) + copy[row - 1][column + 1].rgbtGreen;
                    greenGy = (-2 * copy[row - 1][column].rgbtGreen) - copy[row - 1][column + 1].rgbtGreen;

                    redGx = (2 * copy[row][column + 1].rgbtRed) + copy[row - 1][column + 1].rgbtRed;
                    redGy = (-2 * copy[row - 1][column].rgbtRed) - copy[row - 1][column + 1].rgbtRed;
                }
                // All the other pixels in the left column.
                else
                {
                    blueGx = (2 * copy[row][column + 1].rgbtBlue) + copy[row - 1][column + 1].rgbtBlue + copy[row + 1][column + 1].rgbtBlue;
                    blueGy = (-2 * copy[row - 1][column].rgbtBlue) + (2 * copy[row + 1][column].rgbtBlue) - copy[row - 1][column + 1].rgbtBlue + copy[row + 1][column + 1].rgbtBlue;

                    greenGx = (2 * copy[row][column + 1].rgbtGreen) + copy[row - 1][column + 1].rgbtGreen + copy[row + 1][column + 1].rgbtGreen;
                    greenGy = (-2 * copy[row - 1][column].rgbtGreen) + (2 * copy[row + 1][column].rgbtGreen) - copy[row - 1][column + 1].rgbtGreen + copy[row + 1][column + 1].rgbtGreen;

                    redGx = (2 * copy[row][column + 1].rgbtRed) + copy[row - 1][column + 1].rgbtRed + copy[row + 1][column + 1].rgbtRed;
                    redGy = (-2 * copy[row - 1][column].rgbtRed) + (2 * copy[row + 1][column].rgbtRed) - copy[row - 1][column + 1].rgbtRed + copy[row + 1][column + 1].rgbtRed;
                }
            }
            else if ((column + 1) > (width - 1))
            {
                // Top-right corner
                if ((row - 1) < 0)
                {
                    blueGx = (-2 * copy[row][column - 1].rgbtBlue) - copy[row + 1][column - 1].rgbtBlue;
                    blueGy = (2 * copy[row + 1][column].rgbtBlue) + copy[row + 1][column - 1].rgbtBlue;

                    greenGx = (-2 * copy[row][column - 1].rgbtGreen) - copy[row + 1][column - 1].rgbtGreen;
                    greenGy = (2 * copy[row + 1][column].rgbtGreen) + copy[row + 1][column - 1].rgbtGreen;

                    redGx = (-2 * copy[row][column - 1].rgbtRed) - copy[row + 1][column - 1].rgbtRed;
                    redGy = (2 * copy[row + 1][column].rgbtRed) + copy[row + 1][column - 1].rgbtRed;
                }
                // Bottom-right corner
                else if ((row + 1) > (height - 1))
                {
                    blueGx = (-2 * copy[row][column - 1].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue;
                    blueGy = (-2 * copy[row - 1][column].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue;

                    greenGx = (-2 * copy[row][column - 1].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen;
                    greenGy = (-2 * copy[row - 1][column].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen;

                    redGx = (-2 * copy[row][column - 1].rgbtRed) - copy[row - 1][column - 1].rgbtRed;
                    redGy = (-2 * copy[row - 1][column].rgbtRed) - copy[row - 1][column - 1].rgbtRed;
                }
                // Rest of the right-column pixels
                else
                {
                    blueGx = (-2 * copy[row][column - 1].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue - copy[row + 1][column - 1].rgbtBlue;
                    blueGy = (-2 * copy[row + 1][column].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue + (2 * copy[row + 1][column].rgbtBlue) + copy[row + 1][column - 1].rgbtBlue;

                    greenGx = (-2 * copy[row][column - 1].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen - copy[row + 1][column - 1].rgbtGreen;
                    greenGy = (-2 * copy[row + 1][column].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen + (2 * copy[row + 1][column].rgbtGreen) + copy[row + 1][column - 1].rgbtGreen;

                    redGx = (-2 * copy[row][column - 1].rgbtRed) - copy[row - 1][column - 1].rgbtRed - copy[row + 1][column - 1].rgbtRed;
                    redGy = (-2 * copy[row + 1][column].rgbtRed) - copy[row - 1][column - 1].rgbtRed + (2 * copy[row + 1][column].rgbtRed) + copy[row + 1][column - 1].rgbtRed;
                }
            }
            // Pixels on the top edge
            else if ((row - 1) < 0)
            {
                // All the corners have already been taken care of in the previous if statements. This takes care of all top-edge pixels excluding the corners.
                blueGx = (-2 * copy[row][column - 1].rgbtBlue) - copy[row + 1][column - 1].rgbtBlue +(2 * copy[row][column + 1].rgbtBlue) + copy[row + 1][column + 1].rgbtBlue;
                blueGy = copy[row + 1][column - 1].rgbtBlue + (2 * copy[row + 1][column].rgbtBlue) + copy[row + 1][column + 1].rgbtBlue;

                greenGx = (-2 * copy[row][column - 1].rgbtGreen) - copy[row + 1][column - 1].rgbtGreen +(2 * copy[row][column + 1].rgbtGreen) + copy[row + 1][column + 1].rgbtGreen;
                greenGy = copy[row + 1][column - 1].rgbtGreen + (2 * copy[row + 1][column].rgbtGreen) + copy[row + 1][column + 1].rgbtGreen;

                redGx = (-2 * copy[row][column - 1].rgbtRed) - copy[row + 1][column - 1].rgbtRed +(2 * copy[row][column + 1].rgbtRed) + copy[row + 1][column + 1].rgbtRed;
                redGy = copy[row + 1][column - 1].rgbtRed + (2 * copy[row + 1][column].rgbtRed) + copy[row + 1][column + 1].rgbtRed;
            }
            // Pixels on the bottom edge
            else if ((row + 1) > (height - 1))
            {
                blueGx = (-2 * copy[row][column - 1].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue + (2 * copy[row][column + 1].rgbtBlue) + copy[row - 1][column + 1].rgbtBlue;
                blueGy = (- 2 * copy[row - 1][column].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue - copy[row - 1][column + 1].rgbtBlue;

                greenGx = (-2 * copy[row][column - 1].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen + (2 * copy[row][column + 1].rgbtGreen) + copy[row - 1][column + 1].rgbtGreen;
                greenGy = (- 2 * copy[row - 1][column].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen - copy[row - 1][column + 1].rgbtGreen;

                redGx = (-2 * copy[row][column - 1].rgbtRed) - copy[row - 1][column - 1].rgbtRed + (2 * copy[row][column + 1].rgbtRed) + copy[row - 1][column + 1].rgbtRed;
                redGy = (- 2 * copy[row - 1][column].rgbtRed) - copy[row - 1][column - 1].rgbtRed - copy[row - 1][column + 1].rgbtRed;
            }
            // Any pixel that arrives to this statement must be a non-edge pixel, meaning the full kernel must be considered.
            else
            {
                blueGx = (-2 * copy[row][column - 1].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue - copy[row + 1][column - 1].rgbtBlue + (2 * copy[row][column + 1].rgbtBlue) +
                copy[row - 1][column + 1].rgbtBlue + copy[row + 1][column + 1].rgbtBlue;
                blueGy = (-2 * copy[row - 1][column].rgbtBlue) - copy[row - 1][column - 1].rgbtBlue - copy[row - 1][column + 1].rgbtBlue + (2 * copy[row + 1][column].rgbtBlue) +
                copy[row + 1][column - 1].rgbtBlue + copy[row + 1][column + 1].rgbtBlue;

                greenGx = (-2 * copy[row][column - 1].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen - copy[row + 1][column - 1].rgbtGreen + (2 * copy[row][column + 1].rgbtGreen) +
                copy[row - 1][column + 1].rgbtGreen + copy[row + 1][column + 1].rgbtGreen;
                greenGy = (-2 * copy[row - 1][column].rgbtGreen) - copy[row - 1][column - 1].rgbtGreen - copy[row - 1][column + 1].rgbtGreen + (2 * copy[row + 1][column].rgbtGreen) +
                copy[row + 1][column - 1].rgbtGreen + copy[row + 1][column + 1].rgbtGreen;

                redGx = (-2 * copy[row][column - 1].rgbtRed) - copy[row - 1][column - 1].rgbtRed - copy[row + 1][column - 1].rgbtRed + (2 * copy[row][column + 1].rgbtRed) +
                copy[row - 1][column + 1].rgbtRed + copy[row + 1][column + 1].rgbtRed;
                redGy = (-2 * copy[row - 1][column].rgbtRed) - copy[row - 1][column - 1].rgbtRed - copy[row - 1][column + 1].rgbtRed + (2 * copy[row + 1][column].rgbtRed) +
                copy[row + 1][column - 1].rgbtRed + copy[row + 1][column + 1].rgbtRed;
            }
            // Once outside the if statements, we assign the corresponding values to *blue, *green, and *red capped at 255 (0xff).
            if (sqrt(pow(blueGx, 2) + pow(blueGy, 2)) <= 255)
            {
                *blue = round(sqrt(pow(blueGx, 2) + pow(blueGy, 2)));
            }
            else
            {
                *blue = 255;
            }
            // Cap green channel to 255
            if (sqrt(pow(greenGx, 2) + pow(greenGy, 2)) <= 255)
            {
                *green = round(sqrt(pow(greenGx, 2) + pow(greenGy, 2)));
            }
            else
            {
                *green = 255;
            }
            // Cap red channel to 255
            if (sqrt(pow(redGx, 2) + pow(redGy, 2)) <= 255)
            {
                *red = round(sqrt(pow(redGx, 2) + pow(redGy, 2)));
            }
            else
            {
                *red = 255;
            }
        }
    }
    return;
}