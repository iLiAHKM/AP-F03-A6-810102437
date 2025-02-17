#ifndef RESTAURANT_METHODS
#define RESTAURANT_METHODS
#include "classes.hpp"

Resturant::Resturant(string _name,string _district,string _foods_str, shared_ptr<map<string, string>> _foods,string _oppening,string _closing,string _number_of_tables){
    name = _name ;
    district = _district;
    foods  = _foods;
    reservation_id = 1;
    oppening = _oppening;
    closing = _closing;
    foods_str = _foods_str;
    
    number_of_tables = _number_of_tables;
    tables = make_shared<vector<Table>>();
    for (int i = 1; i <= stoi(number_of_tables); i++)
    {
        reserved_chair[i] = "";
        tables->emplace_back(i);
    }
    
}

Table Resturant::get_table_by_id(int id){
    auto it = find_if(tables->begin(), tables->end(), [id] (Table& table){
        return table.get_id() == id;
    });
    return *it;
}

// map<string, string> Resturant::prepare_food(string food_str){
//     vector<string> pairs = splitStringByDelimiter4(food_str, ';');
//     map<string, string> food_map;
//     vector<string> tempt_pair;
//     for(auto pair : pairs){
//         tempt_pair = splitStringByDelimiter4(pair, ':');
//         food_map[tempt_pair[0]] = tempt_pair[1];
//     }
//     foods = food_map;
// }

void Resturant::set_chair_reserve(int chair,string time){
    reserved_chair[chair] = time;
}
void Resturant::set_table_as_reserved(int id, pair<int, int> interval){
    auto it = find_if(tables->begin(), tables->end(), [id] (Table& table){
        return table.get_id() == id;
    });
    if(it != tables->end()){
        it->set_new_reservtion(interval);
    }
    else{
        cout<<"Not Found"<<endl;
        return;
    }
}
map<int, string> Resturant::get_resrved_chair(){ return reserved_chair; }

string Resturant::get_oppening(){
    return oppening;
}

string Resturant::get_closing(){
    return closing;
}

string Resturant::get_number_of_tables(){
    return number_of_tables;
}

map<int,string> Resturant::get_reserve_chairs(){
    return reserved_chair;
}

int Resturant::get_reservation_id(){ return reservation_id; }

void Resturant::increase_reservation_id(){ ++reservation_id; }

shared_ptr<map<string, string>> Resturant::get_foods() { return foods; }

vector<string> splitStringByDelimiter4(const string input, char delimiter) {
    vector<std::string> result;
    stringstream system(input);
    string item;

    while (std::getline(system, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

#endif