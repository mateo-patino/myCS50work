#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

string encipher(string plaintxt, string key);

int main(int argc, string argv[])
{
    // Check if argv[1] exists before measuring its size (otherwise you'll get a memory error)
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Store the size of the key passed in
    int length = strlen(argv[1]);
    if (length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // Iterate through all characters to see if they're outside the 32-bit decimal range per ASCII.
    for (int l = 0; l < length; l++)
    {
        if ((argv[1][l] > 122 || argv[1][l] < 65) || (argv[1][l] > 90 && argv[1][l] < 97))
        {
            printf("Invalid key. No non-alphabetical characters allowed.\n");
            return 1;
        }
    }
    // Create an array with false-valued elements and use the characters' ASCII value to track their ocurrences with trues.
    int visited[128] = {false};
    for (int i = 0; i < length; i++)
    {
        if (visited[(int) argv[1][i]] == false)
        {
            visited[(int) argv[1][i]] = true;
        }
        else
        {
            printf("%c is a repeated character. key cannot have repeated characters\n", argv[1][i]);
            return 1;
        }
    }

    // Get plaintext and print ciphertext
    string plaintxt = get_string("plaintext: ");
    printf("ciphertext: %s\n", encipher(plaintxt, argv[1]));

    // Exit
    return 0;
}

// Substitution cipher
string encipher(string plaintxt, string key)
{
    // Initialize an index from 0 to 25 for the alphabet
    int index_alphbt = 0;
    string ciphertxt = plaintxt;

    for (int i = 0; i < strlen(plaintxt); i++)
    {
        // Check for spaces and punctutation
        if ((char) plaintxt[i] == ' ')
        {
            ciphertxt[i] = ' ';
        }
        else if (ispunct(plaintxt[i]) || isdigit(plaintxt[i]))
        {
            ciphertxt[i] = plaintxt[i];
        }
        else
        {
            if (isupper(plaintxt[i]))
            {
                // Use -65 to access uppercase ASCII values
                index_alphbt = (int) plaintxt[i] - 65;
                ciphertxt[i] = toupper((char) key[index_alphbt]);
            }
            else
            {
                // Use -97 to access lowercase ASCII values
                index_alphbt = (int) plaintxt[i] - 97;
                ciphertxt[i] = tolower((char) key[index_alphbt]);
            }

        }
    }
    return ciphertxt;
}