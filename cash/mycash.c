#include <stdio.h>
#include <cs50.h>
#include <math.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void) {

// GET CENTS
    int limit = get_int("Maximum numbers: ");
    for (int c = 0; c <= limit; c++) {
        int cents = c;
        printf("Owed: %d ", cents);

        int quarters = calculate_quarters(cents);
        cents = cents - quarters * 25;

        int dimes = calculate_dimes(cents);
        cents = cents - dimes * 10;

        int nickels = calculate_nickels(cents);
        cents = cents - nickels * 5;

        int pennies = calculate_pennies(cents);
        cents = cents - pennies * 1;

        int coins = quarters + dimes + nickels + pennies;

        printf("- R: %i - Q: %i - D: %i - N: %i - P: %i\n",
        coins, quarters, dimes, nickels, pennies);
    }

}

//QUARTERS FUNCTION
int calculate_quarters(int cents) {
    if (cents < 25) {
        return 0;
    }
    else {
        //USE FLOOR FUNCTION TO RETURN NUMBER OF QUARTERS
        double divisionq = cents / 25;
        int resultq = floor(divisionq);
        return resultq;
    }
}

int calculate_dimes(int cents) {
    if (cents < 10) {
        return 0;
    }
    else {
        // USE FLOOR FUNCTION LIKE BEFORE
        double divisiond = cents / 10;
        int resultd = floor(divisiond);
        return resultd;
    }
}

int calculate_nickels(int cents) {
    if (cents < 5) {
        return 0;
    }
    else {
        double divisionn = cents / 5;
        int resultn = floor(divisionn);
        return resultn;
    }
}

int calculate_pennies(int cents) {
    if (cents == 0) {
        return 0;
    }
    else {
        return cents;
    }
}