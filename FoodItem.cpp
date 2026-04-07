#include "FoodItem.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

FoodItem::FoodItem() : id(0), name(""), category(""), price(0.0), available(true) {}

FoodItem::FoodItem(int id, string name, string category, double price, bool available)
    : id(id), name(name), category(category), price(price), available(available) {}

FoodItem::~FoodItem() {}

int FoodItem::getId() const { return id; }
string FoodItem::getName() const { return name; }
string FoodItem::getCategory() const { return category; }
double FoodItem::getPrice() const { return price; }
bool FoodItem::isAvailable() const { return available; }

void FoodItem::setName(string n) { name = n; }
void FoodItem::setCategory(string c) { category = c; }
void FoodItem::setPrice(double p) { price = p; }
void FoodItem::setAvailable(bool a) { available = a; }

void FoodItem::display() const {
    cout << left
         << setw(5)  << id
         << setw(25) << name
         << setw(12) << category
         << "Rs. " << fixed << setprecision(2) << price;
    if (!available) cout << "  [NOT AVAILABLE]";
    cout << endl;
}

string FoodItem::toFileString() const {
    ostringstream oss;
    oss << id << "|" << name << "|" << category << "|"
        << fixed << setprecision(2) << price << "|"
        << (available ? "1" : "0");
    return oss.str();
}

FoodItem FoodItem::fromFileString(const string& line) {
    istringstream iss(line);
    string token;
    int id;
    string name, category;
    double price;
    bool available;

    getline(iss, token, '|'); id = stoi(token);
    getline(iss, name, '|');
    getline(iss, category, '|');
    getline(iss, token, '|'); price = stod(token);
    getline(iss, token, '|'); available = (token == "1");

    return FoodItem(id, name, category, price, available);
}