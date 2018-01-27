#include "libs.hpp"

void pos::to_json(json& j, const customer& c) {
    j = json{{"user_id", c.user_id}, {"name", c.name}, {"latitude", c.latitude}, {"longitude", c.longitude}};
}

void pos::from_json(const json& j, customer& c) {
    c.user_id = j.at("user_id").get<int>();
    c.name = j.at("name").get<std::string>();
    c.latitude = ::atof(j.at("latitude").get<std::string>().c_str())*DEG_TO_RAD;
    c.longitude = ::atof(j.at("longitude").get<std::string>().c_str())*DEG_TO_RAD;
}
