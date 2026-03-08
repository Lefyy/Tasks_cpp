#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../domain/project.h"
#include "../repository/machine_repository.h"
#include "../service/construction_service.h"

ResourcePack makeResources(std::initializer_list<std::pair<ResourceType, int>> items);

bool parseInt(const std::string& value, int& result);

void printProjectSummary(const Project& project);
void printMachineSummary(const Machine& machine);

int salePriceForMachine(const Machine& machine);

int countAssignedByType(const InMemoryMachineRepository& machineRepository,
                        const std::unordered_map<int, std::vector<int>>& assignments,
                        int projectId,
                        MachineType type);

bool isProjectOwned(const Project& project);

const std::unordered_map<MachineType, int>& machineCatalog();
const std::unordered_map<ResourceType, int>& resourcePrices();
