#include <stdio.h>
#include <cs50.h>

/*
 * This is a simple program that asks the users name and returns a greeting.
 * The included packages enable printf and a string variable with user input.
 */

// int is data type of main funtion's return value, void means no input args
int main(void)
{
    // get_string is used to get user input, it is stored in a string variable
    string userName = get_string("What's your name? ");
    // printf prints a formatted string where the variable userName replaces %s
    printf("hello, %s\n", userName);
}