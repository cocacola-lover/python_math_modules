#ifndef _JSON_ACLASS_
#define _JSON_ACLASS_

#include "json.h"
#include <map>
#include <iostream>
#include <stdexcept>

class Json;

class J_array{
    unsigned int size_a = 0;
    std::map <unsigned int, double> numbers;
    std::map <unsigned int, std::string> lines;
    std::map <unsigned int, Json> jsons;
    std::map <unsigned int, bool> bools;
    std::map <unsigned int, J_array> arrays;

    template<typename Type>
    void push_pr(Type value, std::map<unsigned int, Type>& collection){
        collection.insert(std::pair<unsigned int, Type>(size_a, value));
        size_a++;
    }
    template <typename Type>
    void move_back(unsigned int key, std::map<unsigned int, Type>& collection){
        auto nodeHandler = collection.extract(key);
        nodeHandler.key() = key - 1;
        collection.insert(std::move(nodeHandler));
    }

public:
    unsigned int size();

    static bool same(const J_array a, const J_array b);
    friend bool operator==(const J_array a, const J_array b);

    double get_number(unsigned int key);
    std::string get_line(unsigned int key);
    Json get_json(unsigned int key);
    bool get_bool(unsigned int key);
    J_array get_array(unsigned int key);

    void push_back(std::string value);
    void push_back(double value);
    void push_back_bool(bool value);
    void push_back(Json value);
    void push_back(J_array value);

    void erase(unsigned int key);

    friend std::ostream& operator<<(std::ostream &os, const J_array &array_);
    friend std::istream& operator>>(std::istream &is, J_array &array_);
};

bool operator==(const J_array a, const J_array b);

#endif
