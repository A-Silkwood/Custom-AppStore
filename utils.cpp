#include <iostream>

int stoi(const std::string& str) {
    int num = 0;
    for(int i = 0; i < str.length(); i++) {
        num *= 10;
        num += str[i] - '0';
    }

    return num;
}
