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

// outputs all the app names of a given tree in order
void showTree(Tree *root) {
    if(root->left != NULL) {showTree(root->left);}  // output left tree first
    std::cout << '\t' << root->record.name << std::endl; // output this node
    if(root->right != NULL) {showTree(root->right);} // output right tree last
}

// outputs the names of apps in a given tree in order that are within the range given
bool showTreePriceRange(Tree *root, float low, float high) {
    bool left = false; bool right = false; bool curr = false;

    if(root->left != NULL) {left = showTreePriceRange(root->left, low, high);} // output left tree first
    // outputs if low and high are -1 or price of app is within low and high
    if(root->record.price >= low && root->record.price <= high) {
        std::cout << '\t' << root->record.name << std::endl; // output current root
        curr = true;
    }
    if(root->right != NULL) {right = showTreePriceRange(root->right, low, high);} // output right tree last

    return left || curr || right; // returns true if any output occurs
}

// outputs the names of apps in a given tree in order that are within the range given
bool showTreeStringRange(Tree *root, std::string low, std::string high) {
    bool left = false; bool right = false; bool curr = false;

    if(root->left != NULL) {left = showTreeStringRange(root->left, low, high);} // output left tree first
    // outputs if low and high are -1 or price of app is within low and high
    if(strcmp(root->record.name, low) != -1 && strcmp(root->record.name, high) != 1) {
        std::cout << '\t' << root->record.name << std::endl; // output current root
        curr = true;
    }
    if(root->right != NULL) {right = showTreeStringRange(root->right, low, high);} // output right tree last

    return left || curr || right; // returns true if any output occurs
}

// inserts a binary search tree into another tree
void insertTree(Tree *insert, Tree *tree) {
    if(insert != NULL) {
        insertTree(insert->left, tree);
        insertTree(insert->right, tree);
        insert->right = NULL;
        insert->left =NULL;
        Tree *curr = tree;
        while(curr != NULL) {
            if(strcmp(insert->record.name, tree->record.name) == -1) {
                if(curr->left != NULL) {
                    curr = curr->left;
                } else {
                    curr->left = insert;
                    break;
                }
            } else {
                if(curr->right != NULL) {
                    curr = curr->right;
                } else {
                    curr->right = insert;
                    break;
                }
            }
        }
    }
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
    // output
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
        if(strcmp(categories[i].name, category) == 0) {break;}
        i++;
    }
    // output
    if(i < size) {
        if(categories[i].root != NULL) {
            // output all apps in category
            std::cout << "Category: " << category << std::endl;
            showTree(categories[i].root);
        } else {
            // no apps in category
            std::cout << "Category " << category << " no apps found." << std::endl;
        }
    } else {
        // category does not exist
        std::cout << "Category " << category << " not found." << std::endl;
    }
}

// finds all free apps
void findPriceFree(Category *categories, int size) {
    for(int i = 0; i < size; i++) {
        std::cout << "Free Applications in Category: " << categories[i].name << std::endl;
        // output free apps
        if(categories[i].root == NULL || !showTreePriceRange(categories[i].root, 0, 0)) {
            std::cout << "\tNo free application found" << std::endl;
        }
    }
}

// finds all apps within a price range in a specific category
void rangePrice(std::string category, Category *categories, int size, float low, float high) {
    int i = 0;
    while(i < size) {
        if(strcmp(categories[i].name, category) == 0) {break;}
        i++;
    }
    std::cout << "Applications in Price Range (" << low << "," << high << ") in Category: " << category << std::endl;
    // output apps in price range
    if(categories[i].root == NULL || !showTreePriceRange(categories[i].root, low, high)) {
        std::cout << "\tNo applications found in " << category;
        std::cout << " for the given range (" << low << "," << high << ")" << std::endl;
    }
}

// finds all apps within a alphabet range in a specific category
void rangeApp(std::string category, Category *categories, int size, std::string low, std::string high) {
    int i = 0;
    while(i < size) {
        if(strcmp(categories[i].name, category) == 0) {break;}
        i++;
    }
    std::cout << "Applications in Range (" << low << "," << high << ") in Category: " << category << std::endl;
    // output apps in price range
    if(categories[i].root == NULL || !showTreeStringRange(categories[i].root, low, high)) {
        std::cout << "\tNo applications found in " << category;
        std::cout << " for the given range (" << low << "," << high << ")" << std::endl;
    }
}

// updates
void deleteApp(std::string category, std::string app, Category *categories, int catSize,
               HashTableEntry ** hashtable, int hashSize) {
    // remove node from tree
    int i = 0;
    while(i < catSize) {
        if(strcmp(categories[i].name, category) == 0) {break;}
        i++;
    }
    if(i < catSize && categories[i].root != NULL) {
        // find node and parent
        Tree *parent;
        Tree *curr = categories[i].root;
        bool rightBranch;
        while(curr != NULL) {
            if(strcmp(curr->record.name, app) == 0) {
                break;
            } else {
                parent = curr;
                if (strcmp(curr->record.name, app) == -1) {
                    curr = curr->left;
                    rightBranch = false;
                } else {
                    curr = curr->right;
                    rightBranch = true;
                }
            }
        }
        if(curr == NULL) {
            std::cout << "Application " << app << " not found in category " << category;
            std::cout << "; unable to delete." << std::endl;
        } else {
            if(parent == NULL) {
                if(curr->right == NULL) {
                    categories[i].root = curr->left;
                } else {
                    Tree *left = curr->right->left;
                    curr->right->left = curr->left;
                    categories[i].root = curr->right;
                    insertTree(left, categories[i].root);
                }
            } else {
                if(curr->right == NULL) {
                    if(rightBranch) {
                        parent->right = curr->left;
                    } else {
                        parent->left = curr->left;
                    }
                } else {
                    Tree *left = curr->right->left;
                    curr->right->left = curr->left;
                    if(rightBranch) {
                        parent->right = curr->right;
                    } else {
                        parent->left = curr->right;
                    }
                    insertTree(left, categories[i].root);
                }
            }
            delete curr;
            HashTableEntry *entry = hashtable[hash(app, hashSize)];
            while(entry != NULL && strcmp(entry->appName, app) != 0) {
                entry = entry->next;
            }
            delete entry;
        }
    } else {
        std::cout << "Application " << app << " not found in category " << category;
        std::cout << "; unable to delete." << std::endl;
    }
}


// read in query and call the proper function
void readCommand(std::string command, Category *categories, HashTableEntry **hashTable, int catSize, int hashSize) {
    // commands
    if(strcmp(cut(command, ' ', 0, 1), "find") == 0) {
        // find queries
        if(strcmp(cut(command, ' ', 1, 2), "app") == 0) {
            // find app <app_name>
            findApp(cut(command, '"', 1, 2), hashTable, hashSize);
        } else if(strcmp(cut(command, ' ', 1, 2), "category") == 0) {
            // find category <category_name>
            findCategory(cut(command, '"', 1, 2), categories, catSize);
        } else if(strcmp(command, "find price free") == 0) {
            // find price free
            findPriceFree(categories, catSize);
        }
    } else if(strcmp(cut(command, ' ', 0, 1), "range") == 0) {
        // range queries
        if(strcmp(cut(command, ' ', 2, 3), "price") == 0) {
            // range <category_name> price <low> <high>
            std::string temp = cut(command, '"', 2, 3);
            std::string low = cut(temp, ' ', 3, 4);
            std::string high = cut(temp, ' ', 4, 5);
            std::stringstream inputStream;
            float lowF, highF;
            inputStream.str(low);
            inputStream >> lowF;
            inputStream.str(high);
            inputStream.clear();
            inputStream >> highF;
            rangePrice(cut(command, '"', 1, 2), categories, catSize, lowF, highF);
        } else if(strcmp(cut(command, ' ', 2, 3), "app") == 0) {
            // range <category_name> app <low> <high>
            std::string low = cut(command, '"', 3, 4);
            std::string high = cut(command, '"', 4, 5);
            rangeApp(cut(command, '"', 1, 2), categories, catSize, low, high);
        }
    } else if(strcmp(cut(command, ' ', 0, 1), "delete") == 0) {
        deleteApp(cut(command, '"', 1, 2), cut(command, '"', 3, 4),
                  categories, catSize, hashTable, hashSize);
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
