#include <iostream>
#include <string>

#include "../APHTTP-main/server/server.hpp"
#include "handlers.hpp"

void mapServerPaths(Server& server) {
    server.setNotFoundErrPage("static/404.html");
    server.get("/", new ShowPage("static/home.html"));
    server.get("/home.png", new ShowImage("static/home.png"));
    server.get("/rand", new RandomNumberHandler());
    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler());
    server.get("/up", new ShowPage("static/upload_form.html"));
    server.post("/up", new UploadHandler());
    server.get("/colors", new ColorHandler("template/colors.html"));
    server.get("/music", new ShowPage("static/music.html"));
    server.get("/music/moonlight.mp3", new ShowFile("static/moonlight.mp3", "audio/mpeg"));
}

int main(int argc, char** argv) {
    try {
        
        int port = argc > 1 ? std::stoi(argv[1]) : 5000;
        Server server(port);
        mapServerPaths(server);
        std::cout << "Server running on port: " << port << std::endl;
        server.run();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const Server::Exception& e) {
        std::cerr << e.getMessage() << std::endl;
    }
    return 0;
}

int main(int argc, char** argv) {
    try {
        User *first_user = 0;
        string command;
        vector<string> commands;
        System system;
        string resturant_file_name = argv[1];
        ifstream file(resturant_file_name);
        string line;
        getline(file, line);

        while (getline(file, line)) {
            stringstream lineStream(line);
            string cell;
            vector<string> row;

            while (getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }
            shared_ptr<Resturant> x = make_shared<Resturant>(row[0], row[1],row[2], prepare_food(row[2]), row[3], row[4], row[5]);
            system.add_resturant(x);
        }

        file.close();

        string district_file_name = argv[2];
        ifstream file2(district_file_name);
        getline(file2, line);

            while (getline(file2, line)) {
            stringstream lineStream(line);
            string cell;
            vector<string> row;

            while (getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }
            system.add_district(row[0],row[1]);
        }

        file2.close();

        vector<string> defined_commands = {"POST","GET","PUT", "DELETE"};

        while (getline(cin,command))
        {
            split_command(command, commands);
            if (commands.empty()) continue;
            string z;
            switch (detect_main_command(commands[0], defined_commands))
            {
                case 0:
                    cout << "Bad Request" << endl;
                    break;
                case 1:
                    z = POST(commands,system,first_user);
                    if(z == "signup-1"){
                        first_user = system.get_last_user();
                    }else if(z == "logout-succsess"){
                        first_user = 0;
                    }else if(z == ""){

                    }else{
                        first_user = system.get_lost_login(z);
                    }
                    
                    break;
                case 2:  
                    GET(commands,system,first_user);
                    break;  
                case 3:
                    PUT(commands,system,first_user);       
                case 4:
                    DELETE(commands, system, first_user);
            }    
        }
        int port = argc > 1 ? std::stoi(argv[1]) : 5000;
        Server server(port);
        mapServerPaths(server);
        std::cout << "Server running on port: " << port << std::endl;
        server.run();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const Server::Exception& e) {
        std::cerr << e.getMessage() << std::endl;
    }
    return 0;
}