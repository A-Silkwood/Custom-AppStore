#ifndef MYAPPSTOREPROJECT_STRUCTS_H
    #include <iostream>
    #define CAT_NAME_LEN 25
    #define APP_NAME_LEN 50
    #define VERSION_LEN 10
    #define UNIT_SIZE 3

    struct AppInfo {
        std::string category[CAT_NAME_LEN]; // app category
        std::string name[APP_NAME_LEN]; // app name
        std::string version[VERSION_LEN]; // version number
        float size; // app size
        std::string units[UNIT_SIZE]; // units for app size [GB/MB]
        float price; // app price
    };

    struct Tree {
        AppInfo record; // node's app info
        Tree *left; // left tree
        Tree *right; // right tree
    };

    struct Category {
        std::basic_string<char> name; // category name
        Tree *root; // binary search tree of app info
    };

    struct HashTableEntry {
        std::string appName[APP_NAME_LEN]; // app name
        Tree *appNode; // node containing app info
        HashTableEntry *next; // next hash entry in chain
    };
#define MYAPPSTOREPROJECT_STRUCTS_H

#endif //MYAPPSTOREPROJECT_STRUCTS_H
