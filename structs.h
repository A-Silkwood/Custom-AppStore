#ifndef MYAPPSTOREPROJECT_STRUCTS_H
#include <iostream>

struct AppInfo {
    std::string category; // app category
    std::string name; // app name
    std::string version; // version number
    float size; // app size
    std::string units; // units for app size [GB/MB]
    float price; // app price
};

struct Tree {
    AppInfo record; // node's app info
    Tree *left; // left tree
    Tree *right; // right tree
};

struct Category {
    std::string name; // category name
    Tree *root; // binary search tree of app info
};

struct HashTableEntry {
    std::string appName; // app name
    Tree *appNode; // node containing app info
    HashTableEntry *next; // next hash entry in chain
};
#define MYAPPSTOREPROJECT_STRUCTS_H

#endif //MYAPPSTOREPROJECT_STRUCTS_H
