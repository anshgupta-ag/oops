#include "User.h"
#include <iostream>
#include <sstream>
using namespace std;

User::User() : userId(0), username(""), password(""), email(""), phone("") {}

User::User(int userId, string username, string password, string email, string phone)
    : userId(userId), username(username), password(password), email(email), phone(phone) {}

User::~User() {}

int User::getUserId() const { return userId; }
string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
string User::getEmail() const { return email; }
string User::getPhone() const { return phone; }

void User::setPassword(string p) { password = p; }

string User::getRole() const { return "USER"; }

void User::displayInfo() const {
    cout << "  User ID  : " << userId << endl;
    cout << "  Username : " << username << endl;
    cout << "  Email    : " << email << endl;
    cout << "  Phone    : " << phone << endl;
    cout << "  Role     : " << getRole() << endl;
}

string User::toFileString() const {
    return to_string(userId) + "|" + username + "|" + password + "|" + email + "|" + phone;
}

User User::fromFileString(const string& line) {
    istringstream iss(line);
    string token;
    int uid;
    string uname, pass, email, phone;

    getline(iss, token, '|'); uid = stoi(token);
    getline(iss, uname, '|');
    getline(iss, pass, '|');
    getline(iss, email, '|');
    getline(iss, phone, '|');

    return User(uid, uname, pass, email, phone);
}
