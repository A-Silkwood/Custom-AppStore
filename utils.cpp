#include <iostream>

// raise base to the power of exponent; assumes positive exponent
int pow(int base, int exp) {
    int num = 1;
    for(int i = 0; i < exp; i++) {
        num *= base;
    }
    return num;
}

// string to integer; assumes string is only integer chars
int stoi(std::string str) {
    int num = 0;
    for(int i = 0; i < str.length(); i++) {
        num *= 10;
        num += str[i] - '0';
    }

    return num;
}

// string to integer; assumes string is only integer chars and possible decimal point
float stof(std::string str) {
    int num = 0;
    int count = 0;
    bool startCounting = false;

    for(int i = 0; i < str.length(); i++) {
        if(!startCounting && str[i] == '.') {
            startCounting = true;
        } else {
            num *= 10;
            num += str[i] - '0';
            if(startCounting) {count++;}
        }
    }

    return (float)num / (float)pow(10, count);
}

int strsum(std::string str) {
    int sum = 0;
    for(int i = 0; i < str.length(); i++) {
        sum += (int)str[i];
    }
    return sum;
}

// compare strings to find the one that is lesser lexicographically
int strcmp(std::string str1, std::string str2) {
    // get the shorter string length
    int end;
    if(str1.length() >= str2.length()) {
        end = str1.length();
    } else {
        end = str2.length();
    }

    // iterate through strings; if there is a difference determine the lesser string and return
    for(int i = 0; i < end; i++) {
        // find difference
        if(str1[i] != str2[i]) {
            // determine lesser value
            if(str1[i] < str2[i]) {
                return -1; // str1 was less than str2
            } else {
                return 1; // str2 was less than str1
            }
        }
    }

    // both strings were equal up to end length; determine the lesser/shorter string and return
    if(str1.length() == str2.length()) {
        return 0; // strings were equal
    } else if(str1.length() < str2.length()) {
        return -1; // str1 was shorter than str2
    } else {
        return 1; // str2 was shorter than str1
    }
}

// finds next prime number
int nextPrime(int num) {
    // keeps making bigger arrays until prime is found
    int mult = 2;
    while(true) {
        // create max which will be used as the size of the array
        int max = num * mult++;
        bool nums[max]; // true means non prime; false means prime
        nums[0] = true; // set 1 to non prime

        // look for prime
        for(int i = 1; i < max; i++) {
            // found a prime
            if(!nums[i]) {
                // check if prime is greater than number given
                if(num < i+1) {
                    return i+1;
                }
                // mark numbers as non prime
                for(int j = i+(i+1); j < max; j += i+1) {
                    nums[j] = true;
                }
            }
        }
    }
}
