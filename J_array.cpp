#include "J_array.h"

bool J_array::same(const J_array a, const J_array b){
    if (a.numbers == b.numbers && a.lines == b.lines && a.bools == b.bools && a.jsons == b.jsons && a.arrays == b.arrays) return true;
    return false;
}
bool operator==(const J_array a, const J_array b){return J_array::same(a, b);}

void J_array::push_back(std::string value){push_pr<std::string>(value, lines);}
void J_array::push_back(double value){push_pr<double>(value, numbers);}
void J_array::push_back_bool(bool value){push_pr<bool>(value, bools);}
void J_array::push_back(Json value){push_pr<Json>(value, jsons);}
void J_array::push_back(J_array value){push_pr<J_array>(value, arrays);}

double J_array::get_number(unsigned int key){return numbers.at(key);}
std::string J_array::get_line(unsigned int key){return lines.at(key);}
Json J_array::get_json(unsigned int key){return jsons.at(key);}
bool J_array::get_bool(unsigned int key){return bools.at(key);}
J_array J_array::get_array(unsigned int key){return arrays.at(key);}

unsigned int J_array::size(){return size_a;}

void J_array::erase(unsigned int key)
{
    if (key >= size_a) throw std::range_error("Key out of range");

    if (numbers.find(key) != numbers.end()) numbers.erase(key);
    if (lines.find(key) != lines.end()) lines.erase(key);
    if (bools.find(key) != bools.end()) bools.erase(key);
    if (jsons.find(key) != jsons.end()) jsons.erase(key);
    if (arrays.find(key) != arrays.end()) arrays.erase(key);

    for (unsigned int i = key+1; i < size_a; i++){
        if (numbers.find(key) != numbers.end()) move_back<double>(key, numbers);
        if (lines.find(key) != lines.end()) move_back<std::string>(key, lines);
        if (bools.find(key) != bools.end()) move_back<bool>(key, bools);
        if (jsons.find(key) != jsons.end()) move_back<Json>(key, jsons);
        if (arrays.find(key) != arrays.end()) move_back<J_array>(key, arrays);
    }
}
