#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <stdio.h>
#include <string.h>

#define ubigint unsigned long long int
#define bigint long long int

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
ubigint toBearcatII(char* word, int length)
{
    // Variable to store the encoding
    ubigint encoding = 0;
    // Variable to hold current offset, start at 27^0
    ubigint offset = 1;

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
char* fromBearcatII(ubigint encoded, int length)
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

/**
 * Raises x to power y in modulo p
 *
 * computes x ^ y mod p
 *
 * @param[x] value of x
 * @param[y] Power to raise the value of x to
 * @param[p] Modulus for this operation
 * @return Computed value of x ^ y mod p
 */
ubigint modPow(ubigint x, ubigint y, ubigint p)
{
    // initial result
    ubigint result = 1;
    // Ensure x is in modulo p
    x %= p;

    while(y > 0)
    {
        // If pow is odd, multiply x with the result
        if (y % 2 == 1)
        {
            result = (result * x) % p;
        }
        // Right shift pow by one binary digit
        y /= 2;
        // Square the value x
        x = (x * x) % p;
    }

    // return final result
    return result;
}

/**
 * Perform a single iteration of the miller rabin
 * primality test.
 *
 * @param[n] Number to test primality of
 * @param[b] Some value such that n = 2^b * r + 1 for some r >= 1
 * @param[gen] Generator for selecting random numbers
 * @param[dis] Distribution to draw random numbers from,
 *      should be defined in range [2, n - 2]
 * @returns True or false. Always correct when false and has a probability of 0.75
 *      of being correct when returning true.
 */
bool millerRabinIteration(
    ubigint n,
    ubigint b,
    std::mt19937 &gen,
    std::uniform_int_distribution<ubigint> &dis)
{
    // Select a random value in range [2, n - 2]
    ubigint a = dis(gen);

    // Compute a^b % n
    ubigint x = modPow(a, b, n);

    if (x == 1  || x == n-1)
       return true;

    // While b is less than n - 1, for each k iteration
    while (b < n - 1)
    {
        // Square the value of x
        x = (x * x) % n;
        b *= 2;

        if (x == 1)
        {
            return false;
        }
        else if (x == n - 1)
        {
            return true;
        }
    }

    return false;
}

/**
 * Test to determine if a given integer n is prime using
 * the Miller-Rabin primality test. This is a probabilistic algorithm
 * so it will repeat for k interations until it is reasonably sure the value
 * n is prime. Each iteration of k has a 0.75 probability chance of incorrectly
 * assuming the number is prime.
 *
 * @param[n] Number to test primality of
 * @param[k] Number of iterations to repeat miller-rabin primality test, higher
 *      value of k means more accuracy
 * @returns False if not prime, true if relatively confident the number is prime.
 */
bool millerRabinPrimalityTest(ubigint n, int k)
{
    // Edge cases for n < 4
    if (n <= 1 || n == 4)
    {
        return false;
    }
    if (n == 2 || n == 3)
    {
        return true;
    }

    // Create a random uniform distribution for values between 2 and n - 2
    // From this nice stack overflow post https://stackoverflow.com/questions/28115724/getting-big-random-numbers-in-c-c
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<ubigint> dis(2, n - 2);

    // Find positive integers b such that n = 2^b * r + 1 for some r >= 1
    ubigint b = n - 1;
    while (b % 2 == 0)
    {
        b /= 2;
    }

    // Perform test iteratively k times
    for (int i = 0; i < k; i++)
    {
        if (!millerRabinIteration(n, b, gen, dis))
        {
            return false;
        }
    }

    // return true if it passes all iterations
    return true;
}

/**
 * Generate random numbers form a distribution until a given random number is found
 *
 * @param[gen] Generator for selecting random numbers
 * @param[dis] Distribution to draw random numbers from
 * @returns A randomly selected prime from the distribution
 */
ubigint findRandomPrimeNumber(
    std::mt19937 &gen,
    std::uniform_int_distribution<ubigint> &dis)
{
    // Holding variable for our generated prime number
    ubigint value;
    do
    {
        // Select a number from the distribution
        value = dis(gen);
    // Keep iterating until the found value is prime (with confidence iterations using miler rabin)
    } while (!millerRabinPrimalityTest(value, 30));
    return value;
}

/**
 * Compute the greatest common divisor between two values
 *
 * @param[a] First value
 * @param[b] Second value
 * @returns greatest common divisor between two values
 */
ubigint gcd(ubigint a, ubigint b)
{
    // Temporary holding variable
    ubigint temp;
    // While a != b, compute next step of gcd computation
    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    // Return final gcd
    return a;
}

/**
 * Algorithm to compute extended euclidian gcd
 */
ubigint extended_gcd(ubigint a, ubigint b, bigint &s, bigint &t)
{
    // base case
	if (b == 0)
    {
		s = 1;
		t = 0;
		return a;
	}
    // recursive call
	ubigint r = a % b;
    ubigint q = a / b;

	bigint s_, t_;

	ubigint ret_val = extended_gcd(b, r, s_, t_);

	t = s_ - q * t_;
	s = t_;

	return ret_val;
}

int main()
{
    // Select some values p and q by generating large prime numbers
    // Lets generate them between the range of 2^20 - 1 and 2^30 - 1
    //  to ensure n = pq < 2^64 - 1 (maximum value of long long int)
    ubigint minValue = 1048575;
    ubigint maxValue = 1073741823;
    // Create our random number generator
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<ubigint> dis(minValue, maxValue);
    // Randomly select a p and q from the distribution
    // ubigint p = findRandomPrimeNumber(gen, dis);
    // ubigint q = findRandomPrimeNumber(gen, dis);
    ubigint p = 7;
    ubigint q = 11;
    // Compute n = nq
    ubigint n = p * q;

    // debug log n, p, and q to screen
    std::cout << "n: " << n << ", p: " << p << ", q: " << q << std::endl;

    // Compute private key from given public key
    ubigint phi_n = (p - 1) * (q - 1);
    // Prompt user for value of e
    ubigint e;
    char input[100];
    std::cout << "Please provide a public key value e: ";
    std::cin.getline(input, sizeof(input));
    e = std::atoi(input);
    // Prompt user for new input until e and n are co-prime
    while (gcd(e, phi_n) != 1)
    {
        std::cout << "Sorry, " << e << " is not co-prime to " << n << std::endl;
        std::cout << "Please provide another public key value e: ";
        std::cin.getline(input, sizeof(input));
        e = std::atoi(input);
    }
    // d is private key
    bigint d, t;
    extended_gcd(e, phi_n, d, t);
    // Ensure private key is greater than zero
    if (d < 0)
    {
        d += phi_n;
    }
    std::cout << "e: " << e << ", d: " << d << std::endl;

    // Get user input for message M
    // Holding variable for user input
    char M[100];

    // prompt user for input and read input
    printf("Please provide an input string: ");
    std::cin.getline(M, sizeof(M));
    // Get the length of user input
    int length = strlen(M);

    // Encode the user input using bearcatII encoding
    std::cout << "M: " << M << std::endl;
    ubigint encoded = toBearcatII(M, length);

    // Get encrypted value using rsa C = M ^ e mod n
    ubigint cipher_encoded = modPow(encoded, e, n);

    // Get the cipher encoded as a string
    char* cipher_text = fromBearcatII(cipher_encoded, length);
    std::cout << "C: " << cipher_text << std::endl;

    // Decrypt value using rsa M = C ^ d mod n
    //    M = M ^ (e * d) mod n, e * d = 1 mod n
    ubigint decrypted_encoded = modPow(cipher_encoded, d, n);

    // Decode from bearcatII and log result to screen
    char* decrypted_decoded = fromBearcatII(decrypted_encoded, length);
    std::cout << "String decrypted as string " << decrypted_decoded << std::endl;

    return 0;
}
