// #ifndef SYSTEM
// #define SYSTEM
#include "classes.hpp"

string Resturant::get_distric(){
    return district;
}
string Resturant::get_name(){
    return name;
}
// map<string, string> Resturant::get_foods(){
//     return foods;
// }



void System::add_resturant(shared_ptr<Resturant> resturant){
    resturants.push_back(resturant);
}
vector<shared_ptr<Resturant>> System::get_resturants(){
    return resturants; 
} 
void System::add_district(string district,string neighber){
    districts[district] = neighber;
}

bool System::repeated_username(string username){

    for (int i = 0;i < users.size();i++)
    {
        if (username == users[i].get_username())
            
            return 1;        
    }
    return 0;
    
}

User * System::loggin(string username , string password){
    User * x = 0;

    for (int i = 0;i < users.size() ; i++){
        if (users[i].get_username() == username)
        {
            if (users[i].get_password() == password)
            {
                x = &users[i];
                break;
            }
            
        }
        
    }
    return x;
}

void System::add_user(User user){
    users.push_back(user);
}
User * System::get_last_user(){
    return &users.back();
}
User * System::get_lost_login(string usrname){
    User *x = 0;
    for (int  i = 0; i < users.size(); i++)
    {
        if (users[i].get_username() == usrname){
            return &users[i];
            break;
        }

    }
    return x;
    
}

map<string,string> System::get_districts(){
    return districts;
}
// #endif