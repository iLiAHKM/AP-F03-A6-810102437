#include <algorithm>

void set_reservation(System &system, map<string,string> inputedinformation, User *user)
{
    string restaurant_name = inputedinformation["restaurant_name"];
    Resturant * resturant = get_resturant_ptr_filter_name(system, restaurant_name);
    string table_id = inputedinformation["table_id"];
    string inputed_start_time = inputedinformation["start_time"];
    string inputed_end_time = inputedinformation["end_time"];
    string resturant_start_time = resturant->get_oppening();
    string resturant_end_time  = resturant->get_closing();
    // Added
    string food = inputedinformation["food_reserve"];
    double estimated_price = 0;
    map<string, string> foods_in_restaurant = resturant->get_foods();
    map<int, string> restaurant_reserved_chairs = resturant->get_reserve_chairs();
    string times_of_occupied_of_table = find_value_by_key2(restaurant_reserved_chairs, stoi(table_id));
    
    vector<string> intervals_of_occupied_of_table = splitStringByDelimiter2(times_of_occupied_of_table, ',');
    vector<string> start_and_end;
    vector<string> user_reservation_ids_in_this_restaurant;
    map<string, string> user_reservations = user->get_reservation();
    string the_chair_reservation;
    vector<string> the_chair_reservation_intervals;
    vector<string> foods_to_reserve = splitStringByDelimiter2(food, ',');
    if(!has_foods(foods_to_reserve, foods_in_restaurant)){
        cout<<"Not Found"<<endl;
        return;
    }
    if(!has_restaurant(system, restaurant_name)){
        cout<<"Not Found"<<endl;
        return;
    }
    
    if(restaurant_reserved_chairs.find(stoi(table_id)) == restaurant_reserved_chairs.end()){
        cout<<"Not Found"<<endl;
        return;
    }

    for(auto interval : intervals_of_occupied_of_table)
    {
        start_and_end = splitStringByDelimiter2(interval, '-');
        if(!(inputed_start_time >= start_and_end[1] || inputed_end_time <= start_and_end[0]))
        {
            cout<<"Permission Denied"<<endl;
            return;
        }
    }

    if(!(stoi(resturant_start_time) <= stoi(inputed_start_time) &&
         stoi(resturant_end_time) >= stoi(inputed_end_time))  || 
         ((stoi(inputed_start_time) == 1 && stoi(inputed_end_time) == 24)))
    {
        cout<<"Permission Denied"<<endl;
        return;
    }


    
    for(const auto& pair : user_reservations){
        if (pair.first == restaurant_name)
            continue;
        user_reservation_ids_in_this_restaurant = splitStringByDelimiter2(pair.second, ',');
        for (auto id : user_reservation_ids_in_this_restaurant){
            the_chair_reservation = find_value_by_key2((get_resturant_ptr_filter_name(system, restaurant_name))->get_reserve_chairs()
                                                        , stoi(id));
            the_chair_reservation_intervals = splitStringByDelimiter2(the_chair_reservation, ',');
            for(auto interval : the_chair_reservation_intervals){
                start_and_end = splitStringByDelimiter2(interval, '-');
                if(!(inputed_start_time >= start_and_end[1] || inputed_end_time <= start_and_end[0]))
                {
                    cout<<"Permission Denied"<<endl;
                    return;
                }
            }
        }
    }
    

    user->set_reservation(restaurant_name, table_id+"-"+inputed_start_time+"-"+inputed_end_time);

    resturant->set_chair_reserve(stoi(table_id), inputed_start_time+"-"+inputed_end_time);
    
    estimated_price = calc_price(foods_in_restaurant, food);
    if(estimated_price == -1){
        cout<<"Not Found"<<endl;
        return;
    }


    cout << "done reservation" << endl;
}

string find_value_by_key(const map<string, string>& map, const string& x) { 
    auto it = map.find(x); 
    if (it != map.end()) { 
        return it->second; 
    }
    else{
        return ""; 
    }
}

string find_value_by_key2(const map<int, string>& map, const int& x) { 
    auto it = map.find(x); 
    if (it != map.end()) { 
        return it->second; 
    }
    else{
        return ""; 
    }
}

double calc_price(const map<string, string>& foods_in_restaurant, const string& food)
{
    double estimated_price = 0;
    for(auto food_name : food)
    {
        try
        {
            if (find_value_by_key(foods_in_restaurant, &food_name) == "")
            {
                
                return -1;
            }
            estimated_price += stoi(find_value_by_key(foods_in_restaurant, &food_name));
        }
        catch(...)
        {
            return -1;
        }
    }
    return estimated_price;
}

bool has_foods(const vector<string>& foods_to_reserve, map<string, string> foods_in_restaurant){
    for (const auto& food_name : foods_to_reserve){
        if(foods_in_restaurant.find(food_name) == foods_in_restaurant.end()){
        return true;
        }
    }
    return falsel;
}  

bool has_restaurant(System &system, const string& restaurant_name){
    for(auto restaurant : system.get_resturants()){
        if(restaurant_name == restaurant.get_name()){
            return true;
        }
    }
    return false;
}



Resturant * get_resturant_ptr_filter_name(System &system,string name){
    for (int i = 0 ; i < system.get_resturants().size();i++)
    {
        if (system.get_resturants()[i].get_name() == name)
        {
            return &system.get_resturants()[i];
        }
    }
}

map<string,string> make_command_readble(vector<string> commands){
    map<string,string> z ;
    string c = "" ;
    for (auto  i : commands)
    {
        c += " " + i;
    }
    
    bool name = 0;
    bool chair_num = 0;
    bool come = 0;
    bool go = 0;
    bool food = 0;
    for (auto i : commands)
    {
      
        if (i == "restaurant_name")
        {
            name = 1;
        }
        if (i == "table_id")
        {
            chair_num = 1;
        }
        if (i == "start_time")
        {
            come = 1;
        }
        if (i == "end_time")
        {
            go = 1;
        }
        if (i == "foods")
        {
            food = 1;
        }
    }
    if(!(name && chair_num && come && go))
    {
        z["valid"] = "0";
        return z;
    }else{
        z["valid"] = "1";
    }
    if(!(food))
    {
        z["food_valid"] = "0";
        return z;
    }else{
        z["food_valid"] = "1";
    }

    string d =  "";
    size_t pos1 = 0;
    size_t pos2 = 0;
    
    pos1 = c.find("restaurant_name");
    pos1 = c.find('"',pos1);
    pos2 = c.find('"',pos1+1);
    d = c.substr(pos1+1,pos2-pos1-1);
    z["restaurant_name"] = d;

    pos1 = c.find("table_id");
    pos1 = c.find('"',pos1);
    pos2 = c.find('"',pos1+1);
    d = c.substr(pos1+1,pos2-pos1-1);    
    z["table_id"] = d;

    pos1 = c.find("start_time");
    pos1 = c.find('"',pos1);
    pos2 = c.find('"',pos1+1);
    d = c.substr(pos1+1,pos2-pos1-1);
    z["start_time"] = d;
    
    pos1 = c.find("end_time");
    pos1 = c.find('"',pos1);
    pos2 = c.find('"',pos1+1);
    d = c.substr(pos1+1,pos2-pos1-1);
    z["end_time"] = d;
    if (food == 1)
    {
        pos1 = c.find("foods");
        pos1 = c.find('"',pos1);
        pos2 = c.find('"',pos1+1);
        d = c.substr(pos1+1,pos2-pos1-1);
        z["foods"] = d;
        
    }else{
        z["foods"] =  "";
    }
    return z;
}

string get_string_like_normal(string x){
    x.erase(0,1);
    x.erase(x.size()-1,1);
    return x;
}

bool compareByName(Resturant& a, Resturant& b) {
    return a.get_name() < b.get_name();
}

vector<Resturant> get_resturent_filter_district(System &system,string  district){
    vector<Resturant> x ;
    for (auto i : system.get_resturants())
    {
        if (i.get_distric() == district)
        {
            x.push_back(i);
        } 
    }
    sort(x.begin(), x.end(), compareByName);
    return x;
}

vector<string> splitStringByDelimiter2(const string input, char delimiter) {
    vector<std::string> result;
    stringstream system(input);
    string item;

    while (std::getline(system, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

vector<string> show_resturant_in_district(System &system,string district){
    vector<string> row;
    vector<Resturant> resturants = system.get_resturants();
    for(auto &resturant : resturants){
        if (district == resturant.get_distric())
        {
            row.push_back(resturant.get_name());
        } 
    }
    return row;
}

bool inside_out(vector<string> x,string y){
    bool z = 0;
    for (auto i : x)
    {
        if(i == y){
            z = 1;
        }
    }
    return z;
}

Resturant get_restuarant_filter_name(System system,string name){
    for (auto i : system.get_resturants())
    {
        if (i.get_name() == name)
        {
            return i;
        }      
    }
    return system.get_resturants()[0];
}

map<string,string> get_resturant_give_foods(Resturant r){
    map<string,string> x;
    for (auto i : splitStringByDelimiter2(r.get_foods(),';'))
    {
        x[splitStringByDelimiter2(i,':')[0]] = splitStringByDelimiter2(i,':')[1];
    }
    return x;    
}

void get_resturant_give_foods_print(Resturant *r){
    for (auto i : splitStringByDelimiter2(r->get_foods(),';'))
    {
            cout << r->get_name();
            cout << i<<endl;
            // cout << splitStringByDelimiter2(i,':')[0];
    }
}

bool resturant_has_food(Resturant r,string food){
       map<string,string> foods = get_resturant_give_foods(r);
       for (auto [key,value] : foods)
       {
            if (key == food)
            {
                return 1;
            }  
       }
       return 0;
}

void resturant_list(System &system,User *user,vector<string> &future_search,vector<string> &gone_districts,int n = 0){
    
    if (n == 0){
        vector<string> user_neighbor_districts = splitStringByDelimiter2(system.get_districts()[user->get_district()],';');
        future_search.insert(future_search.end(),user_neighbor_districts.begin(),user_neighbor_districts.end());
        gone_districts.push_back(user->get_district());
        n++;
        resturant_list(system,user,future_search,gone_districts,n);
    }else{

        for(auto i : future_search){
            if (!inside_out(gone_districts,i))
            {
               gone_districts.push_back(i);
            }
            
        }
        if (gone_districts.size() >= system.get_districts().size())
        {
            return;
        }
    
        vector<string> copy_from_future;
        for(auto i : future_search){
            copy_from_future.push_back(i);
        }
        future_search.clear();
        for (auto i : copy_from_future)
        {
            for(auto z : splitStringByDelimiter2(system.get_districts()[i],';')){
                future_search.push_back(z);
            }
        }
        
       if (future_search.empty()) {
         return;
        }
        
        resturant_list(system,user,future_search,gone_districts,n);

    }

}

void restaurant_detail(System &system,string name){
    int i = -1;
    for (int j = 0; j<system.get_resturants().size();j++)
    {
        if (system.get_resturants()[j].get_name() == name)
        {
            i = j;
            break;
        }
        
    }
    if (i != -1)
    {
       Resturant x  = system.get_resturants()[i];
       cout <<  "Name: " << x.get_name() << endl;
       cout << "District: " << x.get_distric() << endl;
       cout << "Time: " << x.get_oppening() << "-" << x.get_closing() << endl;
       cout << "Menu: ";
       map<string,string> foods = get_resturant_give_foods(x);
       for (auto [key,value] : foods)
       {
        cout << key << "(" << value << ")" << ", " ;
       }
       
       cout << endl;
        for (auto  [key,value] : x.get_reserve_chairs())
        {
            cout << key <<": "<<endl;
        }
        

    }else{
        cout << "Not Foundm" << endl;
        return;
    }
    
    
}
