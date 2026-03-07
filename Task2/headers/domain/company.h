#pragma once
#include <string>
#include <vector>

class Company {
public:
    Company(std::string name, std::string address, int initialBalance);

    std::string getName() const;
    std::string getAddress() const;
    int getBalance() const;

    void deposit(int amount);
    bool withdraw(int amount);

private:
    std::string name_;
    std::string address_;
    int balance_;
};