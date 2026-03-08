#include "../../headers/service/finance_service.h"

FinanceService::FinanceService(Company& company,
                               MachineRepository& machineRepository,
                               ProjectRepository& projectRepository)
    : company_(company),
      machineRepository_(machineRepository),
      projectRepository_(projectRepository) {}

bool FinanceService::buyMachine(MachineType type,
                                int price,
                                MachineCondition condition) {
    if (price < 0) {
        return false;
    }

    if (!company_.withdraw(price)) {
        return false;
    }

    Machine machine(nextMachineId_++, type, price);
    machine.setCondition(condition);
    machineRepository_.add(machine);
    return true;
}

bool FinanceService::sellMachine(int machineId, int salePrice) {
    if (salePrice < 0) {
        return false;
    }

    const auto machine = machineRepository_.findById(machineId);
    if (!machine.has_value()) {
        return false;
    }

    if (!machineRepository_.remove(machineId)) {
        return false;
    }

    company_.deposit(salePrice);
    return true;
}

bool FinanceService::buyResources(const ResourcePack& resources,
                                  const std::unordered_map<ResourceType, int>& pricesPerUnit) {
    int totalCost = 0;

    for (const auto& [resourceType, amount] : resources.values) {
        if (amount < 0) {
            return false;
        }

        const auto it = pricesPerUnit.find(resourceType);
        if (it == pricesPerUnit.end() || it->second < 0) {
            return false;
        }

        totalCost += amount * it->second;
    }

    if (!company_.withdraw(totalCost)) {
        return false;
    }

    for (const auto& [resourceType, amount] : resources.values) {
        stockResources_.add(resourceType, amount);
    }

    return true;
}

bool FinanceService::sellResources(const ResourcePack& resources,
                                   const std::unordered_map<ResourceType, int>& pricesPerUnit) {
    int totalRevenue = 0;

    for (const auto& [resourceType, amount] : resources.values) {
        if (amount < 0) {
            return false;
        }

        const auto it = pricesPerUnit.find(resourceType);
        if (it == pricesPerUnit.end() || it->second < 0) {
            return false;
        }

        if (stockResources_.getAmountOf(resourceType) < amount) {
            return false;
        }

        totalRevenue += amount * it->second;
    }

    for (const auto& [resourceType, amount] : resources.values) {
        stockResources_.consume(resourceType, amount);
    }

    company_.deposit(totalRevenue);
    return true;
}

ResourcePack& FinanceService::stockResources() noexcept {
    return stockResources_;
}

const ResourcePack& FinanceService::stockResources() const noexcept {
    return stockResources_;
}

bool FinanceService::takeProject(int projectId) {
    const auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return false;
    }

    company_.deposit(project->getBudget());
    return true;
}

bool FinanceService::dropProject(int projectId) {
    const auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return false;
    }

    company_.withdraw(project->getBudget());
    return true;
}
