#include "j_stream.h"

std::ostream& operator<<(std::ostream &os, const Json &json){
    bool first = true;
    os << "{";
    print_collection_Json<double>(os, json.numbers, first);
    print_collection_Json_lines(os, json.lines, first);
    print_collection_Json_bool(os, json.bools, first);
    print_collection_Json<Json>(os, json.jsons, first);
    print_collection_Json<J_array>(os, json.arrays, first);
    if (!first) os << std::endl  << "}";
    else os << "}";

    return os;
}

std::ostream& operator<<(std::ostream &os, const J_array &array_){
    os << "[";
    for (unsigned int key = 0; key < array_.size_a; key++){
    if (key != 0) os << ",";
    if (array_.numbers.find(key) != array_.numbers.end()) {os << array_.numbers.at(key); continue;}
    if (array_.lines.find(key) != array_.lines.end()) {
            if (array_.lines.at(key) == "") {os << "null" ; continue;}
            os << "\"" << array_.lines.at(key) << "\""; continue;
    }
    if (array_.bools.find(key) != array_.bools.end()) {os << (array_.bools.at(key) ? "true" : "false"); continue;}
    if (array_.jsons.find(key) != array_.jsons.end()) {os << std::endl << array_.jsons.at(key); continue;}
    if (array_.arrays.find(key) != array_.arrays.end()) {os << std::endl << array_.arrays.at(key); continue;}
    }
    os << "]";
    return os;
}

void print_collection_Json_lines(std::ostream &os, const std::map<std::string, std::string>& collection, bool &first){
    if (collection.empty()) return;
    for (auto it = collection.begin(); it != collection.end(); it++){
        os << (first ? "" : ",")
        << std::endl
        << "\"" << (it->first) << "\""
        << ":";
        if (it->second == "") os << "null";
        else os << "\""<< (it->second) << "\"";
        if (first) first = false;
    }
}
void print_collection_Json_bool(std::ostream &os, const std::map<std::string, bool>& collection, bool& first){
    if (collection.empty()) return;
    for (auto it = collection.begin(); it != collection.end(); it++){
        os << (first ? "" : ",")
        << std::endl
        << "\"" << (it->first) << "\""
        << ":"
        << (it->second ? "true" : "false");
        if (first) first = false;
    }
}

std::istream& operator>>(std::istream &is, Json &json){
    for (char i = (char)is.get(); i != '{' && i != '\0'; i = (char)is.get()){if (i == '\0') return is;}
    Json_parcer ans(is);
    ans.parce(json);
    return is;
}

std::istream& operator>>(std::istream &is, J_array &array_){
    for (char i = (char)is.get(); i != '[' && i != '\0'; i = (char)is.get()){if (i == '\0') return is;}
    J_array_parcer ans(is);
    ans.parce(array_);
    return is;
}
// J_SON PARCER methods

bool Json_parcer::is_blank(char c){
    return (c == ' ' || c == '\n' || c == '\t' || c == 'v');
}
char Json_parcer::read_blank(){
    char i;
    for (i = get(); is_blank(i); i = get());
    return i;
}
std::string Json_parcer::find_name(){
    std::string rem;

    char i; std::vector<char> ans;
    i = read_blank(); rem = memory();
    if (i == '}') throw END_REACHED;
    if (i != '"') throw parcer_exception("CANNOT FIND NEW FIELD", rem);

    return close_string(memory());
}
std::string Json_parcer::close_string(std::string rem){
    char i; std::vector<char> ans;

    i = get();
    while (i != '"'){
        ans.push_back(i); i = get();
        if (i == '\0') throw parcer_exception("\" NOT CLOSED", rem);
    }
    return std::string(ans.begin(), ans.end());
}
bool Json_parcer::find_value(Json& json, std::string key){
    std::string rem = memory(); int result = NOWHERE;

    char i = read_blank();
    if (i != ':') throw parcer_exception("NO \':\' FOUND", rem);
    i = read_blank();

    if (i == '"') read_string(json, key);
    else if (i == '{') read_json(json, key);
    else if (i == '[') read_array(json, key);
    else if (i == 't' || i == 'f' || i == 'n') read_bool(json, key, i);
    else result = read_number(json, key, i);

    if (result != NOWHERE) {
        if (result == COMMA_REACHED) return COMMA_REACHED;
        return END_REACHED;
    }

    i = read_blank();
    if (i != ',' && i != '}') throw parcer_exception("MISSING \",\"", rem);
    if (i == ',') return COMMA_REACHED;
    return END_REACHED;
}

void Json_parcer::read_string(Json& json, std::string key){
    std::string rem = memory();
    std::string ans = close_string(memory()); bool result = false;

    result = json.add_line(key, ans);

    if (!result) throw parcer_exception("DUPLICATE KEY", rem);
}
void Json_parcer::read_json(Json& json, std::string key){
    std::string rem = memory();

    Json_parcer parcer(in);
    Json ans; parcer.parce(ans);

    if (!json.add_json(key, ans)) throw parcer_exception("DUPLICATE KEY", rem);
}
void Json_parcer::read_array(Json& json, std::string key){
    std::string rem = memory();

    J_array_parcer parcer(in);
    J_array ans; parcer.parce(ans);

    if (!json.add_array(key, ans)) throw parcer_exception("DUPLICATE KEY", rem);
}
int Json_parcer::read_number(Json& json, std::string key, char i){
    std::string rem = memory(); bool result = false;
    std::vector<char> number; number.push_back(i);

    for (i = get(); !is_blank(i) && i != ',' && i != '}'; i = get()) number.push_back(i);
    std::string line(number.begin(), number.end());

    double ans = atof(line.c_str());
    if (ans == 0){
        if (line == "0" || line == "0.0" || line == "0.") result = json.add_number(key, ans);
        else throw parcer_exception("UNRECOGNIZED VALUE", rem);
    }
    else result = json.add_number(key, ans);
    if (!result) throw parcer_exception("DUPLICATE KEY", rem);

    if (i == '}') return END_REACHED;
    if (i == ',') return COMMA_REACHED;
    return NOWHERE;
}
void Json_parcer::read_bool(Json& json, std::string key, char i){
    std::vector<char> ans; ans.push_back(i);
    int times = 4;
    if (i == 'f') times = 5;

    for (int j = 1; j < times; j++){
        i = get(); if (i == '\0') throw parcer_exception("UNKNOWN VALUE", memory());
        ans.push_back(i);
    }

    std::string line(ans.begin(), ans.end());
    if (line == "true") {json.add_bool(key, true); return;}
    if (line == "false") {json.add_bool(key, false); return;}
    if (line == "null") {json.add_line(key, ""); return;}
}

std::string Json_parcer::memory(){
    return std::string(memory_.begin(), memory_.end());
}
char Json_parcer::get(){
    int i = in.get();
    if (i == -1) return '\0';
    memory_.push_back((char)i);
    if ((int)memory_.size() > 10) memory_.pop_front();
    return (char)i;
}
Json_parcer::Json_parcer(std::istream& in):in(in){}
void Json_parcer::parce(Json& json){
    bool end_reached = false;
    while(!end_reached){
        try{
        end_reached = find_value(json, find_name());
        }catch(Where_are_We e){return;}
    }
}

// J_ARRAY PARCER methods


J_array_parcer::J_array_parcer(std::istream& in): in(in){}
bool J_array_parcer::is_blank(char c){
        return (c == ' ' || c == '\n');
}
char J_array_parcer::get(){
    int i = in.get();
    if (i == -1) return '\0';
    memory_.push_back((char)i);
    if ((int)memory_.size() > 10) memory_.pop_front();
    return (char)i;
}
char J_array_parcer::read_blank(){
    char i;
    for (i = get(); is_blank(i); i = get());
    return i;
}
std::string J_array_parcer::memory(){
    return std::string(memory_.begin(), memory_.end());
}

std::string J_array_parcer::close_string(std::string rem){
    char i; std::vector<char> ans;

    i = get();
    while (i != '"'){
        ans.push_back(i); i = get();
        if (i == '\0') throw parcer_exception("\" NOT CLOSED", rem);
    }
    return std::string(ans.begin(), ans.end());
}
bool J_array_parcer::find_value(J_array& array_){
    int result = NOWHERE; std::string rem = memory();

    char i = read_blank();
    if (i == ']') return END_REACHED;
    if (i == '"') read_string(array_);
    else if (i == '{') read_json(array_);
    else if (i == '[') read_array(array_);
    else result = read_number(array_, i);

       if (result != NOWHERE) {
        if (result == COMMA_REACHED) return COMMA_REACHED;
        return END_REACHED;
    }

    i = read_blank();
    if (i != ',' && i != ']') throw parcer_exception("MISSING \",\"", rem);
    if (i == ',') return COMMA_REACHED;
    return END_REACHED;
}

void J_array_parcer::read_string(J_array& array_){
    std::string rem = memory();
    std::string ans = close_string(memory());

    if (ans == "true") array_.push_back_bool(true);
    else if (ans == "false") array_.push_back_bool(true);
            else array_.push_back(ans);
}
void J_array_parcer::read_json(J_array& array_){
    std::string rem = memory();

    Json_parcer parcer(in);
    Json ans; parcer.parce(ans);

    array_.push_back(ans);
}
void J_array_parcer::read_array(J_array& array_){
    std::string rem = memory();

    J_array_parcer parcer(in);
    J_array ans; parcer.parce(ans);

    array_.push_back(ans);
}
int J_array_parcer::read_number(J_array& array_, char i){
    std::string rem = memory();
    std::vector<char> number; number.push_back(i);

    for (i = get(); !is_blank(i) && i != ',' && i != ']'; i = get()) number.push_back(i);
    std::string line(number.begin(), number.end());

    double ans = atof(line.c_str());
    if (ans == 0){
        if (line == "0" || line == "0.0" || line == "0.") array_.push_back(ans);
        else throw parcer_exception("UNRECOGNIZED VALUE", rem);
    }
    else array_.push_back(ans);

    if (i == ']') return END_REACHED;
    if (i == ',') return COMMA_REACHED;
    return NOWHERE;
}


void J_array_parcer::parce(J_array& array_){
    bool end_reached = false;
    while (!end_reached){
        end_reached = find_value(array_);
    }
}







