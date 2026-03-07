#pragma once

#include <string>
#include <vector>
#include "machine.h"

enum class Phase {
    NOT_STARTED,
    IN_PROGRESS,
    COMPLETED
};

struct PhaseRequirements {
    std::vector<MachineType> machineTypes;
    float resourceMultiplier;
};

inline const PhaseRequirements PHASE_REQS[]= {
    PhaseRequirements{{MachineType::TYPE_A, MachineType::TYPE_B}, 0.3f},
    PhaseRequirements{{MachineType::TYPE_B}, 0.4f},
    PhaseRequirements{{MachineType::TYPE_C}, 0.3f}
};

class Project {
public:
    Project(unsigned int id, const std::string& name, unsigned int budget, unsigned int requiredResources);

    void nextWeek();
    void allocateResources(unsigned int amount);

    unsigned int getId() const;
    std::string getName() const;
    unsigned int getBudget() const;
    Phase getCurrentPhase() const;

private:
    unsigned int id;
    std::string name;
    unsigned int budget;
    unsigned int requiredResources;
    unsigned int allocatedResources;
    Phase currentPhase;

    void nextPhase();
};
