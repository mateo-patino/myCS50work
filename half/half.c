// Calculate your half of a restaurant bill
// Data types, operations, type casting, return value

#include <cs50.h>
#include <stdio.h>

float half(float bill, float tax, int tip);

int main(void)
{
    float bill_amount = get_float("Bill before tax and tip: ");
    float tax_percent = get_float("Sale Tax Percent: ");
    int tip_percent = get_int("Tip percent: ");

    // Print how much each one owes
    printf("You will owe $%.2f each!\n", half(bill_amount, tax_percent, tip_percent));
}

float half(float bill, float tax, int tip)
{
    // Convert the tax and tip rate to decimal format by dividing by 100
    float taxes = bill * (tax / (float)100);
    float tips = (bill + taxes) * (tip / (float)100);
    bill = bill + taxes + tips;
    return (bill / 2);
}
