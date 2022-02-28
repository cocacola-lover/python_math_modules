#ifndef _JSON_STREAM_
#define _JSON_STREAM_

#include "json.h"
#include "J_array.h"
#include <deque>
#include <vector>
#include <stdlib.h>

enum Where_are_We{
    NOWHERE = 2,
    COMMA_REACHED = 0,
    END_REACHED = 1
};

class parcer_exception: public std::exception
{
    const std::string what_;
    const std::string where_;
public:
    parcer_exception(std::string what_, std::string where_) : what_(what_), where_("near " + where_){}
    const std::string where(){return where_;}
    const std::string what(){return what_;}
};

class Json_parcer{
    std::deque<char> memory_;
    std::istream& in;

    static bool is_blank(char c);
    char read_blank();
    char get();

    std::string find_name();
    std::string close_string(std::string rem);
    bool find_value(Json& json, std::string key);

    void read_string(Json& json, std::string key);
    void read_json(Json& json, std::string key);
    void read_array(Json& json, std::string key);
    void read_bool(Json& json, std::string key, char i);
    int read_number(Json& json, std::string key, char i);

    std::string memory();
public:
    Json_parcer(std::istream& in);
    void parce(Json& json);
};

class J_array_parcer{
    std::deque<char> memory_;
    std::istream& in;

    static bool is_blank(char c);
    char read_blank();
    char get();

    std::string close_string(std::string rem);
    bool find_value(J_array& array_);

    void read_string(J_array& array_);
    void read_json(J_array& array_);
    void read_array(J_array& array_);
    int read_bool(J_array& array_, char i);
    int read_number(J_array& array_, char i);

    std::string memory();
public:
    J_array_parcer(std::istream& in);
    void parce(J_array& array_);
};

template<typename Type>
void print_collection_Json(std::ostream &os, const std::map<std::string, Type>& collection, bool& first){
    if (collection.empty()) return;
    for (auto it = collection.begin(); it != collection.end(); it++){
        os << (first ? "" : ",")
        << std::endl
        << "\"" << (it->first) << "\""
        << ":"
        << (it->second);
        if (first) first = false;
    }
}

template<typename Type>
void print_collection_Json_quotes(std::ostream &os, const std::map<std::string, Type>& collection, bool& first){
    if (collection.empty()) return;
    for (auto it = collection.begin(); it != collection.end(); it++){
        os << (first ? "" : ",")
        << std::endl
        << "\"" << (it->first) << "\""
        << ":"
        << "\"" << (it->second) << "\"";
        if (first) first = false;
    }
}

void print_collection_Json_lines(std::ostream &os, const std::map<std::string, std::string>& collection, bool &first);
void print_collection_Json_bool(std::ostream &os, const std::map<std::string, bool>& collection, bool &first);
std::ostream& operator<<(std::ostream &os, const Json &json);
std::ostream& operator<<(std::ostream &os, const J_array &array_);

std::istream& operator>>(std::istream &is, Json &json);
std::istream& operator>>(std::istream &is, J_array &array_);



#endif // _JSON_STREAM_
