#include "Order.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

const string ORDERS_FILE = "orders.txt";

Order::Order(int orderId, int userId, string username, string date)
    : orderId(orderId), userId(userId), username(username),
      totalAmount(0.0), orderDate(date), status("Placed") {}

Order::~Order() {}

void Order::addItem(const FoodItem& item, int quantity) {
    items.push_back(OrderItem(item, quantity));
}

double Order::calculateTotal() {
    totalAmount = 0.0;
    for (const auto& oi : items) {
        totalAmount += oi.item.getPrice() * oi.quantity;
    }
    return totalAmount;
}

int Order::getOrderId() const { return orderId; }
int Order::getUserId() const { return userId; }
string Order::getUsername() const { return username; }
double Order::getTotalAmount() const { return totalAmount; }
string Order::getOrderDate() const { return orderDate; }
string Order::getStatus() const { return status; }
vector<OrderItem> Order::getItems() const { return items; }

void Order::setStatus(string s) { status = s; }

void Order::displayBill() const {
    cout << "\n";
    cout << "  =================================================" << endl;
    cout << "           ONLINE FOOD ORDERING SYSTEM            " << endl;
    cout << "  =================================================" << endl;
    cout << "  Order ID  : " << orderId << endl;
    cout << "  Customer  : " << username << endl;
    cout << "  Date/Time : " << orderDate << endl;
    cout << "  Status    : " << status << endl;
    cout << "  -------------------------------------------------" << endl;
    cout << "  " << left << setw(22) << "Item"
         << setw(10) << "Qty"
         << setw(10) << "Price"
         << "Subtotal" << endl;
    cout << "  -------------------------------------------------" << endl;

    for (const auto& oi : items) {
        double subtotal = oi.item.getPrice() * oi.quantity;
        cout << "  " << left << setw(22) << oi.item.getName()
             << setw(10) << oi.quantity
             << "Rs." << setw(7) << fixed << setprecision(2) << oi.item.getPrice()
             << "Rs." << fixed << setprecision(2) << subtotal << endl;
    }

    cout << "  -------------------------------------------------" << endl;
    cout << "  " << right << setw(42) << "TOTAL: Rs."
         << fixed << setprecision(2) << totalAmount << endl;
    cout << "  =================================================" << endl;
    cout << "      Thank you for ordering! Enjoy your meal!    " << endl;
    cout << "  =================================================" << endl;
}

void Order::saveToFile() const {
    ofstream file(ORDERS_FILE, ios::app);
    if (!file.is_open()) return;

    file << "ORDER|" << orderId << "|" << userId << "|" << username << "|"
         << orderDate << "|" << status << "|"
         << fixed << setprecision(2) << totalAmount << "\n";

    for (const auto& oi : items) {
        file << "ITEM|" << oi.item.getId() << "|" << oi.item.getName() << "|"
             << oi.item.getCategory() << "|"
             << fixed << setprecision(2) << oi.item.getPrice() << "|"
             << oi.quantity << "\n";
    }
    file << "END\n";
    file.close();
}

vector<Order> Order::loadUserOrders(int userId) {
    vector<Order> orders;
    ifstream file(ORDERS_FILE);
    if (!file.is_open()) return orders;

    string line;
    Order* currentOrder = nullptr;

    while (getline(file, line)) {
        if (line.substr(0, 5) == "ORDER") {
            istringstream iss(line);
            string tag, oid, uid, uname, date, status, total;
            getline(iss, tag, '|'); getline(iss, oid, '|');
            getline(iss, uid, '|'); getline(iss, uname, '|');
            getline(iss, date, '|'); getline(iss, status, '|');
            getline(iss, total, '|');

            if (stoi(uid) == userId) {
                orders.push_back(Order(stoi(oid), stoi(uid), uname, date));
                orders.back().setStatus(status);
                orders.back().totalAmount = stod(total);
                currentOrder = &orders.back();
            } else {
                currentOrder = nullptr;
            }
        } else if (line.substr(0, 4) == "ITEM" && currentOrder != nullptr) {
            istringstream iss(line);
            string tag, iid, iname, cat, price, qty;
            getline(iss, tag, '|'); getline(iss, iid, '|');
            getline(iss, iname, '|'); getline(iss, cat, '|');
            getline(iss, price, '|'); getline(iss, qty, '|');
            FoodItem fi(stoi(iid), iname, cat, stod(price));
            currentOrder->addItem(fi, stoi(qty));
        } else if (line == "END") {
            currentOrder = nullptr;
        }
    }
    file.close();
    return orders;
}

vector<Order> Order::loadAllOrders() {
    vector<Order> orders;
    ifstream file(ORDERS_FILE);
    if (!file.is_open()) return orders;

    string line;
    Order* currentOrder = nullptr;

    while (getline(file, line)) {
        if (line.substr(0, 5) == "ORDER") {
            istringstream iss(line);
            string tag, oid, uid, uname, date, status, total;
            getline(iss, tag, '|'); getline(iss, oid, '|');
            getline(iss, uid, '|'); getline(iss, uname, '|');
            getline(iss, date, '|'); getline(iss, status, '|');
            getline(iss, total, '|');
            orders.push_back(Order(stoi(oid), stoi(uid), uname, date));
            orders.back().setStatus(status);
            orders.back().totalAmount = stod(total);
            currentOrder = &orders.back();
        } else if (line.substr(0, 4) == "ITEM" && currentOrder != nullptr) {
            istringstream iss(line);
            string tag, iid, iname, cat, price, qty;
            getline(iss, tag, '|'); getline(iss, iid, '|');
            getline(iss, iname, '|'); getline(iss, cat, '|');
            getline(iss, price, '|'); getline(iss, qty, '|');
            FoodItem fi(stoi(iid), iname, cat, stod(price));
            currentOrder->addItem(fi, stoi(qty));
        } else if (line == "END") {
            currentOrder = nullptr;
        }
    }
    file.close();
    return orders;
}

int Order::getNextOrderId() {
    ifstream file(ORDERS_FILE);
    if (!file.is_open()) return 1001;
    int count = 1001;
    string line;
    while (getline(file, line)) {
        if (line.substr(0, 5) == "ORDER") count++;
    }
    file.close();
    return count;
}
