#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    int userId;
    string username;
    string password;
    string email;
    string phone;

public:
    User();
    User(int userId, string username, string password, string email, string phone);
    virtual ~User();

    int getUserId() const;
    string getUsername() const;
    string getPassword() const;
    string getEmail() const;
    string getPhone() const;

    void setPassword(string password);

    virtual string getRole() const;
    virtual void displayInfo() const;
    virtual string toFileString() const;
    static User fromFileString(const string& line);
};

#endif
