#include <sstream>
#include <iomanip>
#include "clothing.h"
#include "util.h"
using namespace std;

Clothing::Clothing(const string name, double price, int qty,
                   const string size, const string brand)
    : Product("clothing", name, price, qty), size_(size), brand_(brand)
{
}

Clothing::~Clothing()
{
}

set<string> Clothing::keywords() const
{
    set<string> nameWords = parseStringToWords(name_);
    set<string> brandWords = parseStringToWords(brand_);
    return setUnion(nameWords, brandWords);
}

string Clothing::displayString() const
{
    stringstream ss;
    ss << name_ << "\n"
       << "Size: " << size_ << " Brand: " << brand_ << "\n"
       << fixed << setprecision(2) << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Clothing::dump(ostream& os) const
{
    Product::dump(os);
    os << size_ << "\n" << brand_ << endl;
}