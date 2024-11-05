#include <cs50.h>
#include <stdio.h>
#include <math.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

//GET CENTS FUNCTION
int get_cents(void)
{
    int result;
    do {
        result = get_int("Cents owed: ");
    }
    while (result < 0);
    return result;
}

//QUARTERS FUNCTION
int calculate_quarters(int cents)
{
    if (cents < 25)
    {
        return 0;
    }
    else
    {
        //USE FLOOR FUNCTION TO RETURN NUMBER OF QUARTERS
        double divisionq = cents / 25;
        int resultq = floor(divisionq);
        return resultq;
    }
}

int calculate_dimes(int cents)
{
    if (cents < 10)
    {
        return 0;
    }
    else
    {
        // USE FLOOR FUNCTION LIKE BEFORE
        double divisiond = cents / 10;
        int resultd = floor(divisiond);
        return resultd;
    }
}

int calculate_nickels(int cents)
{
    if (cents < 5)
    {
        return 0;
    }
    else
    {
        double divisionn = cents / 5;
        int resultn = floor(divisionn);
        return resultn;
    }
}

int calculate_pennies(int cents)
{
    if (cents == 0)
    {
        return 0;
    }
    else
    {
        return cents;
    }
}