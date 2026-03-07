#include "../../headers/domain/company.h"

#include <stdexcept>
#include <utility>

Company::Company(std::string name, std::string address, int initialBalance)
    : name_(std::move(name)), address_(std::move(address)), balance_(initialBalance) {
    if (initialBalance < 0) {
        throw std::invalid_argument("Initial balance cannot be negative");
    }
}

std::string Company::getName() const {
    return name_;
}

std::string Company::getAddress() const {
    return address_;
}

int Company::getBalance() const {
    return balance_;
}

void Company::deposit(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Deposit amount cannot be negative");
    }
    balance_ += amount;
}

bool Company::withdraw(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Withdraw amount cannot be negative");
    }
    if (balance_ < amount) {
        return false;
    }

    balance_ -= amount;
    return true;
}
