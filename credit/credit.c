#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get CC number
    long number;
    do {
        number = get_long("Credit card number: ");
    }
    while (number < 0);
    // Checksum function

}

int checksum(long n)
{
    int lastd = n % 10
    n = n - lastd

}