// #ifndef CLASSES
// #define CLASSES
#include "header.hpp"



class Table{
public:
    Table(int id_) {
        id = id_;
        // reservation = make_shared<RservInterval>();
    }
    void set_new_reservtion(const pair<int, int>& interval){ (reservation).push_back(interval); }
    int get_id(){ return id; }
    RservInterval get_reservation(){
        
        // if(!is_reserved()){
        //     cout<<"dddd"<<endl;
        //     return nullptr;
        // }
        return reservation; 
    }
    bool is_reserved(){ return !(reservation.empty()); }
private:
    int id;
    RservInterval reservation;
};

class Resturant {
public:
    Resturant(string _name,string _district,string _foods_str, shared_ptr<map<string, string>> _foods,string _oppening,string _closing,string _number_of_tables);
    Resturant(Resturant& obj){
        name = obj.get_name();
        district = obj.get_distric();
        foods = make_shared<map<string, string>>(*obj.get_foods());
        oppening = obj.get_oppening();
        closing = obj.get_closing();
        number_of_tables = obj.get_number_of_tables();
        reserved_chair = obj.get_reserve_chairs();
        tables = make_shared<vector<Table>>(*obj.get_tables());
        reservation_id = obj.get_reservation_id();
    }
    //a~Resturant(){ delete reservation_id; }
    string get_distric();
    string get_name();
    shared_ptr<map<string, string>> get_foods();
    string get_oppening();
    string get_closing();
    string get_number_of_tables();
    string get_food_str(){ return foods_str; }
    shared_ptr<vector<Table>> get_tables(){ return tables; }
    int get_resevation_id(){ return reservation_id; }
    map<int,string> get_reserve_chairs();
    Table get_table_by_id(int id);
    void set_chair_reserve(int chair,string time);
    int get_reservation_id();
    string get_foods_str() { return foods_str; }
    map<int, string> get_resrved_chair();
    void increase_reservation_id();
    void set_table_as_reserved(int id, pair<int, int> interval);
    bool has_reservation(int id){
        auto it = reserved_chair.find(id);
        if(it == reserved_chair.end()) return false;
        return true;
    }
private:
    string name;
    string district;
    shared_ptr<map<string, string>> foods;
    string foods_str;
    string oppening;
    string closing;
    string number_of_tables;
    map<int, string> reserved_chair;
    shared_ptr<vector<Table>> tables; 
    int reservation_id;
};

class User {
    public:
    string get_username();
    string get_password();
    string get_district();
    shared_ptr<map<string,string>> get_reservation();
    bool has_reservation(const string restaurant_name){
        if(restaurant_name != ""){
            auto it_rest = reservation2.find(restaurant_name);
            auto reservation_info = it_rest->second;
            for(auto it:reservation_info){
                if(!(it.second)->empty()) return true;
            }
            return false;
        }else{
            for(auto it_rest: reservation2){
                auto reservation_info = it_rest.second;
                for(auto it:reservation_info){
                    if(!(it.second)->empty()) return true;
                }
            }
            return false;
        }
    }
    bool has_this_reservation(const string restaurant_name, const int& reservation_id){
        auto reservations = this->get_reservation2();
        auto it_rest = reservations.find(restaurant_name);
        auto reservation_info = it_rest->second;
        auto it = reservation_info.find(reservation_id);
        return (it != reservation_info.end());
    }
    Reservation get_reservation2() { return reservation2; }
    void set_username(string usrname);
    void set_password(string password);
    void set_district(string district);
    void set_reservation(string x,string y);
    void set_reservation2(int id, pair<int, int> interval, const string& restaurant_name) {
        auto it_rest = reservation2.find(restaurant_name);
        if (it_rest == reservation2.end()) {
            cout << restaurant_name << endl;
            cout << "OPOPO" << endl;            
            reservation2[restaurant_name] = map<int, shared_ptr<RservInterval>>();
        }
        
        auto& reservation_info = reservation2[restaurant_name];

        auto it = reservation_info.find(id);
        if (it == reservation_info.end()) {
            cout << "2e2e2" << endl;            
            RservInterval vec = {interval};
            reservation_info[id] = make_shared<RservInterval>(vec);
            return;
        }
        
        (it->second)->push_back(interval);
}


    bool is_logged_in();
    private:
        string district = "";
        string username;
        string password;
        bool login_status = 0;
        bool is_auser = 0;
        shared_ptr<map<string,string>> reservation= make_shared<map<string,string>>();
        Reservation reservation2;
};

class System{
    public:
        System(){}
        void add_resturant(shared_ptr<Resturant> resturant);
        void add_district(string districts,string neighber);
        void add_user(User user);
        bool repeated_username(string username);
        User * loggin(string username , string password);
        User * get_last_user();
        User * get_lost_login(string username);
        vector<shared_ptr<Resturant>> get_resturants(); 
        map<string,string> get_districts();
    private:
        vector<shared_ptr<Resturant>> resturants;
        vector<User> users;
        map<string,string> districts;

};


// #endif