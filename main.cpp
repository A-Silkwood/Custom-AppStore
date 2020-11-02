#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "structs.h"
#include "utils.h"

// hash function for storing app info
int hash(std::string key, int k) {
    int sum = strsum(key);
    while(sum >= k) {
        sum -= k;
    }
    return sum;
}

// initializes values for an AppInfo
void initAppInfo(AppInfo *app) {
    std::string input;
    std::stringstream inputStream;
    // app category
    getline(std::cin, input);
    app->category = std::string(input);
    // app name
    getline(std::cin, input);
    app->name = std::string(input);
    // version number
    getline(std::cin, input);
    app->version = std::string(input);
    // app size
    getline(std::cin, input);
    inputStream.str(input);
    inputStream >> app->size;
    // app size units
    getline(std::cin, input);
    app->units = std::string(input);
    // app price
    getline(std::cin, input);
    inputStream.str(input);
    inputStream.clear();
    inputStream >> app->price;
}

// read in each app into a tree node and adds it to the search tree and hash table
void popApps(Category *categories ,HashTableEntry **table, int appCount, int m) {
    for(int i = 0; i < appCount; i++) { // read in each app
        // create app's node for search tree
        Tree *appTree = new Tree;
        initAppInfo(&appTree->record);  // read in next app info
        appTree->left = NULL;
        appTree->right = NULL;
        // create hash entry for node
        HashTableEntry *appHash = new HashTableEntry;
        appHash->appName = appTree->record.name;
        appHash->appNode = appTree;

        // find the proper category to store node
        int s = 0;
        while (strcmp(appTree->record.category, categories[s].name) != 0) { s++; }

        if (categories[s].root == NULL) {
            categories[s].root = appTree;
        } else {
            Tree *curr = categories[s].root;
            while (true) {
                if (strcmp(appTree->record.name, curr->record.name) == -1) {
                    if (curr->left == NULL) {
                        curr->left = appTree;
                        break;
                    } else {
                        curr = curr->left;
                    }
                } else {
                    if (curr->right == NULL) {
                        curr->right = appTree;
                        break;
                    } else {
                        curr = curr->right;
                    }
                }
            }
        }

        // add hash entry to the hash table
        int hashIx = hash(appHash->appName, m);
        // collision handling
        if(table[hashIx] != NULL) {
            appHash->next = table[hashIx];
        } else {
            appHash->next = NULL;
        }
        // add new head to chain
        table[hashIx] = appHash;
    }
}

// deallocates a hash table entry and its app node from memory
void deleteMem(HashTableEntry *entry) {
    if(entry->next != NULL) {
        deleteMem(entry->next);
    }
    delete entry->appNode;
    delete entry;
}

int main() {
    std::string input;
    std::stringstream inputStream;

    // create categories
    // get number of categories
    getline(std::cin, input);
    inputStream.str(input);
    int catsSize;
    inputStream >> catsSize;
    Category *categories = new Category[catsSize];

    // read in and set category names
    for(int i = 0; i < catsSize; i++) {
        getline(std::cin, input);
        categories[i].name = std::string(input);
        categories[i].root = NULL;
    }

    // get number of apps
    getline(std::cin, input);
    inputStream.str(input);
    inputStream.clear();
    int appCount;
    inputStream >> appCount;

    // compute hash table size
    int hashSize = nextPrime(appCount * 2);
    // create hash table
    HashTableEntry **hashTable = new HashTableEntry*[hashSize];
    for(int i = 0; i < hashSize; i++) { // set all pointers to NULL
        hashTable[i] = NULL;
    }

    // populate app store
    popApps(categories, hashTable, appCount, hashSize);



    // deallocate memory
    for(int i = 0; i < hashSize; i++) {
        if(hashTable[i] != NULL) {
            deleteMem(hashTable[i]);
        }
    }
    delete [] categories;
    delete [] hashTable;

    return 0;
}
