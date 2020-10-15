#include <iostream>
#include "structs.h"
#include "utils.h"

int main() {
    std::string input;

    // create categories
    getline(std::cin, input);
    int catsSize = stoi(input);
    Category *categories = new Category[catsSize];

    for(int i = 0; i < catsSize; i++) {
        getline(std::cin, input);
        categories[i].name = input;
    }

    return 0;
}
