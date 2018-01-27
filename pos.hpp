#include "json/json.hpp"

using json = nlohmann::json;

const double DEG_TO_RAD = 0.01745329252;

namespace pos {

    struct customer {
    int user_id;
    std::string name;
    double latitude;
    double longitude;
    };

    void to_json(json& j, const customer& c);

    void from_json(const json& j, customer& c);
}
