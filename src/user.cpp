// #ifndef USER
// #define USER
#include "classes.hpp"

string User::get_username(){
    return username;
}

string User::get_password(){
    return password;
}

bool User::is_logged_in(){
    return login_status;
}

void User::set_username(string usrname){
    username =usrname;
}

void User::set_password(string _password){
    password =_password;
}

void User::set_district(string _district){
    district = _district;
}

string User::get_district(){
    return district;
}

void User::set_reservation(string x, string y){
    try//farz mikonam x name of resturan va yid reserv haye motenazer ba oon resturan basheh
    {
        (*reservation)[x] =(*reservation)[x] + " " + y;
    }
    catch(const std::exception& e)
    {
        cout<<232323<<endl;
        (*reservation)[x] = y;
    }
}

shared_ptr<map<string,string>> User::get_reservation(){
    return reservation;
}
// #endif