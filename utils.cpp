#include <iostream>

// returns the start of the string up until the first occurrence of the delimiter
std::string cut(std::string str, char delim, int start, int end) {
    if(end-start <= 0) {return "";}
    int ix = -1;
    int found = 0;
    int startIx;
    bool startFound = false;
    if(start == 0) {startIx = -1; startFound = true;}
    while((found < end || (end == -1 && !startFound)) && ix < (int)str.length()) {
        ix++;
        if(str[ix] == delim) {
            found++;
        }
        if(!startFound && found == start) {
            startIx = ix;
            startFound = true;
        }
    }
    if((found < end || end == -1) && startFound) {
        return str.substr(startIx+1);
    } else if(!startFound) {
        return "";
    } else {
        return str.substr(startIx + 1, ix - startIx - 1);
    }
}

// returns the sum of all the characters of a string converted to an integer
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
