#include "FoodSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <ctime>
using namespace std;

FoodSystem::FoodSystem() : currentUser(nullptr), isAdminLoggedIn(false) {
    loadUsers();
    loadMenu();
    if (menu.empty()) {
        menu.push_back(FoodItem(1,  "Paneer Butter Masala",  "Veg",     180.00));
        menu.push_back(FoodItem(2,  "Dal Tadka",             "Veg",     120.00));
        menu.push_back(FoodItem(3,  "Veg Biryani",           "Veg",     160.00));
        menu.push_back(FoodItem(4,  "Aloo Paratha",          "Veg",     80.00));
        menu.push_back(FoodItem(5,  "Chicken Biryani",       "Non-Veg", 220.00));
        menu.push_back(FoodItem(6,  "Butter Chicken",        "Non-Veg", 240.00));
        menu.push_back(FoodItem(7,  "Mutton Rogan Josh",     "Non-Veg", 280.00));
        menu.push_back(FoodItem(8,  "Egg Fried Rice",        "Non-Veg", 140.00));
        menu.push_back(FoodItem(9,  "Mango Lassi",           "Drinks",  60.00));
        menu.push_back(FoodItem(10, "Masala Chai",           "Drinks",  30.00));
        menu.push_back(FoodItem(11, "Fresh Lime Soda",       "Drinks",  40.00));
        menu.push_back(FoodItem(12, "Cold Coffee",           "Drinks",  70.00));
        saveMenu();
    }
}

FoodSystem::~FoodSystem() {
    saveUsers();
    saveMenu();
    delete currentUser;
}

void FoodSystem::loadUsers() {
    ifstream file(USERS_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) users.push_back(User::fromFileString(line));
    }
    file.close();
}

void FoodSystem::saveUsers() {
    ofstream file(USERS_FILE);
    if (!file.is_open()) return;
    for (const auto& u : users) file << u.toFileString() << "\n";
    file.close();
}

void FoodSystem::loadMenu() {
    ifstream file(MENU_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) menu.push_back(FoodItem::fromFileString(line));
    }
    file.close();
}

void FoodSystem::saveMenu() {
    ofstream file(MENU_FILE);
    if (!file.is_open()) return;
    for (const auto& f : menu) file << f.toFileString() << "\n";
    file.close();
}

void FoodSystem::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void FoodSystem::pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string FoodSystem::getCurrentDateTime() {
    time_t now = time(0);
    char buf[80];
    struct tm tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return string(buf);
}

int FoodSystem::getNextUserId() {
    if (users.empty()) return 1001;
    int maxId = 0;
    for (const auto& u : users)
        if (u.getUserId() > maxId) maxId = u.getUserId();
    return maxId + 1;
}

int FoodSystem::getNextFoodId() {
    if (menu.empty()) return 1;
    int maxId = 0;
    for (const auto& f : menu)
        if (f.getId() > maxId) maxId = f.getId();
    return maxId + 1;
}

bool FoodSystem::usernameExists(const string& username) {
    for (const auto& u : users)
        if (u.getUsername() == username) return true;
    return false;
}

void FoodSystem::printHeader(const string& title) {
    cout << "\n  =================================================" << endl;
    cout << "   " << title << endl;
    cout << "  =================================================" << endl;
}

void FoodSystem::printMenuItems(const string& filter) {
    cout << "\n  " << left << setw(5) << "ID"
         << setw(25) << "Name"
         << setw(12) << "Category"
         << "Price" << endl;
    cout << "  -------------------------------------------------" << endl;
    bool found = false;
    for (const auto& f : menu) {
        if ((filter == "ALL" || f.getCategory() == filter) && f.isAvailable()) {
            f.display();
            found = true;
        }
    }
    if (!found) cout << "  (No items available)" << endl;
    cout << "  -------------------------------------------------" << endl;
}

void FoodSystem::registerUser() {
    printHeader("REGISTER NEW ACCOUNT");
    string username, password, email, phone;
    cout << "\n  Enter Username : "; cin >> username;
    if (usernameExists(username)) {
        cout << "\n  [ERROR] Username already taken.\n";
        pause(); return;
    }
    cout << "  Enter Password : "; cin >> password;
    cout << "  Enter Email    : "; cin >> email;
    cout << "  Enter Phone    : "; cin >> phone;
    int newId = getNextUserId();
    users.push_back(User(newId, username, password, email, phone));
    saveUsers();
    cout << "\n  [SUCCESS] Account created! Your User ID: " << newId << endl;
    pause();
}

bool FoodSystem::loginUser() {
    printHeader("USER LOGIN");
    string username, password;

    // Clear the buffer once before starting to ensure no leftover 'Enter' keys
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "\n  Enter Username : ";
    cin >> username; 
    
    cout << "  Enter Password : ";
    cin >> password;

    for (const auto& u : users) {
        if (u.getUsername() == username && u.getPassword() == password) {
            currentUser = new User(u);
            cout << "\n  [SUCCESS] Welcome, " << username << "!\n";
            pause(); 
            return true;
        }
    }
    cout << "\n  [ERROR] Invalid credentials.\n";
    pause(); 
    return false;
}

void FoodSystem::logoutUser() {
    cout << "\n  Goodbye, " << currentUser->getUsername() << "!\n";
    delete currentUser;
    currentUser = nullptr;
    pause();
}

bool FoodSystem::loginAdmin() {
    printHeader("ADMIN LOGIN");
    string username, password;

    // 1. CLEAR THE BUFFER: This is the most important line
    cin.ignore(1000, '\n'); 

    cout << "\n Enter Admin Username : ";
    getline(cin, username);
    
    cout << " Enter Admin Password : ";
    getline(cin, password);

    // 2. CHECK CREDENTIALS: Using your constants from the header
    // REMOVE THE QUOTES from the variable names!
   
    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        isAdminLoggedIn = true;
        cout << "\n [SUCCESS] Admin access granted!\n";
        pause(); 
        return true;
    } 
    else {
        cout << "\n [ERROR] Invalid admin credentials.\n";
        pause(); 
        return false;
    }
}

void FoodSystem::logoutAdmin() {
    isAdminLoggedIn = false;
    cout << "\n  Admin logged out.\n";
    pause();
}

void FoodSystem::viewMenu() {
    printHeader("FOOD MENU");
    int choice;
    cout << "\n  1. All Items\n  2. Veg\n  3. Non-Veg\n  4. Drinks\n\n  Enter choice: ";
    cin >> choice;
    switch (choice) {
        case 1: printMenuItems("ALL");     break;
        case 2: printMenuItems("Veg");     break;
        case 3: printMenuItems("Non-Veg"); break;
        case 4: printMenuItems("Drinks");  break;
        default: cout << "  Invalid choice.\n";
    }
    pause();
}

void FoodSystem::placeOrder() {
    printHeader("PLACE AN ORDER");
    printMenuItems("ALL");
    int orderId = Order::getNextOrderId();
    Order order(orderId, currentUser->getUserId(), currentUser->getUsername(), getCurrentDateTime());
    cout << "\n  Enter item IDs (0 to finish):\n";
    while (true) {
        int itemId, qty;
        cout << "\n  Item ID (0 to finish): "; if (!(cin >> itemId)) {
    cin.clear(); // clear error
    cin.ignore(1000, '\n'); // discard bad input
    cout << "  [ERROR] Invalid input. Try again.\n";
    continue;
}
        if (itemId == 0) break;
        bool found = false;
        for (const auto& f : menu) {
            if (f.getId() == itemId && f.isAvailable()) {
                cout << "  Quantity: ";if (!(cin >> qty)) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "  [ERROR] Invalid quantity.\n";
    continue;
}

                if (qty > 0) { order.addItem(f, qty); cout << "  Added!\n"; found = true; }
                break;
            }
        }
        if (!found) cout << "  [ERROR] Item not found.\n";
    }
    if (order.getItems().empty()) {
        cout << "\n  No items added. Cancelled.\n"; pause(); return;
    }
    order.calculateTotal();
    order.displayBill();
    char confirm;
    cout << "\n  Confirm order? (y/n): "; cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        order.saveToFile();
        cout << "\n  [SUCCESS] Order placed! ID: " << orderId << endl;
    } else {
        cout << "\n  Order cancelled.\n";
    }
    pause();
}

void FoodSystem::viewMyOrders() {
    printHeader("MY ORDERS");
    vector<Order> orders = Order::loadUserOrders(currentUser->getUserId());
    if (orders.empty()) { cout << "\n  No orders found.\n"; pause(); return; }
    for (auto& o : orders) { o.displayBill(); cout << "\n"; }
    pause();
}

void FoodSystem::viewMyProfile() {
    printHeader("MY PROFILE");
    cout << "\n"; currentUser->displayInfo();
    pause();
}

void FoodSystem::userMenu() {
    int choice;
    while (true) {
        clearScreen();
        printHeader("USER PANEL — Welcome, " + currentUser->getUsername());
        cout << "\n  1. View Food Menu\n  2. Place Order\n  3. View My Orders\n  4. My Profile\n  0. Logout\n\n  Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: clearScreen(); viewMenu();      break;
            case 2: clearScreen(); placeOrder();    break;
            case 3: clearScreen(); viewMyOrders();  break;
            case 4: clearScreen(); viewMyProfile(); break;
            case 0: logoutUser(); return;
            default: cout << "\n  Invalid.\n"; pause();
        }
    }
}

void FoodSystem::addFoodItem() {
    printHeader("ADD FOOD ITEM");
    string name, category; double price;
    cout << "\n  Enter Item Name     : "; cin.ignore(); getline(cin, name);
    cout << "  Enter Category (Veg / Non-Veg / Drinks): "; getline(cin, category);
    cout << "  Enter Price (Rs.)   : "; cin >> price;
    int newId = getNextFoodId();
    menu.push_back(FoodItem(newId, name, category, price));
    saveMenu();
    cout << "\n  [SUCCESS] Added '" << name << "' with ID: " << newId << endl;
    pause();
}

void FoodSystem::removeFoodItem() {
    printHeader("REMOVE FOOD ITEM");
    viewAllFoodItems();
    int itemId;
    cout << "\n  Enter ID to remove: "; cin >> itemId;
    for (auto it = menu.begin(); it != menu.end(); ++it) {
        if (it->getId() == itemId) {
            cout << "\n  Removing: " << it->getName() << endl;
            menu.erase(it); saveMenu();
            cout << "  [SUCCESS] Removed.\n"; pause(); return;
        }
    }
    cout << "\n  [ERROR] Not found.\n"; pause();
}

void FoodSystem::viewAllFoodItems() {
    printHeader("ALL FOOD ITEMS");
    cout << "\n  " << left << setw(5) << "ID" << setw(25) << "Name"
         << setw(12) << "Category" << setw(10) << "Price" << "Status" << endl;
    cout << "  -------------------------------------------------" << endl;
    for (const auto& f : menu) {
        cout << "  " << left << setw(5) << f.getId() << setw(25) << f.getName()
             << setw(12) << f.getCategory()
             << "Rs." << setw(7) << fixed << setprecision(2) << f.getPrice()
             << (f.isAvailable() ? "Available" : "NOT AVAILABLE") << endl;
    }
    cout << "  -------------------------------------------------" << endl;
}

void FoodSystem::toggleFoodAvailability() {
    printHeader("TOGGLE AVAILABILITY");
    viewAllFoodItems();
    int itemId;
    cout << "\n  Enter Item ID: "; cin >> itemId;
    for (auto& f : menu) {
        if (f.getId() == itemId) {
            f.setAvailable(!f.isAvailable()); saveMenu();
            cout << "\n  [SUCCESS] '" << f.getName() << "' is now "
                 << (f.isAvailable() ? "AVAILABLE" : "NOT AVAILABLE") << ".\n";
            pause(); return;
        }
    }
    cout << "\n  [ERROR] Not found.\n"; pause();
}

void FoodSystem::viewAllOrders() {
    printHeader("ALL ORDERS");
    vector<Order> orders = Order::loadAllOrders();
    if (orders.empty()) { cout << "\n  No orders found.\n"; pause(); return; }
    for (auto& o : orders) { o.displayBill(); cout << "\n"; }
    pause();
}

void FoodSystem::adminMenu() {
    int choice;
    while (true) {
        clearScreen();
        printHeader("ADMIN PANEL");
        cout << "\n  1. View All Food Items\n  2. Add Food Item\n  3. Remove Food Item\n  4. Toggle Availability\n  5. View All Orders\n  0. Logout\n\n  Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: clearScreen(); viewAllFoodItems(); pause();          break;
            case 2: clearScreen(); addFoodItem();                        break;
            case 3: clearScreen(); removeFoodItem();                     break;
            case 4: clearScreen(); toggleFoodAvailability();             break;
            case 5: clearScreen(); viewAllOrders();                      break;
            case 0: logoutAdmin(); return;
            default: cout << "\n  Invalid.\n"; pause();
        }
    }
}

void FoodSystem::run() {
    int choice;
    while (true) {
        clearScreen();
        cout << "\n  =================================================" << endl;
        cout << "        WELCOME TO ONLINE FOOD ORDERING           " << endl;
        cout << "  =================================================" << endl;
        cout << "\n  1. Register\n  2. User Login\n  3. Admin Login\n  0. Exit\n\n  Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: clearScreen(); registerUser(); break;
            case 2: clearScreen(); if (loginUser()) userMenu(); break;
            case 3: clearScreen(); if (loginAdmin()) adminMenu(); break;
            case 0: cout << "\n  Goodbye!\n\n"; return;
            default: cout << "\n  Invalid choice.\n"; pause();
        }
    }
}
