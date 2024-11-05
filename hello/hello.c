#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask for name and print
    string name = get_string("What is your name? ");
    printf("hello, %s\n", name);
}
