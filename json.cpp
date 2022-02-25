#include "json.h"


bool Json::add_bool(std::string key, bool value){
    return add_pr<bool>(key, value, bools);
}

bool Json::add_line(std::string key, std::string value){
    return add_pr<std::string>(key, value, lines);
}

bool Json::add_number(std::string key, double value){
    return add_pr<double>(key, value, numbers);
}

bool Json::add_json(std::string key, Json value){
    return add_pr<Json>(key, value, jsons);
}
bool Json::add_array(std::string key, J_array value){
    return add_pr<J_array>(key, value, arrays);
}
//bool Json::add(std::string key, bool value){return add_bool(key, value);}
bool Json::add(std::string key, double value){return add_number(key, value);}
bool Json::add(std::string key, std::string value){return add_line(key, value);}
bool Json::add(std::string key, Json value){return add_json(key, value);}
bool Json::add(std::string key, J_array value){return add_array(key, value);}


double Json::get_number(std::string key){return numbers.at(key);}
bool Json::get_bool(std::string key){return bools.at(key);}
std::string Json::get_line(std::string key){return lines.at(key);}
Json Json::get_json(std::string key){return jsons.at(key);}
J_array Json::get_array(std::string key){return arrays.at(key);}

bool Json::same(const Json a, const Json b){
    if (a.numbers == b.numbers && a.lines == b.lines && a.bools == b.bools && a.jsons == b.jsons && a.arrays == b.arrays) return true;
    return false;
}
bool operator==(const Json a, const Json b){return Json::same(a, b);}

unsigned int Json::size(){return keys.size();}

bool Json::erase(std::string key){
    if (keys.find(key) == keys.end()) return false;
    keys.erase(key);
    if (numbers.find(key) != numbers.end()) {numbers.erase(key); return true;}
    if (lines.find(key) != lines.end()) {lines.erase(key); return true;}
    if (bools.find(key) != bools.end()) {bools.erase(key); return true;}
    if (jsons.find(key) != jsons.end()) {jsons.erase(key); return true;}
    if (arrays.find(key) != arrays.end()) {arrays.erase(key); return true;}
    throw std::range_error("Json integrity has been violated");
}
