#include <iomanip>
#include <sstream>
#include "mydatastore.h"
#include "util.h"
using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for (set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it) {
        delete *it;
    }
    for (map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
        delete it->second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.insert(p);
    set<string> kws = p->keywords();
    for (set<string>::iterator it = kws.begin(); it != kws.end(); ++it) {
        index_[*it].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    string key = convToLower(u->getName());
    users_[key] = u;
    carts_[key];   // create an empty cart
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    vector<Product*> hits;
    if (terms.empty()) {
        return hits;
    }

    set<Product*> result;
    bool first = true;
    for (size_t i = 0; i < terms.size(); i++) {
        set<Product*> current;
        map<string, set<Product*> >::iterator f = index_.find(convToLower(terms[i]));
        if (f != index_.end()) {
            current = f->second;
        }
        if (first) {
            result = current;
            first = false;
        }
        else if (type == 0) {
            result = setIntersection(result, current);
        }
        else {
            result = setUnion(result, current);
        }
    }

    for (set<Product*>::iterator it = result.begin(); it != result.end(); ++it) {
        hits.push_back(*it);
    }
    return hits;
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << fixed << setprecision(2);
    ofile << "<products>" << endl;
    for (set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it) {
        (*it)->dump(ofile);
    }
    ofile << "</products>" << endl;
    ofile << "<users>" << endl;
    for (map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
        it->second->dump(ofile);
    }
    ofile << "</users>" << endl;
}

bool MyDataStore::addToCart(const string& username, Product* p)
{
    map<string, deque<Product*> >::iterator it = carts_.find(convToLower(username));
    if (it == carts_.end()) {
        return false;
    }
    it->second.push_back(p);
    return true;
}

bool MyDataStore::viewCart(const string& username)
{
    map<string, deque<Product*> >::iterator it = carts_.find(convToLower(username));
    if (it == carts_.end()) {
        return false;
    }
    int n = 1;
    for (deque<Product*>::iterator p = it->second.begin(); p != it->second.end(); ++p) {
        cout << "Item " << setw(3) << n << endl;
        cout << (*p)->displayString() << endl;
        cout << endl;
        n++;
    }
    return true;
}

bool MyDataStore::buyCart(const string& username)
{
    string key = convToLower(username);
    map<string, deque<Product*> >::iterator it = carts_.find(key);
    if (it == carts_.end()) {
        return false;
    }
    User* u = users_[key];
    deque<Product*> remaining;
    for (deque<Product*>::iterator p = it->second.begin(); p != it->second.end(); ++p) {
        if ((*p)->getQty() > 0 && u->getBalance() >= (*p)->getPrice()) {
            (*p)->subtractQty(1);
            u->deductAmount((*p)->getPrice());
        }
        else {
            remaining.push_back(*p);
        }
    }
    it->second = remaining;
    return true;
}