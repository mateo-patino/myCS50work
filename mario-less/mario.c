#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask for size
    int size;
    do
    {
        size = get_int("Size: ");
    }
    while (size < 1 || size > 8);

    // Outer loop
    for (int i = 0; i < size; i++)
    {
        // Num of spaces such that spaces and hashes equal size
        for (int j = 0; j < size - i - 1; j++)
        {
            printf(" ");
        }
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}