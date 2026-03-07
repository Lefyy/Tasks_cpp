#pragma once

#include <string>

enum class MachineType {
    TYPE_A,
    TYPE_B,
    TYPE_C
};

enum class status {
    NEW,
    USED
};

class Machine {
public:
    Machine(unsigned int id, MachineType type, unsigned int price);

    void setStatus(status newStatus);

    unsigned int getId() const;
    MachineType getType() const;
    unsigned int getPrice() const;
    status getStatus() const;

private:
    unsigned int id;
    MachineType type;
    unsigned int price;
    status machineStatus;
};
