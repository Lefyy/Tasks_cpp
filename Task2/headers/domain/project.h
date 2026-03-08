#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "machine.h"

enum class ResourceType {
    Concrete,
    Steel,
    Wood,
    Fuel
};

struct ResourcePack {
    std::unordered_map<ResourceType, int> values;

    int getAmountOf(ResourceType type) const;
    void add(ResourceType type, int amount);
    bool consume(ResourceType type, int amount);
    bool hasAtLeast(const ResourcePack& other) const;
};

enum class ProjectState {
    Draft,
    Active,
    Paused,
    Completed,
    Cancelled
};

struct PhaseRequirements {
    std::unordered_map<MachineType, int> machines;
    ResourcePack resourcesPerWeek;
    int durationWeeks{};
};

struct MachineRequirements {
    MachineType machineType;
    int quantity;
};

class ProjectPhase {
public:
    ProjectPhase(std::string name, PhaseRequirements requirements);

    const std::string& getName() const noexcept;
    const PhaseRequirements& getRequirements() const noexcept;

private:
    std::string name_;
    PhaseRequirements requirements_;
};


class Project {
public:
    Project(int id, std::string name, int budget, std::vector<ProjectPhase> phases);

    int getId() const noexcept;
    const std::string& getName() const noexcept;
    int getBudget() const noexcept;
    ProjectState getState() const noexcept;
    std::size_t getCurrentPhaseIndex() const noexcept;
    int getElapsedWeeksInCurrentPhase() const noexcept;
    bool isFinished() const noexcept;


    const ProjectPhase& getCurrentPhase() const;
    const std::vector<ProjectPhase>& getPhases() const noexcept;

    void activate();
    void pause();
    void cancel();
    void advanceWeek();

private:
    int id_{};
    std::string name_;
    int contractValue_{};
    std::vector<ProjectPhase> phases_;
    ProjectState state_{ProjectState::Draft};
    std::size_t currentPhaseIndex_{};
    int elapsedWeeksInCurrentPhase_{};

};

std::string toString(ResourceType type);
std::string toString(ProjectState state);

