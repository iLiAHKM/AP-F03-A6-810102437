// #ifndef COMMANDS
// #define COMMANDS
#include "header.hpp"
#include "classes.hpp"
#include "reserve.hpp"
bool POST_signup(vector<string> &commands,System &ss,User *user){
    string username = commands[4];
    string password = commands[6];

    if (ss.repeated_username(username))
    {
        cout << "Bad Request" << username<< endl;
        return 0;
    }
    if (user != 0)
    {
        cout << "Permission Denied" << endl;
        return 0;
    }
    
    User newuser;
    ss.add_user(newuser);
    user = ss.get_last_user();
    
    user->set_username(username);
    user->set_password(password);

    cout << "OK" << endl;
    return 1;
    
}

string POST_login(vector<string> &commands,System &ss,User *user){
    string username = "";
    string password = "";
    if (commands[3] == "username")
    {
        string username = commands[4];
        string password = commands[6];
    
    }else{

        string username = commands[6];
        string password = commands[4];
    
    }
    username = commands[4];
    password = commands[6];  

    if(user != 0){
        cout << "Permission Denied";
        return "";
    }
    if (!ss.repeated_username(username))
    {
        cout << "Not Found" <<endl  ;
        return "";
    }
    
    User *x = ss.loggin(username,password);
    if(x == 0){
        cout << "Permission Denied" << endl;
        return "";
    }else{
        user = x;
        cout << "OK" << endl;
        return user->get_username();
    }
}

bool POST_logout(User *user){
    if(!user){
        cout << "Permission Denied";
        return 0;
    }else{
        user = 0;
        cout << "OK" << endl ;
        return 1;
    }
}


string POST(vector<string> &commands ,System &ss,User *user){
    
    if (commands.size() < 3){
        cout << "Bad Request" << endl;
        return "" ;
    }
    if (commands[1] == "reserve")
    {
        if(commands[2] == "?"){
            map<string,string> x = make_command_readble(commands);
            if (x["valid"] == "1")
            {
                set_reservation(ss, x, user);
            }else{
                cout << "Bad Request";
                return "";
            }
        }else{
            cout << "Bad Request";
            return "";
        }
        return "";
    }
    if (commands[1] == "signup" )
    {
        if(commands.size() >= 6){
            if (commands[2] == "?" & commands[3] == "username"  && commands[5] == "password")
            {
                if(POST_signup(commands,ss,user) == 1){
                    return "signup-1";
                }
                
            }else{
                  cout << "Bad Request" << endl;
                
            }
            
        }else{
                cout << "Bad Request" << endl;
            
        }
        return "";
    }else if(commands[1] == "login"){
        if(commands.size() >= 7){
            if (commands[2] == "?" & commands[3] == "username"  && commands[5] == "password")
            {
                string k = POST_login(commands,ss,user);
                if (k!="")
                {
                    return k;
                }                
            }else{
                  cout << "Bad Request" << endl;
            }
            
        }else{
                cout << "Bad Request" << endl;
        }
        return "";
    }else if(commands[1] == "logout"){
        if (commands[2] == "?"){
            if (POST_logout(user) ==1)
            {
                return "logout-succsess";
            }
            
        }
        return "" ;
        
    }

    
    cout << "Not Found"<<endl;
    return "";
}

vector<string> splitStringByDelimiter(const string& input, char delimiter) {
    vector<std::string> result;
    stringstream ss(input);
    string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}


void GET(vector<string> &commands,System &ss,User * user){
    if(commands.size() < 3){
        cout << "Bad Request" << endl;
        return ;
    }
    if(commands[1] == "districts"){
        if(commands[2] == "?"){
            if(commands.size() >= 3){
                if(commands.size() == 3){
                    if (ss.get_districts().size() == 0)
                    {
                        cout << "Empty";
                        return ;
                    }
                    
                    for (const auto& [key, value] : ss.get_districts()) {
                        cout << key << ":";
                        vector<string> x = splitStringByDelimiter(value,';');
                        for(auto a : x){
                            if (a != x.back())
                            {
                                cout <<" "<< a <<",";                        
                            }else{
                                cout <<" "<< a ;                        
                            }
                        }
                        cout << endl;
                    }

                }else{
                    if(commands.size() <5){
                        cout << "Bad Request" << endl;
                        return ;
                    }else{
                        if (commands[3] != "district")
                        {
                            cout << "Bad Request" << endl;
                            return ;
                        }else{
                            string mahalleh = commands[4];
                            mahalleh.erase(0,1);
                            mahalleh.erase(mahalleh.size() - 1, 1);
                            string determine = "";
                            for (auto& [key,value] : ss.get_districts())
                            {
                                if(key == mahalleh){
                                    determine = mahalleh;
                                    break;
                                }
                            }

                            if(determine == ""){
                                cout << "Not Found";
                                return ;
                            }else{
                                cout << determine;
                                vector<string> x = splitStringByDelimiter(ss.get_districts()[determine],';');
                                for (int i = 0; i < x.size(); i++)
                                {
                                    if (i!= x.size()-1)
                                    {
                                        cout << " "<< x[i]<<",";
                                    }else{
                                        cout << " "<< x[i];
                                    }      
                                }
                                cout << endl;
                            }    
                        }  
                    }
                }
            }
        }else{
            cout << "Bad Request" << endl;
            return;
        }

        return ;
    }
    if (commands[1] == "restaurants")
    {
        if(commands[2] == "?"){
            if (user->get_district() == "")
            {
                cout << "Not Found";
                return;
            }
            
            vector<string> gone_districts;
            vector<string> future_search;
            resturant_list(ss,user,future_search,gone_districts,0);
            int b = 0;
            for (auto i : gone_districts)
            {
                vector<shared_ptr<Resturant>> a = get_resturent_filter_district(ss,i); 
                if(a.size() > 0){
                    for (auto z : a)
                    {
                        if(commands.size() >= 4 && commands[3] == "food_name"){
                            if (resturant_has_food(z,get_string_like_normal(commands[4])))
                            {
                                cout << z->get_name() << " (" << z->get_distric() << ")" << endl ;
                                b++;
                            }
                            
                        }else{
                        cout << z->get_name() << " (" << z->get_distric() << ")" << endl ;
                        b++;
                        }
                    }
                }   
            }
            if(b == 0){
                cout << "Empty";
            }
            return;
            
        }else{
            cout  << "Bad Request";
        }
    }

    if (commands[1] == "restaurant_detail")
    {
        if (commands.size() < 5)
        {
            try
            {
                cout << "Bad Request";
            }
            catch(...)
            {
                cout << "Bad Request";
            }
            
            return;
        }
        
        if (commands[2] == "?" && commands[3] == "restaurant_name")
        {
            string comm=commands[4];
            for (int i = 4; i < commands.size(); i++)
            {
                if (i == 4)
                {
                    continue;
                }
                comm += " " +commands[i];
            }
            
            restaurant_detail(ss,get_string_like_normal(comm));
            return;
        }else{
            cout << "Bad Request";
            return;
        }
    }
    if(commands[1] == "reserve")
    {
        
        if(commands[2] == "?")
        {
            map<string,string> input_informations2 = make_command_readble2(commands);
        if (input_informations2["valid"] == "1")
        {
            reservation_report(ss, user, input_informations2);
        }else{

        }
        
            

        }else{
            cout << "Bad Request";
            return;
        }
        return;
    }
    
    cout << "Not Found"<<endl;
}


void PUT(vector<string> &commands,System &ss,User * user){
    if (user == 0)
    {
        cout <<"Permission Denied"<<endl;
        return;
    }
    
    if(commands.size() < 5){
        cout << "Bad Request" << endl;
        return;
    }

    if (commands[1] == "my_district")
    {
        if (commands[2] == "?" && commands[3] == "district")
        {
            if (commands.size() >4)
            {
                string user_wish_district = commands[4];
                user_wish_district.erase(0,1);
                user_wish_district.erase(user_wish_district.size() - 1, 1);
                for (auto& [key,value] : ss.get_districts())
                {
                    if (user_wish_district == key){
                        user->set_district(key);
                        cout << "OK"<<endl;
                        return;
                    }
                }
                cout << "Not Found" << endl;
                return;
                
            }else{
                cout << "Bad Request" << endl;
                return;
            }
            
        }else{
            cout << "Bad Request" << endl;
            return;
        }
        
    }
    cout << "Bad Request1";
}
// #endif