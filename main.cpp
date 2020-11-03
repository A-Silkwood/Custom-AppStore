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

// output the record of the given app
void showRecord(AppInfo app) {
    std::cout << "\tCategory: " << app.category << std::endl;
    std::cout << "\tApplication Name: " << app.name << std::endl;
    std::cout << "\tVersion: " << app.version << std::endl;
    std::cout << "\tSize: " << std::fixed << std::setprecision(1) << app.size << std::endl;
    std::cout << "\tUnits: " << app.units << std::endl;
    std::cout << "\tPrice: $" << std::fixed << std::setprecision(2) << app.price << std::endl;
}

// queries

// finds an app in the hash table; if found it prints out its record
void findApp(std::string app, HashTableEntry **hashTable, int size) {
    // find app in table
    int hashIx = hash(app, size);
    HashTableEntry *curr = &(*hashTable[hashIx]);
    // continues through chain until the end or app is found
    while(curr != NULL) {
        if(strcmp(curr->appName, app) == 0) {
            break;
        }
        curr = curr->next;
    }

    if(curr != NULL) {
        // record was found
        std::cout << "Found Application: " << app << std::endl;
        showRecord(curr->appNode->record);  // print record
    } else {
        // record not found
        std::cout << "Application " << app << " not found." << std::endl;
    }
}

// finds all apps of a given category and prints the names in order of app name
void findCategory(std::string category, Category *categories, int size) {
    int i = 0;
    while(i < size) {
        if(strcmp(categories[i].name, category) == 0) {
            break;
        }
        i++;
    }
}

// updates

// read in query and call the proper function
void readCommand(std::string command, Category *categories, HashTableEntry **hashTable, int catSize, int hashSize) {
    // commands
    if(strcmp(cut(command, ' ', 0, 1), "find") == 0) {
        // find commands
        if(strcmp(cut(command, ' ', 1, 2), "app") == 0) {
            // find app <app_name>
            findApp(cut(command, '"', 1, 2), hashTable, hashSize);
        } else if(strcmp(cut(command, ' ', 1, 2), "category") == 0) {
            findCategory(cut(command, '"', 1, 2), categories);
        } else if(strcmp(command, "find price free") == 0) {
            std::cout << "FIND PRICE FREE: " << command << std::endl;
        } else {
            std::cout << "FIND NOTHING: " << command << std::endl;
        }
    } else if(strcmp(cut(command, ' ', 0, 1), "range") == 0) {
        if(strcmp(cut(command, ' ', 2, 3), "price") == 0) {
            std::cout << "RANGE PRICE: " << command << std::endl;
        } else if(strcmp(cut(command, ' ', 2, 3), "app") == 0) {
            std::cout << "RANGE APP: " << command << std::endl;
        } else {
            std::cout << "RANGE NOTHING: " << command << std::endl;
        }
    } else if(strcmp(cut(command, ' ', 0, 1), "delete") == 0) {
        std::cout << "DELETE: " << command << std::endl;
    } else {
        std::cout << "NOTHING: " << command << std::endl;
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

    // get numbers of queries and updates
    getline(std::cin, input);
    inputStream.str(input);
    inputStream.clear();
    int queriesCount;
    inputStream >> queriesCount;
    // perform all queries and updates
    for(int i = 0; i < queriesCount; i++) {
        getline(std::cin, input);
        readCommand(input, categories, hashTable, catsSize, hashSize);
    }

    // report

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
