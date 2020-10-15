#include <iostream>
#include <string>
#include "structs.h"
#include "utils.h"

// hash function for storing app info
int hash(const std::string& key, int k) {
    int sum = strsum(key);
    while(sum >= k) {
        sum -= k;
    }
    return sum;
}

// initializes values for an AppInfo and also sets a given string to this app's category
void initAppInfo(AppInfo *app, std::string *category) {
    std::string input;
    // get category
    getline(std::cin, input);
    *category = std::string(input);
    app->category = std::string(*category);
    // get other info
    // app name
    getline(std::cin, input);
    app->name = std::string(input);
    // version number
    getline(std::cin, input);
    app->version = std::string(input);
    // app size
    getline(std::cin, input);
    app->size = stof(input);
    // app size units
    getline(std::cin, input);
    app->units = std::string(input);
    // app price
    getline(std::cin, input);
    app->price = stof(input);
}

int main() {
    std::string input;

    // create categories
    getline(std::cin, input);
    int catsSize = stoi(input);
    Category *categories;
    categories = new Category[catsSize];

    // set category names
    for(int i = 0; i < catsSize; i++) {
        getline(std::cin, input);
        categories[i].name = std::string(input);
    }

    // create app hash table size
    getline(std::cin, input);
    int appCount = stoi(input);
    int hashSize = nextPrime(appCount * 2);
    std::cout << "Prime: " << hashSize << std::endl;

    // create hash table
    HashTableEntry *hashTable;
    hashTable = new HashTableEntry[hashSize];

    // populate apps
    for(int i = 0; i < appCount; i++) {
        // create app's node for search tree
        Tree *app = new Tree;
        std::string category;
        // add info to the record for the node
        initAppInfo(&app->record, &category);

        // find the proper category to store node
        int s = 0;
        while(strcmp(category, categories[s].name) != 0) {s++;}
        // navigate category's tree and add node
        if(categories[s].root == NULL) {
            // make node root of tree
            categories[s].root = app;
        } else {
            // navigate tree
            Tree *curr = categories[s].root;
            while(true) { // breaks when node is added
                if(strcmp(curr->record.name, app->record.name) == 1) {
                    // app name was greater; right tree
                    if(curr->right == NULL) {
                        // add node
                        curr->right = app;
                        break;
                    } else {
                        // check right tree
                        curr = curr->right;
                    }
                } else {
                    // app name was lesser or equal; left tree
                    if(curr->left == NULL) {
                        // add node
                        curr->left = app;
                        break;
                    } else {
                        // check left tree
                        curr = curr->left;
                    }
                }
            }
        }

        // add app info to hash table

    }

    return 0;
}
