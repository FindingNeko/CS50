#include <cs50.h>
#include <stdio.h>
#include <math.h>

/*
* This program checks a credit card number to see if it is valid, and if it is VISA, AMEX, or MASTERCARD.
*/

int addD(int); // A function I made! See bottom.
####################################################################################################
int main(void)
{
    /*
    * Prompt the user for input, and check that the number provided is in a reasonable range.
    */
    long userInput = get_long("Type your CC number here this is totally legit I promise: ");
    if (userInput < 4000000000000 || userInput > 5599999999999999)
    {
        printf("INVALID\n");
        return 0;
    }
    /*
    * Convert the user input from a long to a bunch of int vars representing each digit of the long.
    */
    int d1e = userInput % 10;
    userInput = (userInput - d1e) / 10;
    int d2e = userInput % 10;
    userInput = (userInput - d2e) / 10;
    int d3e = userInput % 10;
    userInput = (userInput - d3e) / 10;
    int d4e = userInput % 10;
    userInput = (userInput - d4e) / 10;
    int d5e = userInput % 10;
    userInput = (userInput - d5e) / 10;
    int d6e = userInput % 10;
    userInput = (userInput - d6e) / 10;
    int d7e = userInput % 10;
    userInput = (userInput - d7e) / 10;
    int d8e = userInput % 10;
    userInput = (userInput - d8e) / 10;
    int d9e = userInput % 10;
    userInput = (userInput - d9e) / 10;
    int d10e = userInput % 10;
    userInput = (userInput - d10e) / 10;
    int d11e = userInput % 10;
    userInput = (userInput - d11e) / 10;
    int d12e = userInput % 10;
    userInput = (userInput - d12e) / 10;
    int d13e = userInput % 10;
    userInput = (userInput - d13e) / 10;
    int d14e = userInput % 10;
    userInput = (userInput - d14e) / 10;
    int d15e = userInput % 10;
    userInput = (userInput - d15e) / 10;
    int d16e = userInput % 10;
    /*
    * Check if card is valid by calculating the checksum, and printing the type of card once identified. Reject bad card numbers.
    */
    if (d16e != 0) // Check if number is 16 digits
    {
        if ((addD(d16e) + addD(d14e) + addD(d12e) + addD(d10e) + addD(d8e) + addD(d6e) + addD(d4e) + addD
             (d2e) + d1e + d3e + d5e + d7e + d9e + d11e + d13e + d15e) % 10 != 0)
        {
            printf("INVALID\n");    // Failed Checksum
            return 0;
        }
        if (d16e == 4)
        {
            printf("VISA\n");       // if it's 16 digits, and 1st digit is a 4, and it passes checksum, IT MUST BE A VISA!
            return 0;
        }
        if (d16e == 5 && d15e > 0 && d15e < 6)
        {
            printf("MASTERCARD\n"); // if it's 16 digits, and 1st digit is 51 to 56, and it passes checksum, IT MUST BE A MASTERCARD!!
            return 0;
        }
        printf("INVALID\n");        // if it's 16 digits, and the first digits weren't 4 or 51 through 56, IT MUST BE INVALID!
        return 0;
    }
    if (d15e != 0)
    {
        if ((addD(d14e) + addD(d12e) + addD(d10e) + addD(d8e) + addD(d6e) + addD(d4e) + addD
             (d2e) + d1e + d3e + d5e + d7e + d9e + d11e + d13e + d15e) % 10 != 0)
        {
            printf("INVALID\n");    // Failed Checksum
            return 0;
        }
        if (d15e == 3 && (d14e == 4 || d14e == 7))
        {
            printf("AMEX\n");   // if it's 15 digits, and 1st digits are 34 or 37, and it passes checksum, IT MUST BE A MASTERCARD!
            return 0;
        }
        printf("INVALID\n");    // if it's 15 digits, and the 1st digits aren't 34 or 37, IT MUST BE INVALID!
        return 0;
    }
    if (d14e != 0)
    {
        printf("INVALID\n");    // if it's 14 digits, IT MUST BE INVALID!
        return 0;
    }
    if (d13e == 0)
    {
        printf("INVALID\n");    // if it isn't at least 13 digits, IT MUST BE INVALID!
        return 0;
    }
    if ((addD(d12e) + addD(d10e) + addD(d8e) + addD(d6e) + addD(d4e) + addD
         (d2e) + d1e + d3e + d5e + d7e + d9e + d11e + d13e) % 10 != 0)
    {
        printf("INVALID\n");    // Failed Checksum
        return 0;
    }
    if (d13e == 4)
    {
        printf("VISA\n");       // if it's 13 digits, and 1st digit is a 4, and it passes checksum, IT MUST BE A VISA!
        return 0;
    }
    printf("INVALID\n");        // if it's 13 digits, and the first digit isn't a 4, IT MUST BE INVALID!
    return 0;
}


/*
* This function takes an int, multiplies it by two, and then adds together the digits of the base 10 repesentation to return a new int.
*/
int addD(int x)
{
    if (2 * x > 9)
    {
        x = 2 * x - 9;
    }
    else
    {
        x = 2 * x;
    }
    return x;
}