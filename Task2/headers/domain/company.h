#pragma once
#include <string>
#include <vector>
#include "machine.h"
#include "project.h"

class Company {
public:
    Company(const std::string& name, const std::string& address);

    void buyMachine(const MachineType& type);
    void sellMachine(const MachineType& machine, int quantity);
    void getProject();
    void dropProject(const Project& project);
    void completeProject(const Project& project);

    std::string getName() const;
    std::string getAddress() const;
    int getBalance() const;
    std::vector<Machine> getMachines() const;
    std::vector<Project> getProjects() const;

private:
    std::string name;
    std::string address;
    int balance;
    std::vector<Machine> machines;
    std::vector<Project> projects;
};