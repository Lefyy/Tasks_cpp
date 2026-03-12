#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility> 

#include "../domain/project.h"
#include "../repository/equipment_repository.h"

ResourcePack makeResources(std::initializer_list<std::pair<ResourceType, int>> items);

bool parseInt(const std::string& value, int& result);

void printSectionHeader(const std::string& title);
void printSeparator();
void printKeyValueRow(const std::string& key, const std::string& value);

void printProjectSummary(const Project& project);
void printEquipmentSummary(const Equipment& equipment);
int salePriceForEquipment(const Equipment& equipment);

bool isProjectOwned(const Project& project);

int countAssignedByType(const InMemoryEquipmentRepository& equipmentRepository,
                        const std::unordered_map<int, std::vector<int>>& assignments,
                        int projectId,
                        const EquipmentType& type);

const std::unordered_map<MachineType, int>& machineCatalog();
const std::unordered_map<ToolType, int>& toolCatalog();
const std::unordered_map<ResourceType, int>& resourcePrices();
