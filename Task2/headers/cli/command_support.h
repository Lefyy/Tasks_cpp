#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../domain/project.h"
#include "../service/construction_service.h"

ResourcePack makeResources(std::initializer_list<std::pair<ResourceType, int>> items);

bool parseInt(const std::string& value, int& result);

void printSectionHeader(const std::string& title);
void printSeparator();
void printKeyValueRow(const std::string& key, const std::string& value);

void printProjectSummary(const Project& project);

bool isProjectOwned(const Project& project);

const std::unordered_map<ResourceType, int>& resourcePrices();
