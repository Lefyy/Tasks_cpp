#include "../../../headers/cli/commands/buy_resources_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::string toLowerCopy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

bool parseResourceType(const std::string& token, ResourceType& type) {
    const std::string normalized = toLowerCopy(token);

    if (normalized == "concrete") {
        type = ResourceType::Concrete;
        return true;
    }
    if (normalized == "steel") {
        type = ResourceType::Steel;
        return true;
    }
    if (normalized == "wood") {
        type = ResourceType::Wood;
        return true;
    }
    if (normalized == "fuel") {
        type = ResourceType::Fuel;
        return true;
    }

    return false;
}

std::string resourceBuyKey(ResourceType type) {
    if (type == ResourceType::Concrete) {
        return "concrete";
    }
    if (type == ResourceType::Steel) {
        return "steel";
    }
    if (type == ResourceType::Wood) {
        return "wood";
    }
    return "fuel";
}

}

BuyResourcesMenuCommand::BuyResourcesMenuCommand(AppContext context)
    : context_(context) {}

std::string BuyResourcesMenuCommand::key() const {
    return "3";
}

std::string BuyResourcesMenuCommand::description() const {
    return "Купить ресурсы";
}

void BuyResourcesMenuCommand::execute(const std::vector<std::string>&) {
    const std::vector<ResourceType> order = {
        ResourceType::Concrete,
        ResourceType::Steel,
        ResourceType::Wood,
        ResourceType::Fuel,
    };

    while (true) {
        printSectionHeader("Покупка ресурсов");

        const auto& prices = resourcePrices();
        const auto& stock = context_.financeService.stockResources();
        for (const ResourceType resourceType : order) {
            printSeparator();
            printKeyValueRow("Ресурс", toString(resourceType));
            printKeyValueRow("Цена за 1", std::to_string(prices.at(resourceType)));
            printKeyValueRow("На складе", std::to_string(stock.getAmountOf(resourceType)));
            std::cout << "\n";
        }

        printSeparator();
        printSectionHeader("Команды:");
        std::cout << "buy <resource> <amount>  — Купить ресурс\n"
                  << "back                    — Выход\n";
        printSeparator();

        std::string line;
        if (!std::getline(std::cin, line)) {
            return;
        }

        if (line == "b" || line == "back") {
            return;
        }

        std::istringstream stream(line);
        std::string command;
        std::string resourceToken;
        std::string amountToken;
        stream >> command >> resourceToken >> amountToken;

        if (command != "buy") {
            continue;
        }

        ResourceType resourceType = ResourceType::Concrete;
        if (!parseResourceType(resourceToken, resourceType)) {
            std::cout << "Неизвестный ресурс. Используйте: concrete, steel, wood, fuel\n";
            continue;
        }

        int amount = 0;
        if (!parseInt(amountToken, amount) || amount <= 0) {
            std::cout << "Количество должно быть целым числом больше нуля\n";
            continue;
        }

        ResourcePack pack;
        pack.add(resourceType, amount);

        if (!context_.financeService.buyResources(pack, resourcePrices())) {
            std::cout << "Недостаточно средств для покупки\n";
            continue;
        }

        std::cout << "Покупка выполнена\n";
    }
}
