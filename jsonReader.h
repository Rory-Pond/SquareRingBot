
using CardFactoryFunc = std::function<std::unique_ptr<BaseCard>(const nlohmann::json&)>;

class CardFactory {
public:
    static CardFactory& instance() {
        static CardFactory inst;
        return inst;
    }

    void register_type(const std::string& type, CardFactoryFunc func) {
        registry[type] = std::move(func);
    }

    std::unique_ptr<BaseCard> create(const nlohmann::json& j) {
        auto it = registry.find(j.value("type", ""));
        if (it != registry.end()) {
            return it->second(j);
        }
        throw std::runtime_error("Unknown card type: " + j.value("type", "unknown"));
    }

private:
    std::unordered_map<std::string, CardFactoryFunc> registry;
};


void register_card_types() {
    CardFactory::instance().register_type("AttackCard", [](const json& j) {
        std::string name = j.value("name", "Unnamed_Card");
        auto dir = parse_direction(j.value("direction", "Attack_Forwards"));
        int power = j.value("power", 1);
        std::vector<std::string> moves = j.value("moves", {});
        return std::make_unique<AttackCard>(name, dir, power, moves);
    });

    CardFactory::instance().register_type("DamageModificationCard", [](const json& j) {
        std::string name = j.value("name", "Unnamed_DMG");
        int mod = j.value("mod", 0);
        bool can_be_dazed = j.value("can_be_dazed", false);
        return std::make_unique<DamageModificationCard>(name, mod, can_be_dazed);
    });

    // Add more card types here...
}

std::unordered_map<std::string, std::unique_ptr<BaseCard>> load_cards_from_json(const std::string& filename) {
    std::unordered_map<std::string, std::unique_ptr<BaseCard>> cards;

    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open card JSON file.");
    json card_data; in >> card_data;

    for (const auto& j : card_data) {
        auto card = CardFactory::instance().create(j);
        cards[card->name] = std::move(card);
    }

    return cards;
}