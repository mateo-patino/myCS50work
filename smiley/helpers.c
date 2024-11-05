#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
  for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Don't define a dummy variable for image[i][j] as this will create a copy at a different address than the
            // original image and the RGB values will be stored somewhere far away.
            if (image[i][j].rgbtBlue == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtRed == 0x00)
            {
                BYTE *r = &image[i][j].rgbtRed;
                BYTE *g = &image[i][j].rgbtGreen;
                BYTE *b = &image[i][j].rgbtBlue;

                *r = 0xFF;
                *g = 0xC8;
                *b = 0x78;
            }
        }
    }
    return 0;
}