#include <algorithm>


void set_reservation(System &system, map<string,string> inputedinformation, User *user);

shared_ptr<Resturant> * get_resturant_ptr_filter_name(System &system,string name);

string find_value_by_key(const map<string, string> map, const string& x);

string find_value_by_key2(const map<int, string>& map, const int& x);

double calc_price(shared_ptr<map<string, string>>  foods_in_restaurant, const string& food);

bool has_foods(const vector<string>& foods_to_reserve,shared_ptr<map<string, string>>  foods_in_restaurant);

bool has_restaurant(System &system, const string& restaurant_name);


shared_ptr<Resturant> get_restuarant_filter_name(System system,string name);

map<string,string> make_command_readble(vector<string> commands);

string get_string_like_normal(string x);

bool compareByName(shared_ptr<Resturant>& a, shared_ptr<Resturant>& b);

vector<shared_ptr<Resturant>> get_resturent_filter_district(System &system,string  district);

vector<string> splitStringByDelimiter2(const string input, char delimiter);

vector<string> show_resturant_in_district(System &system,string district);

bool inside_out(vector<string> x,string y);

void get_resturant_give_foods_print(shared_ptr<Resturant> *r);

map<string,string> get_resturant_give_foods(shared_ptr<Resturant> r);

void get_resturant_give_foods_print(shared_ptr<Resturant> *r);

bool resturant_has_food(shared_ptr<Resturant> r,string food);

void resturant_list(System &system,User *user,vector<string> &future_search,vector<string> &gone_districts,int n );

void restaurant_detail(System &system,string name);


bool check_interfrence_of_user_reservaations(System system, const shared_ptr<map<string, string>>& user_reservations,
                                             const string restaurant_name, const string inputed_start_time, 
                                             const string inputed_end_time);




std::map<std::string, std::string> handleReserves(const std::string& command) {
    std::map<std::string, std::string> extractedKeyValuePairs;
    std::istringstream stream(command);
    std::string key, value;

    while (stream >> key) {
        if (stream.peek() == '"') {
            stream.ignore();
            std::getline(stream, value, '"');
            extractedKeyValuePairs[key] = value;
        }
    }

    return extractedKeyValuePairs;
}



struct TableInfo {
    string reservation_id;
    string table_id;
    string start_time;
    string end_time;
    map<string, int> food_counts;
};

bool compare_by_time(const TableInfo &a, const TableInfo &b) {
    if (a.start_time != b.start_time)
        return a.start_time < b.start_time;
    return a.end_time < b.end_time;
}

void stream_reservations(const shared_ptr<map<string, string>> &user_reservations, const string inputed_restaurant_name, const string inputed_reserve_id) {
    if(inputed_restaurant_name == "" && inputed_reserve_id == "")
    {
        for (const auto &entry : *user_reservations) {
            string restaurant_name = entry.first;
            string value = entry.second;

            
            istringstream stream(value);
            vector<TableInfo> table_infos;
            string token;
            while (stream >> token) {
                istringstream record_stream(token);
                string reservation_id, table_id, start_time, end_time;
                getline(record_stream, reservation_id, '-');
                getline(record_stream, table_id, '-');
                getline(record_stream, start_time, '-');
                getline(record_stream, end_time, '-');

                map<string, int> food_counts;
                string food_item;
                while (getline(record_stream, food_item, '-')) {
                    food_counts[food_item]++;
                }

                table_infos.push_back({reservation_id, table_id, start_time, end_time, food_counts});
            }

            sort(table_infos.begin(), table_infos.end(), compare_by_time);
            
            for (const auto &info : table_infos) {
                cout<<info.reservation_id<< ": "<<restaurant_name<<" "<<info.table_id
                    <<" "<<info.start_time<<"-"<<info.end_time;
                for(auto& food_pair: info.food_counts){
                    cout<<" "<<food_pair.first<<"("<<food_pair.second<<") ";
                }
                cout<<endl;
            }
        }
    }
    else{
        
        string restaurant_name = inputed_restaurant_name;
        string value = find_value_by_key(*user_reservations, inputed_restaurant_name);

        
        istringstream stream(value);
        vector<TableInfo> table_infos;
        string token;
        while (stream >> token) {
            istringstream record_stream(token);
            string reservation_id, table_id, start_time, end_time;
            getline(record_stream, reservation_id, '-');
            getline(record_stream, table_id, '-');
            getline(record_stream, start_time, '-');
            getline(record_stream, end_time, '-');

            map<string, int> food_counts;
            string food_item;
            while (getline(record_stream, food_item, '-')) {
                cout<<"ooo_item "<<food_item<<endl;
                food_counts[food_item]++;
            }

            table_infos.push_back({reservation_id, table_id, start_time, end_time, food_counts});
        }

        for (const auto &info : table_infos) {
            cout<<info.reservation_id<< ": "<<restaurant_name<<" "<<info.table_id
                <<" "<<info.start_time<<"-"<<info.end_time;
            for(auto& food_pair: info.food_counts){
                cout<<" "<<food_pair.first<<"("<<food_pair.second<<") ";
            }
            cout<<endl;
        }
    }
}





void reservation_report(System& system, User *user, map<string,string> inputed_information)
{
    string inputed_restauarnt_name = inputed_information["restaurant_name"];
    string inputed_reserve_id = inputed_information["reserve_id"];
    shared_ptr<map<string, string>> user_reservations = user->get_reservation();
    stream_reservations(user_reservations, inputed_restauarnt_name, inputed_reserve_id);
}




void printMap(const map<string, string>& myMap) 
{ 
    for (const auto& pair : myMap) 
    { 
        cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
    }
}
	


void set_reservation(System &system, map<string,string> inputedinformation, User *user)
{
    if(inputedinformation["valid"] == "0"){
        cout << "Bad Request" << endl ;
        return;
    }
    string restaurant_name = inputedinformation["restaurant_name"];
    shared_ptr<Resturant> * resturant = get_resturant_ptr_filter_name(system, restaurant_name);
    if (*resturant == shared_ptr<Resturant>(NULL)){
        cout<<"Not Found"<<endl;
        return;
    }
    string table_id = inputedinformation["table_id"];
    string inputed_start_time = inputedinformation["start_time"];
    string inputed_end_time = inputedinformation["end_time"];
    string resturant_start_time = (*resturant)->get_oppening();
    string resturant_end_time  = (*resturant)->get_closing();
    // Added
    string food = inputedinformation["foods"];
    double estimated_price = 0;
    shared_ptr<map<string, string>> foods_in_restaurant = (*resturant)->get_foods();
    map<int, string> restaurant_reserved_chairs = (*resturant)->get_reserve_chairs();
    string times_of_occupied_of_table = find_value_by_key2(restaurant_reserved_chairs, stoi(table_id));
    vector<string> intervals_of_occupied_of_table = splitStringByDelimiter2(times_of_occupied_of_table, ',');
    vector<string> start_and_end;
    shared_ptr<map<string, string>> user_reservations = user->get_reservation();
    vector<string> foods_to_reserve = splitStringByDelimiter2(food, ',');
    string reservation_id = to_string((*resturant)->get_reservation_id());
    if(!(inputedinformation["food_valid"] == "")){
        food = inputedinformation["foods"];
        if(!has_foods(foods_to_reserve, foods_in_restaurant)){
            cout<<"Not Found1"<<endl;
            return;
        }
        estimated_price = calc_price(foods_in_restaurant, food);    
        if(estimated_price == -1){
            cout<<"Not Found4"<<endl;
            return;
        }
        

    }

    //printMap(*foods_in_restaurant);
    if(!has_restaurant(system, restaurant_name)){
        cout<<"Not Found2"<<endl;
        return;
    }
    if(restaurant_reserved_chairs.find(stoi(table_id)) == restaurant_reserved_chairs.end()){
        cout<<"Not Found3"<<endl;
        return;
    }
    
    for(auto interval : intervals_of_occupied_of_table)
    {
        start_and_end = splitStringByDelimiter2(interval, '-');
        if(!(stoi(inputed_start_time) >= stoi(start_and_end[1]) || stoi(inputed_end_time) <= stoi(start_and_end[0])))
        {
            cout<<"Permission Denied1"<<endl;
            return;
        }
    }
    if(!(stoi(resturant_start_time) <= stoi(inputed_start_time) &&
         stoi(resturant_end_time) >= stoi(inputed_end_time))  || 
         ((stoi(inputed_start_time) == 1 && stoi(inputed_end_time) == 24)))
    {
        cout<<"Permission Denied2"<<endl;
        return;
    }
    
    if(check_interfrence_of_user_reservaations(system, user_reservations, restaurant_name, inputed_start_time, inputed_end_time))
    {
        cout<<"Permission Denied3"<<endl;
        return;
    }
    string tempt_foods="";
    for(auto food:foods_to_reserve){
        tempt_foods = tempt_foods+"-"+food;
    }
    user->set_reservation(restaurant_name,reservation_id+"-"+table_id+"-"+inputed_start_time+"-"+inputed_end_time+tempt_foods);
    (*resturant)->set_chair_reserve(stoi(table_id), inputed_start_time+"-"+inputed_end_time);
    (*resturant)->increase_reservation_id();

    cout<<"Reserve ID: "<<reservation_id<<endl;
    cout<<"Table "<<table_id<<" for "<<inputed_start_time
    <<" to "<<inputed_end_time<<" in "<<restaurant_name<<endl;
    cout<<"Price: "<<estimated_price<<endl;
    cout << "done reservation" << endl;
    // cout<<"AAAAA\n"<<resturant->get_foods_str()<<endl;
}

bool check_interfrence_of_user_reservaations(System system, const shared_ptr<map<string, string>>& user_reservations,
                                             const string restaurant_name, const string inputed_start_time, const string inputed_end_time)
{
    vector<string> user_reservation_ids_in_this_restaurant;
    string the_chair_reservation;
    vector<string> start_and_end;
    vector<string> the_chair_reservation_intervals;
    for(const auto& pair : *user_reservations){
        // if (pair.first == restaurant_name)
        //     continue;
        user_reservation_ids_in_this_restaurant = splitStringByDelimiter2(pair.second, ',');
        for (auto id : user_reservation_ids_in_this_restaurant){
            the_chair_reservation = find_value_by_key2((*(get_resturant_ptr_filter_name(system, restaurant_name)))->get_reserve_chairs(), stoi(id));
            cout<<"EEEEEE "<<the_chair_reservation<<endl;
            the_chair_reservation_intervals = splitStringByDelimiter2(the_chair_reservation, ',');
            for(auto interval : the_chair_reservation_intervals){
                start_and_end = splitStringByDelimiter2(interval, '-');
                if((stoi(inputed_start_time) >= stoi(start_and_end[1]) || stoi(inputed_end_time) <= stoi(start_and_end[0])))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

string find_value_by_key(const map<string, string> map, const string& x) { 
    auto it = (map).find(x); 
    if (it != (map).end()) { 
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


double calc_price(shared_ptr<map<string, string>>  foods_in_restaurant, const string& food)
{
    
    double estimated_price = 0;
    for(auto food_name : splitStringByDelimiter2(food, ','))
    {
        try
        {
            if (find_value_by_key((*foods_in_restaurant), food_name) == "")
            {
                return -1;
            }
            estimated_price += stoi(find_value_by_key((*foods_in_restaurant), food_name));
        }
        catch(...)
        {
            return -1;
        }
    }
    return estimated_price;
}

bool has_foods(const vector<string>& foods_to_reserve, shared_ptr<map<string, string>>  foods_in_restaurant){
    for (const auto& food_name : foods_to_reserve){
        if((*foods_in_restaurant).find(food_name) == (*foods_in_restaurant).end()){
        return false;
        }
    }
    return true;
}  

bool has_restaurant(System &system, const string& restaurant_name){
    for(auto restaurant : system.get_resturants()){
        if(restaurant_name == restaurant->get_name()){
            return true;
        }
    }
    return false;
}



shared_ptr<Resturant> * get_resturant_ptr_filter_name(System &system,string name){
    int aa = 0;
    for (int i = 0 ; i < system.get_resturants().size();i++)
    {
        aa++;
        if (system.get_resturants()[i]->get_name() == name)
        {
            return (new shared_ptr<Resturant>(system.get_resturants()[i]));
        }

    }
    return new shared_ptr<Resturant>(NULL);
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



map<string,string> make_command_readble2(vector<string> commands){
    map<string,string> z ;
    string c = "" ;
    for (auto  i : commands)
    {
        c += " " + i;
    }
    
    bool name = 0;
    bool chair_num = 0;

    for (auto i : commands)
    { 
        if (i == "restaurant_name")
        {
            name = 1;
        }
        if (i == "reserve_id")
        {
            chair_num = 1;
        }
       
    }
    
    if(!(name && chair_num) || (name && chair_num))
    {
        z["valid"] = "1";
    }else{
        z["valid"] = "0";
        return z;
    }
   

    string d =  "";
    size_t pos1 = 0;
    size_t pos2 = 0;
    if(name == 1 & chair_num == 1){
        pos1 = c.find("restaurant_name");
        pos1 = c.find('"',pos1);
        pos2 = c.find('"',pos1+1);
        d = c.substr(pos1+1,pos2-pos1-1);
        z["restaurant_name"] = d;
        pos1 = c.find("reserve_id");
        pos1 = c.find('"',pos1);
        pos2 = c.find('"',pos1+1);
        d = c.substr(pos1+1,pos2-pos1-1);    
        z["reserve_id"] = d;
    }
    return z;
}


string get_string_like_normal(string x){
    x.erase(0,1);
    x.erase(x.size()-1,1);
    return x;
}

bool compareByName(shared_ptr<Resturant>& a, shared_ptr<Resturant>& b) {
    return a->get_name() < b->get_name();
}

vector<shared_ptr<Resturant>> get_resturent_filter_district(System &system,string  district){
    vector<shared_ptr<Resturant>> x ;
    for (auto i : system.get_resturants())
    {
        if (i->get_distric() == district)
        {
            x.push_back(i);
        } 
    }
    sort((x.begin()), (x.end()), compareByName);
    return x;
}

vector<string> splitStringByDelimiter2(const string input, char delimiter) {
    vector<string> result;
    stringstream system(input);
    string item;

    if(input == "") return result;
    while (std::getline(system, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

vector<string> show_resturant_in_district(System &system,string district){
    vector<string> row;
    vector<shared_ptr<Resturant>> resturants = system.get_resturants();
    for(auto &resturant : resturants){
        if (district == resturant->get_distric())
        {
            row.push_back(resturant->get_name());
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

shared_ptr<Resturant> get_restuarant_filter_name(System system,string name){
    for (auto i : system.get_resturants())
    {
        if (i->get_name() == name)
        {
            return i;
        }      
    }
    return system.get_resturants()[0];
}

map<string,string> get_resturant_give_foods(shared_ptr<Resturant> r){
    map<string,string> x;
    for (auto i : splitStringByDelimiter2(r->get_foods_str(),';'))
    {
        x[splitStringByDelimiter2(i,':')[0]] = splitStringByDelimiter2(i,':')[1];
    }
    return x;    
}

void get_resturant_give_foods_print(shared_ptr<Resturant> *r){
    for (auto i : splitStringByDelimiter2((*r)->get_foods_str(),';'))
    {
            cout << (*r)->get_name();
            cout << i<<endl;
            // cout << splitStringByDelimiter2(i,':')[0];
    }
}

bool resturant_has_food(shared_ptr<Resturant> r,string food){
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
        if (system.get_resturants()[j]->get_name() == name)
        {
            i = j;
            break;
        }
        
    }
    if (i != -1)
    {
       shared_ptr<Resturant> x  = system.get_resturants()[i];
       cout <<  "Name: " << x->get_name() << endl;
       cout << "District: " << x->get_distric() << endl;
       cout << "Time: " << x->get_oppening() << "-" << x->get_closing() << endl;
       cout << "Menu: ";
       map<string,string> foods = get_resturant_give_foods(x);
       for (auto [key,value] : foods)
       {
        cout << key << "(" << value << ")" << ", " ;
       }
       
       cout << endl;
        for (auto  [key,value] : x->get_reserve_chairs())
        {
            cout << key <<": "<<endl;
        }
        

    }else{
        cout << "Not Foundm" << endl;
        return;
    }
    
    
}