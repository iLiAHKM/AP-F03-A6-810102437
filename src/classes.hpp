// #ifndef CLASSES
// #define CLASSES
#include "header.hpp"

class Resturant {
    public:
        Resturant(string _name,string _district,string _foods_str, shared_ptr<map<string, string>> _foods,string _oppening,string _closing,string _number_of_tables);
        //a~Resturant(){ delete reservation_id; }
        string get_distric();
        string get_name();
        shared_ptr<map<string, string>> get_foods();
        string get_oppening();
        string get_closing();
        string get_number_of_tables();
        map<int,string> get_reserve_chairs();
        void set_chair_reserve(int chair,string time);
        // Added
        int get_reservation_id();
        string get_foods_str() { return foods_str; }
        map<int, string> get_resrved_chair();
        void increase_reservation_id();
        // map<string, string> Resturant::prepare_food(string food_str);
    private:
        string name;
        string district;
        shared_ptr<map<string, string>> foods;
        string foods_str;
        string oppening;
        string closing;
        string number_of_tables;
        map<int, string> reserved_chair;
        int reservation_id;
};

class User {
    public:
    string get_username();
    string get_password();
    string get_district();
    shared_ptr<map<string,string>> get_reservation();
    void set_username(string usrname);
    void set_password(string password);
    void set_district(string district);
    void set_reservation(string x,string y);
    bool is_logged_in();
    private:
        string district = "";
        string username;
        string password;
        bool login_status = 0;
        bool is_auser = 0;
        shared_ptr<map<string,string>> reservation= make_shared<map<string,string>>();
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