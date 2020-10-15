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
void initAppInfo(AppInfo *app) {
    std::string input;
    // get category
    getline(std::cin, input);
    app->category = std::string(input);
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

void popApps(Category *categories ,HashTableEntry **table, int appCount, int m) {
    // read each app and add to the categories and hash table
    for(int i = 0; i < appCount; i++) {
        // create app's node for search tree
        Tree *appTree = new Tree;
        initAppInfo(&appTree->record);  // read in next app info

        // find the proper category to store node
        int s = 0;
        while(strcmp(appTree->record.category, categories[s].name) != 0) {s++;}
        // navigate category's tree and add node
        if(categories[s].root == NULL) {
            // make node root of tree
            categories[s].root = appTree;
        } else {
            // navigate tree
            Tree *curr = categories[s].root;
            while(true) { // breaks when node is added
                if(strcmp(curr->record.name, appTree->record.name) == 1) {
                    // app name was greater; right tree
                    if(curr->right == NULL) {
                        // add node
                        curr->right = appTree;
                        break;
                    } else {
                        // check right tree
                        curr = curr->right;
                    }
                } else {
                    // app name was lesser or equal; left tree
                    if(curr->left == NULL) {
                        // add node
                        curr->left = appTree;
                        break;
                    } else {
                        // check left tree
                        curr = curr->left;
                    }
                }
            }
        }

        // create hash entry for node
        HashTableEntry *appHash = new HashTableEntry;
        appHash->appName = appTree->record.name;
        appHash->appNode = appTree;

        // add hash entry to array
        int hashIx = hash(appHash->appName, m);
        // check for collision
        if(table[hashIx] != NULL) {
            appHash->next = table[hashIx];
        }
        // add new head to chain
        table[hashIx] = appHash;
    }
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
    HashTableEntry *hashTable[hashSize];

    // populate app store
    popApps(categories, hashTable, appCount, hashSize);

    return 0;
}
