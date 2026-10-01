#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <iostream>
#include "datastore.h"
#include "product.h"
#include "user.h"

class MyDataStore : public DataStore {
public:
    MyDataStore();
    ~MyDataStore();

    void addProduct(Product* p);
    void addUser(User* u);
    std::vector<Product*> search(std::vector<std::string>& terms, int type);
    void dump(std::ostream& ofile);

    // Menu commands. Each returns false if the username is invalid.
    bool addToCart(const std::string& username, Product* p);
    bool viewCart(const std::string& username);
    bool buyCart(const std::string& username);

private:
    std::set<Product*> products_;
    std::map<std::string, User*> users_;                    // key: lowercase username
    std::map<std::string, std::set<Product*> > index_;      // keyword -> products
    std::map<std::string, std::deque<Product*> > carts_;    // key: lowercase username
};
#endif