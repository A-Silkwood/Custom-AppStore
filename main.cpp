#include <iostream>
#include <string>
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

// initializes values for an AppInfo and also sets a given string to this app's category
void initAppInfo(AppInfo *app) {
    std::string input;
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
    app->size = stof(input);
    // app size units
    getline(std::cin, input);
    app->units = std::string(input);
    // app price
    getline(std::cin, input);
    app->price = stof(input);
}

// read each app into a tree node and add it to the search tree and hash table
void popApps(Category *categories ,HashTableEntry **table, int appCount, int m) {
    // read each app and add to the categories and hash table
    for(int i = 0; i < appCount; i++) {
        // create app's node for search tree
        Tree *appTree = new Tree;
        initAppInfo(&appTree->record);  // read in next app info
        appTree->left = NULL;
        appTree->right = NULL;

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
                if(strcmp(appTree->record.name, curr->record.name) == 1) {
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

// used to display search tree info
void showTree(Tree curr, int l, int r) {
    if(curr.left == NULL && curr.right == NULL) {
        std::cout << "Leaf: ";
    }
    std::cout << "(" << l << "," << r << ")" << curr.record.name << std::endl;
    if(curr.left != NULL) {
        std::cout << "Left: ";
        showTree(*curr.left, l+1, r);
    }
    if(curr.right != NULL) {
        std::cout << "Right: ";
        showTree(*curr.right, l, r+1);
    }
}

// displays all app info in proper format
void displayApp(AppInfo app) {
    std::cout << "\tCategory: " << app.category << std::endl;
    std::cout << "\tApplication Name: " << app.name << std::endl;
    std::cout << "\tVersion: " << app.version << std::endl;
    std::cout << "\tSize: " << app.size << std::endl;
    std::cout << "\tUnits: " << app.units << std::endl;
    std::cout << "\tPrice: $" << app.price;
}

//queries

// finds an app
void findApp(HashTableEntry **table, std::string name, int k) {
    HashTableEntry *entry = table[hash(name, k)];
    while(entry != NULL) {
        if(name == entry->appName) {
            // found app
            std::cout << "Found Application:\t" << name << std::endl;
            displayApp(entry->appNode->record);
            return;
        }
        entry = entry->next;
    }
    // app not found
    std::cout << "Application " << name << "not found.";
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
        categories[i].root = NULL;
    }

    // create app hash table size
    getline(std::cin, input);
    int appCount = stoi(input);
    int hashSize = nextPrime(appCount * 2);

    // create hash table
    HashTableEntry *hashTable[hashSize];
    for(int i = 0; i < hashSize; i++) {
        hashTable[i] = NULL;
    }

    // populate app store
    popApps(categories, hashTable, appCount, hashSize);

    // start accepting queries and/or updates
    getline(std::cin, input);
    int queries = stoi(input);
    for(int i = 0; i < queries; i++) {
        // read in query
        getline(std::cin, input);
        // execute query; only find app <app name> is implemented
        if (input.substr(0, 4) == "find") {
            if (input.substr(5, 3) == "app") {
                findApp(hashTable, input.substr(9, std::string::npos), hashSize);
            } else if (input.substr(5, 8) == "category") {

            } else if (input.substr(5, 5) == "price") {

            }
        } else if (input.substr(0, 5) == "price") {

        } else if (input.substr(0, 6) == "delete") {

        }
        if(i + 1 != queries) {
            std::cout << std::endl << std::endl;
        }

        // report is unimplemented
    }

    return 0;
}
