#ifndef FOODSYSTEM_H
#define FOODSYSTEM_H

#include "User.h"
#include "Admin.h"
#include "FoodItem.h"
#include "Order.h"
#include <vector>
#include <string>
using namespace std;

class FoodSystem {
private:
    vector<User> users;
    vector<FoodItem> menu;
    User* currentUser;
    bool isAdminLoggedIn;

    const string ADMIN_USERNAME = "admin";
    const string ADMIN_PASSWORD = "admin123";
    const string ADMIN_CODE     = "FOOD@ADMIN";
    const string USERS_FILE = "users.txt";
    const string MENU_FILE  = "menu.txt";

    void loadUsers();
    void saveUsers();
    void loadMenu();
    void saveMenu();
    void clearScreen();
    void pause();
    string getCurrentDateTime();
    int getNextUserId();
    int getNextFoodId();
    bool usernameExists(const string& username);
    void printHeader(const string& title);
    void printMenuItems(const string& filter = "ALL");

    void registerUser();
    bool loginUser();
    void logoutUser();
    bool loginAdmin();
    void logoutAdmin();

    void userMenu();
    void viewMenu();
    void placeOrder();
    void viewMyOrders();
    void viewMyProfile();

    void adminMenu();
    void addFoodItem();
    void removeFoodItem();
    void viewAllFoodItems();
    void viewAllOrders();
    void toggleFoodAvailability();

public:
    FoodSystem();
    ~FoodSystem();
    void run();
};

#endif