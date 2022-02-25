#ifndef _JSON_CLASS_
#define _JSON_CLASS_

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stdexcept>
#include "J_array.h"

class J_array;

class Json{
    std::set <std::string> keys;
    std::map <std::string, double> numbers;
    std::map <std::string, std::string> lines;
    std::map <std::string, Json> jsons;
    std::map <std::string, bool> bools;
    std::map <std::string, J_array> arrays;

    template<typename Type>
    bool add_pr(std::string key, Type value, std::map<std::string, Type>& collection){
        if (keys.find(key) != keys.end()) return false;
        keys.insert(key);
        collection.insert(std::pair<std::string, Type>(key, value));
        return true;
    }



public:

    static bool same(const Json a, const Json b);
    friend bool operator==(const Json a, const Json b);

    bool add_number(std::string key, double value);
    bool add_line(std::string key, std::string value);
    bool add_json(std::string key, Json value);
    bool add_bool(std::string key, bool value);
    bool add_array(std::string key, J_array value);
    bool add(std::string key, double value);
    bool add(std::string key, std::string value);
    bool add(std::string key, Json value);
    //bool add(std::string key, bool value);
    bool add(std::string key, J_array value);

    double get_number(std::string key);
    std::string get_line(std::string key);
    Json get_json(std::string key);
    bool get_bool(std::string key);
    J_array get_array(std::string key);

    bool erase(std::string key);

    unsigned int size();

    friend std::ostream& operator<<(std::ostream &os, const Json &json);
    friend std::istream& operator>>(std::istream &is, Json &json);
};

bool operator==(const Json a, const Json b);

#endif // _JSON_CLASS_
