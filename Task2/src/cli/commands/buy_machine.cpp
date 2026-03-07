#include "../../../headers/cli/commands/buy_machine.h"

#include <algorithm>
#include <optional>
#include <string>

namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::optional<MachineType> parseMachineType(const std::string& token) {
    const std::string normalized = toLower(token);
    if (normalized == "excavator") return MachineType::Excavator;
    if (normalized == "bulldozer") return MachineType::Bulldozer;
    if (normalized == "crane") return MachineType::Crane;
    if (normalized == "truck") return MachineType::Truck;
    if (normalized == "concretemixer" || normalized == "concrete_mixer") return MachineType::ConcreteMixer;
    return std::nullopt;
}

std::optional<MachineCondition> parseCondition(const std::string& token) {
    const std::string normalized = toLower(token);
    if (normalized == "new") return MachineCondition::New;
    if (normalized == "used") return MachineCondition::Used;
    return std::nullopt;
}

struct BuyMachineRequest {
    MachineType type;
    int price{};
    MachineCondition condition{MachineCondition::New};
};

std::optional<BuyMachineRequest> parseRequest(const std::vector<std::string>& args) {
    if (args.size() != 2 && args.size() != 3) {
        return std::nullopt;
    }

    const auto type = parseMachineType(args[0]);
    if (!type) {
        return std::nullopt;
    }

    int price{};
    try {
        price = std::stoi(args[1]);
    } catch (...) {
        return std::nullopt;
    }

    if (price <= 0) {
        return std::nullopt;
    }

    MachineCondition condition = MachineCondition::New;
    if (args.size() == 3) {
        const auto parsedCondition = parseCondition(args[2]);
        if (!parsedCondition) {
            return std::nullopt;
        }
        condition = *parsedCondition;
    }

    return BuyMachineRequest{*type, price, condition};
}

void printUsage(std::ostream& output) {
    output << "Usage: buy_machine <type> <price> [new|used]\n";
}

void printResult(std::ostream& output, bool success) {
    if (success) {
        output << "Machine purchase completed successfully.\n";
        return;
    }

    output << "Machine purchase failed: not enough funds or invalid operation.\n";
}

}  // namespace

BuyMachineCommand::BuyMachineCommand(FinanceService& financeService, std::ostream& output)
    : financeService_(financeService), output_(output) {
}

std::string BuyMachineCommand::key() const {
    return "buy_machine";
}

std::string BuyMachineCommand::description() const {
    return "Buy a machine: <type> <price> [condition]";
}

void BuyMachineCommand::execute(const std::vector<std::string>& args) {
    const auto request = parseRequest(args);
    if (!request) {
        printUsage(output_);
        return;
    }

    const bool success = financeService_.buyMachine(request->type, request->price, request->condition);
    printResult(output_, success);
}
