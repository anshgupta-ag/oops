#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
private:
    string adminCode;

public:
    Admin(int userId, string username, string password, string email, string phone, string adminCode);
    ~Admin();

    string getRole() const override;
    void displayInfo() const override;
    string getAdminCode() const;
};

#endif