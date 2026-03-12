#include "../../../headers/cli/commands/buy_tools_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>

BuyToolsMenuCommand::BuyToolsMenuCommand(AppContext context)
    : context_(context) {}

std::string BuyToolsMenuCommand::key() const {
    return "3";
}

std::string BuyToolsMenuCommand::description() const {
    return "Купить инструменты";
}

void BuyToolsMenuCommand::execute(const std::vector<std::string>&) {
    std::string statusLine = "Введите id инструмента, который хотите купить, или back для выхода.";

    const std::vector<ToolType> order = {
        ToolType::Jackhammer,
        ToolType::Generator,
        ToolType::LaserLevel,
    };

    while (true) {
        printSectionHeader("Покупка инструментов");
        for (std::size_t i = 0; i < order.size(); ++i) {
            const ToolType type = order[i];
            const int newPrice = toolCatalog().at(type);
            printSeparator();
            printKeyValueRow("ID", std::to_string(i + 1));
            printKeyValueRow("Тип", toString(type));
            printKeyValueRow("Состояние", "Новый");
            printKeyValueRow("Цена покупки", std::to_string(newPrice));
            printKeyValueRow("Цена продажи", std::to_string(newPrice / 2));
            std::cout << "\n";
        }

        printSeparator();
        printSectionHeader("Команды:");
        std::cout << "<id>  — Купить выбранный инструмент\n"
                  << "back  — Выход\n";
        printSeparator();
        std::cout << statusLine << '\n';

        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b" || input == "back") {
            return;
        }

        int selected = 0;
        if (!parseInt(input, selected) || selected <= 0 || selected > static_cast<int>(order.size())) {
            continue;
        }

        const ToolType type = order[static_cast<std::size_t>(selected - 1)];
        const int price = toolCatalog().at(type);
        const bool purchased = context_.financeService.buyEquipment(type, price, EquipmentCondition::New);

        if (!purchased) {
            statusLine = "У вас недостаточно средств для покупки этого инструмента. "
                         "Введите id инструмента, который хотите купить, или back для выхода.";
            continue;
        }

        statusLine = "Инструмент успешно куплен. Введите id инструмента, который хотите купить, или back для выхода.";
    }
}

