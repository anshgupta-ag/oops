#ifndef FOODITEM_H
#define FOODITEM_H

#include <string>
using namespace std;

class FoodItem {
private:
    int id;
    string name;
    string category;
    double price;
    bool available;

public:
    FoodItem();
    FoodItem(int id, string name, string category, double price, bool available = true);
    ~FoodItem();

    int getId() const;
    string getName() const;
    string getCategory() const;
    double getPrice() const;
    bool isAvailable() const;

    void setName(string name);
    void setCategory(string category);
    void setPrice(double price);
    void setAvailable(bool available);

    void display() const;
    string toFileString() const;
    static FoodItem fromFileString(const string& line);
};

#endif
