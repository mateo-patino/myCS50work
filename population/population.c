#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start and end size
    int size;
    do
    {
        size = get_int("Start size: ");
    }
    while (size < 9);

    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < size);

    // Compute number of years
    int year = 0;
    if (size == end)
    {
        year = 0;
    }
    else
    {
        // Loop through the increasing size
        do
        {
            int in = size / 3;
            int out = size / 4;
            size = size + (in - out);
            year++;
        }
        while (size < end);
    }
    // Print the number of years
    printf("Years: %i\n", year);
}