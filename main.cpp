#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>

using namespace std;

/**
 * Gets the bearcatII value of a letter from a given character.
 *
 * blank character = 0
 * A, ..., Z are assigned values 1, ..., 26 respectively.
 *
 * @throws std::invalid_argument if the character is not in the set
 *  of character A-Z and space character.
 *
 * @param[letter] letter to get numeric encoding of
 * @returns numeric encoding of the letter
 */
int charToBearcatII(char letter)
{
    if(letter == ' ')
    {
        // Return zero for a space character
        return 0;
    }
    else if(letter >= 'A' && letter <= 'Z')
    {
        // Return offset from 'A' + 1 so 'A' = 1
        return letter - 'A' + 1;
    }
    else
    {
        // Mark letter as invalid and report error to console
        std::cerr << "Invalid letter '" << letter << "' not in range A-Z or space";
        throw std::invalid_argument("Invalid letter for for BearcatII exception");
    }
}

/**
 * Decodes a character from the BearcatII encoding
 *
 * @throws std::invalid_argument if the character is not in the set
 *  of character A-Z and space character values 0, ..., 26.
 *
 * @param[letter] numeric encoding of letter
 * @returns ASCII encoding of the letter as a character
 */
char charFromBearcatII(int letter)
{
    if (letter == 0)
    {
        // space character from zero
        return ' ';
    }
    else if (letter < 27)
    {
        // Return offset from 'A' as a letter minus 1 to account for 'A' = 1
        return 'A' + letter - 1;
    }
    else
    {
        // Mark letter as invalid and report error to console
        std::cerr << "Invalid numeric encoding '" << letter << "' not in range [0,26]";
        throw std::invalid_argument("Invalid numeric encoding for BearcatII exception");
    }

}

/**
 * Convert a message into the form of BearcatII numeric encoding
 * which uses encoding methodology of bearcatII, see charToBearcatII
 * function for encoding description.
 *
 * Combine them into decimal format by shifting them by
 * powers of 27 for each character.
 *
 * N = 20 5 19 20
 * D = 20 * 27 ^ 0 + 5 * 27 ^ 1 + 19 * 27 ^ 2 + 20 * 27 ^ 3
 *
 * @param[word] String of characters to input to algorithm.
 * @param[length] Length of the string of characters
 * @return Encoding of word as BearcatII in numeric representation
 */
unsigned long long int toBearcatII(char* word, int length)
{
    // Variable to store the encoding
    unsigned long long int encoding = 0;
    // Variable to hold current offset, start at 27^0
    unsigned long long int offset = 1;

    // temporary variables for loop
    char letter;
    int bearcatIIChar;

    // Go through and encode each character
    for (int pos = 0; pos < length; pos++)
    {
        // Get the character at the current position
        // Get the upper case representation of the letter
        letter = toupper(word[pos]);
        // Get the encoding for our letter
        bearcatIIChar = charToBearcatII(letter);
        // Add our character with offset
        encoding += bearcatIIChar * offset;
        // shift offset by power of 27
        offset *= 27;
    }

    // Return final computed result
    return encoding;
}

/**
 * Convert a message into a string from the BearcatII numeric encoding.
 * Will read in a given encoded string and parse a fixed length
 * message from the string.
 *
 * @param[encoded] Encoded character represented as a number
 * @param[length] Length of the string of characters
 * @return Decoded form of the message as a string
 */
char* fromBearcatII(unsigned long long int encoded, int length)
{
    // Allocate memory for returning the string
    char* decoded = new char[length + 1];
    // Add string terminator for proper printing to screen
    decoded[length] = '\0';
    // Decode values from the encoded string
    for (int pos = 0; pos < length; pos++)
    {
        // Get the current character stored at this position
        int value = encoded % 27;
        // Put the string at this position into the decoded string
        decoded[pos] = charFromBearcatII(value);
        // Shift the numeric encoding by a power of 27
        encoded /= 27;
    }
    // Return the decoded string
    return decoded;
}

int main()
{
    // Holding variable for user input
    char input[100];

    // prompt user for input and read input
    printf("Please provide an input string: ");
    cin.getline(input, sizeof(input));
    // Get the length of user input
    int length = strlen(input);

    // Encode the user input using bearcatII encoding
    printf("Input string size: %u, value: %s\n", length, input);
    unsigned long long int encoded = toBearcatII(input, length);

    // Debug and log this encoding to screen
    printf("String encoded as numeric value: %u\n", encoded);

    // Decode from bearcatII and log result to screen
    char* decoded = fromBearcatII(encoded, length);
    printf("String decoded as string %s\n", decoded);
}
