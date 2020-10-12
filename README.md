# Homework 3

Nicholas Maltbie

Advanced Algorithms class Fall 2020

# Assignment

Write a C++ program that involves implementing the RSA cryptosystem. In practice for the encryption to be secure and to handle larger messages you would need to utilize a class for large integers. However, for this assignment you can use built-in types to store integers, e.g., `unsigned long long int`.

Also, rather than using the ASCII table for this assignment use BEARCATII, which restricts the characters to the blank character and the lower-case letters of the alphabet as follows:

* blank character is assigned the value 0.
* A, ..., Z are assigned the values 1, ..., 26, respectively

The message M will be represented by replacing each character in the message with its assigned integer base 27. For example, the message M = "TEST" will be represented as

```
N = 20 5 19 20
```

Translating this to decimal we obtain:
```
D = 20 + 19*27 + 5*27^2 + 20*27^3 = 397838
```

Note that to convert back to base 27, we simply apply the algorithm we discussed in class, i.e., the least significant digit (rightmost) is obtained by performing the operations D mod 27 and performing a recursive call with D/27. For the example above we obtain,


```
  397838 / 27, 397838 mod 27 = 14734, 20
→  14734 / 27,  14734 mod 27, 20 = 545, 19, 20
→    545 / 27,    545 mod 20, 19, 20 = 20, 5, 19, 20 = N
```

Find primes _p_ and _q_ by choosing positive integers at random and testing for primality using *Miller-Rabin* probabilistic algorithm.

Your program should
1. prompt the user to input a positive integer representing the public key _e_.
    * If the user enters a number that is not relatively prime to _n = pq_, then have the user reenter and keep doing this until _e_ and _n_ are coprime, i.e., gcd(_e_,φ(_n_)) = 1.
2. Also prompt the user to enter the message M (as a character string). For handing purposes, run your program with _M_ = "TEST".
3. Output _p_, _q_, _n_, _M_, _C_, _P_ where _C_ is the encrypted message, i.e., cypher text, and _P_ is the decrypted message, i.e., plaintext.

If your program is working correctly then _M_ should be equal to _P_.

# Windows Setup

Need to have g++ installed, go to this website to setup a g++ compiler [https://osdn.net/projects/mingw/releases/](https://osdn.net/projects/mingw/releases/).
When installing, make sure to select the install option for `mingw32-gcc-g++-bin` when installing to include the version of the c++ compiler we need for this project.
Note, you may need to add `C:\MinGW\bin` to your path and restart your terminal.
Then there should be a command for `mingw32-g++.exe` available.

# Compiling the program

Inn order to compile the program, use the following command to setup and create the executable. The executable file will be named `a.out` and be placed in your current working directory.

```bash
mingw32-g++.exe main.cpp
```
