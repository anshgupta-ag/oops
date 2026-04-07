#ifndef ORDER_H
#define ORDER_H

#include "FoodItem.h"
#include <vector>
#include <string>
using namespace std;

struct OrderItem {
    FoodItem item;
    int quantity;
    OrderItem(FoodItem item, int qty) : item(item), quantity(qty) {}
};

class Order {
private:
    int orderId;
    int userId;
    string username;
    vector<OrderItem> items;
    double totalAmount;
    string orderDate;
    string status;

public:
    Order(int orderId, int userId, string username, string date);
    ~Order();

    void addItem(const FoodItem& item, int quantity);
    double calculateTotal();

    int getOrderId() const;
    int getUserId() const;
    string getUsername() const;
    double getTotalAmount() const;
    string getOrderDate() const;
    string getStatus() const;
    vector<OrderItem> getItems() const;

    void setStatus(string status);
    void displayBill() const;
    void saveToFile() const;

    static vector<Order> loadUserOrders(int userId);
    static vector<Order> loadAllOrders();
    static int getNextOrderId();
};

#endif