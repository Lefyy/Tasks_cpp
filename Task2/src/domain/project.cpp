#include "../../headers/domain/project.h"

#include <stdexcept>
#include <utility>

namespace {

void ensureNonNegativeAmount(int amount, const char* message) {
    if (amount < 0) {
        throw std::invalid_argument(message);
    }
}

void ensureCanActivate(ProjectState state) {
    if (state != ProjectState::Draft && state != ProjectState::Paused) {
        throw std::logic_error("Project can only be activated from Draft or Paused state");
    }
}

void ensureCanPause(ProjectState state) {
    if (state != ProjectState::Active) {
        throw std::logic_error("Project can only be paused from Active state");
    }
}

void ensureCanCancel(ProjectState state) {
    if (state == ProjectState::Completed || state == ProjectState::Cancelled) {
        throw std::logic_error("Project cannot be cancelled in its current state");
    }
}

void ensureCanAdvanceWeek(ProjectState state) {
    if (state != ProjectState::Active) {
        throw std::logic_error("Only active projects can advance by week");
    }
}

}

int ResourcePack::getAmountOf(ResourceType type) const {
    auto it = values.find(type);
    if (it == values.end()) {
        return 0;
    }
    return it->second;
}

void ResourcePack::add(ResourceType type, int amount) {
    ensureNonNegativeAmount(amount, "Resource amount cannot be negative");
    values[type] += amount;
}

bool ResourcePack::consume(ResourceType type, int amount) {
    ensureNonNegativeAmount(amount, "Resource amount cannot be negative");

    if (getAmountOf(type) < amount) {
        return false;
    }

    values[type] -= amount;
    return true;
}

bool ResourcePack::hasAtLeast(const ResourcePack& other) const {
    for (const auto& [type, amount] : other.values) {
        if (getAmountOf(type) < amount) {
            return false;
        }
    }
    return true;
}

ProjectPhase::ProjectPhase(std::string name, PhaseRequirements requirements)
    : name_(std::move(name)), requirements_(std::move(requirements)) {}

const std::string& ProjectPhase::getName() const noexcept {
    return name_;
}

const PhaseRequirements& ProjectPhase::getRequirements() const noexcept {
    return requirements_;
}

Project::Project(int id, std::string name, int budget, std::vector<ProjectPhase> phases)
    : id_(id),
      name_(std::move(name)),
      contractValue_(budget),
      phases_(std::move(phases)),
      state_(ProjectState::Draft),
      currentPhaseIndex_(0),
      elapsedWeeksInCurrentPhase_(0) {
    if (budget < 0) {
        throw std::invalid_argument("Project budget cannot be negative");
    }
    if (phases_.empty()) {
        throw std::invalid_argument("Project must contain at least one phase");
    }
}

int Project::getId() const noexcept {
    return id_;
}

const std::string& Project::getName() const noexcept {
    return name_;
}

int Project::getBudget() const noexcept {
    return contractValue_;
}

ProjectState Project::getState() const noexcept {
    return state_;
}

std::size_t Project::getCurrentPhaseIndex() const noexcept {
    return currentPhaseIndex_;
}

int Project::getElapsedWeeksInCurrentPhase() const noexcept {
    return elapsedWeeksInCurrentPhase_;
}

bool Project::isFinished() const noexcept {
    return state_ == ProjectState::Completed || currentPhaseIndex_ >= phases_.size();
}

const ProjectPhase& Project::currentPhase() const {
    if (currentPhaseIndex_ >= phases_.size()) {
        throw std::out_of_range("No current phase: project is already finished");
    }
    return phases_[currentPhaseIndex_];
}

const std::vector<ProjectPhase>& Project::phases() const noexcept {
    return phases_;
}

void Project::activate() {
    ensureCanActivate(state_);
    state_ = ProjectState::Active;
}

void Project::pause() {
    ensureCanPause(state_);
    state_ = ProjectState::Paused;
}

void Project::cancel() {
    ensureCanCancel(state_);
    state_ = ProjectState::Cancelled;
}

void Project::advanceWeek() {
    ensureCanAdvanceWeek(state_);

    if (isFinished()) {
        return;
    }

    ++elapsedWeeksInCurrentPhase_;

    const auto& req = phases_[currentPhaseIndex_].getRequirements();
    if (elapsedWeeksInCurrentPhase_ >= req.durationWeeks) {
        ++currentPhaseIndex_;
        elapsedWeeksInCurrentPhase_ = 0;

        if (currentPhaseIndex_ >= phases_.size()) {
            state_ = ProjectState::Completed;
        }
    }
}

std::string toString(ResourceType type) {
    switch (type) {
        case ResourceType::Concrete:
            return "Concrete";
        case ResourceType::Steel:
            return "Steel";
        case ResourceType::Wood:
            return "Wood";
        case ResourceType::Fuel:
            return "Fuel";
    }

    return "Unknown Resource";
}

std::string toString(ProjectState state) {
    switch (state) {
        case ProjectState::Draft:
            return "Draft";
        case ProjectState::Active:
            return "Active";
        case ProjectState::Paused:
            return "Paused";
        case ProjectState::Completed:
            return "Completed";
        case ProjectState::Cancelled:
            return "Cancelled";
    }

    return "Unknown State";
}
