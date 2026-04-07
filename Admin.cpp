#include "Admin.h"
#include <iostream>
using namespace std;

Admin::Admin(int userId, string username, string password, string email, string phone, string adminCode)
    : User(userId, username, password, email, phone), adminCode(adminCode) {}

Admin::~Admin() {}

string Admin::getRole() const {
    return "ADMIN";
}

void Admin::displayInfo() const {
    User::displayInfo();
    cout << "  Admin Code : " << adminCode << endl;
}

string Admin::getAdminCode() const {
    return adminCode;
}
