#include <iostream>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;

void* get_file_chunk(void* arg){
    // Extracting arguments from arg
    logger("getting file chunk fjlakj " + string((char*) arg));
    vector<string> tokenised_arg = tokenise((char*) arg);
    // for(auto token:tokenised_arg){
    //     logger(token);
    //     cout<<token<<endl;
    // }
    logger("command size " + to_string(tokenised_arg.size()));
    long long chunk_index = stoll(tokenised_arg[0]);
    int chunk_size = stoi(tokenised_arg[1]);
    int sock = stoi(tokenised_arg[2]);
    string file_path = tokenised_arg[3];

    // Sending request command to tracker
    char* command = new char();
    strcat(command, to_string(chunk_index).c_str());
    strcat(command, (" " + to_string(CHUNK_SIZE)).c_str());
    strcat(command, (" " + to_string(chunk_size)).c_str());
    strcat(command, (" " + file_path).c_str());
    send(sock, command, strlen(command), 0);
    logger("command " + string(command));

    // Reading response from the peer
    char* response = new char[2* CHUNK_SIZE];
    int char_count = read(sock, response, 2* CHUNK_SIZE);
    // logger("response " + string(response));
    if(char_count > 0){
        // Process the response from the tracker
        return (void*) response;
    }
    else{
        char* msg;
        strcat(msg, string("-1").c_str());
        return (void*)msg;
    }
}

void* is_user_available(void* void_ip_port){
    // Extracting peer ip and peer port from void_ip_port
    logger("thread called for checking connection");
    string ip_port = (const char*)void_ip_port;
    logger("ip_port " + ip_port);
    int k=0, len = ip_port.size();
    while(ip_port[k] != '_') k++;
    string peer_IP = ip_port.substr(0,k);
    int peer_Port = stoi(ip_port.substr(k+1,len-k-1));
    logger(peer_IP);
    logger(to_string(peer_Port));

    int sock = 0;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		logger("Socket creation error");
        int * msg = new int(-1);
		return (void*) msg;
	}

    struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(peer_Port);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, peer_IP.c_str(), &serv_addr.sin_addr)
		<= 0) {
		logger("Invalid address/ Address not supported");
        // char* msg = "fail Invalid address/ Address not supported";
        int * msg = new int(-1);
		return (void*)msg;
	}
    int peer_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (peer_fd < 0) {
		logger("Connection Failed");
        // char* msg = "fail Connection Failed";
        int * msg = new int(-1);
		return (void*)msg;
	}
    else{
        // char* msg = "success " + to_string(sock) + " " + ip_port + " is available"
        logger("success in making connection");
        int * msg = new int(sock);
        return (void*) msg;
    }
}

void get_file_from_peers(vector<string> tokenised_command, vector<string> tokenised_response){

    // Extracting information from tokenised command
    string group_name = tokenised_command[1];
    string file_name = tokenised_command[2];
    string destination_path = tokenised_command[3];

    // Extracting information from tokenised response
    string chunk_size = tokenised_response[1];
    string file_size = tokenised_response[2];
    string file_sha = tokenised_response[3];
    string file_hash_protocol = tokenised_response[4];
    int users_count = stoi(tokenised_response[5]);
    vector<string> users_ip_port;
    vector<string> file_paths;
    for(int i=6; i< 6 + users_count; i++){
        if(tokenised_response[i].size() != 0) users_ip_port.push_back(tokenised_response[i]);
        else break;
    }

    for(int i=6+users_count; i< 6+2*(users_count); i++){
        if(tokenised_response[i].size() != 0) file_paths.push_back(tokenised_response[i]);
        else break;
    }

    // Creating an empty file to write data
    if(destination_path[destination_path.size()-1] != '/') destination_path.push_back('/');
    string file_path = destination_path + file_name;
    ofstream file(file_path);
    if(!file.is_open()){
        logger("Can't open file at this location");
        // char* msg;
        // strcat(msg, string("Can't download file at this location").c_str());
        // return msg;
        return;
    }


    // Checking if the peers who have the file are online in different threads
    // int users_count = users_ip_port.size();
    vector<pthread_t> threads(users_count);
    for(int i=0; i<users_count; i++){
        logger("checking for user availability " + users_ip_port[i]);
        const char* ip_port = users_ip_port[i].c_str();
        pthread_create(&threads[i], NULL, &is_user_available, (void*)ip_port);
    }

    // Joining threads and storing the sock of the connection in socks vector
    vector<int> socks;
    for (int i = 0; i < users_count; ++i) {
        void* result;
        pthread_join(threads[i], &result);
        logger("joining threads ");
        int* msg = (int*) result;
        int sock = *msg;
        if(sock < 0){ 
            logger(users_ip_port[i] + " not found online");
            users_ip_port.erase(users_ip_port.begin() + i);
            file_paths.erase(file_paths.begin() + i);
        }
        else socks.push_back(sock);
    }
    string str_socks = "";
    for(auto a:socks){
        str_socks += to_string(a) + " ";
    }
    logger("socks " + str_socks);

    // Getting the file chunks in different thread
    long long chunks_count = ceil(stol(file_size)/(CHUNK_SIZE*1.0));
    logger("file_size" + file_size);
    logger("chunks count" + to_string(chunks_count));
    threads.clear();
    int thread_pool_size = 10;
    threads.resize(thread_pool_size)
    for(long long i=0; i<chunks_count; i++){
        int index = rand()%(socks.size());

        // updating the chunk size if its the last chunk of file
        int chunk_size = CHUNK_SIZE;
        if(i==chunks_count-1) chunk_size = stol(file_size) - (long long)CHUNK_SIZE*(chunks_count-1);

        // combining the arguments required for geting file chunk
        char* arg = new char();
        strcat(arg, to_string(i).c_str());
        strcat(arg, (" " + to_string(chunk_size)).c_str());
        strcat(arg, (" " + to_string(socks[index])).c_str());
        strcat(arg, (" " + file_paths[index]).c_str());
        logger("getting file chunk " + string(arg));
        void* void_arg = (void*)arg;
        // vector<string> tokenised_arg = tokenise((char*) void_arg);
        // for(auto token:tokenised_arg){
        //     logger(token);
        //     cout<<token<<endl;
        // }
        // logger("getting file chunk fs " + string((char*) void_arg));
        pthread_create(&threads[i%thread_pool_size], NULL, &get_file_chunk, void_arg);
        this_thread::sleep_for(std::chrono::milliseconds(10));

        // if(i==5) break;


        if(i%thread_pool_size==thread_pool_size-1 || i==chunks_count-1){
            // Joining threads and storing the sock of the connection in socks vector
            for (long long j=0; j<=i%thread_pool_size; j++) {
                void* result;
                pthread_join(threads[j], &result);
                logger("joining threads to combine file chunks");
                char* msg = (char*) result;

                // Extracting information from msg
                vector<string> tokenised_msg = tokenise(msg);
                long long chunk_index = stoll(tokenised_msg[0]);
                int chunk_size = stoi(tokenised_msg[1]);
                int header_size = tokenised_msg[0].size() + tokenised_msg[1].size() + 2;
                string str_file_chunk = string(msg).substr(header_size); 
                char* file_chunk = new char();
                strcat(file_chunk, str_file_chunk.c_str());


                // Checking whether chunk is received or not
                if(chunk_index >= 0){
                    long long chunk_start_position = (long long)CHUNK_SIZE * chunk_index;
                    file.seekp(chunk_start_position, ios::beg);
                    file.write(file_chunk, chunk_size);
                }
                else{
                    logger("chunk error" + to_string(chunk_index));
                }
            }
        }

    }

    // Closing the file
    file.close();

    // Terminating the connections from all the peers
    for(auto sock:socks){
        close(sock);
    }

    // Returning success msg
    // char* msg;
    // strcat(msg, string("File downloaded successfully").c_str());
    logger("File downloaded successfully");
    // return msg;
    return;
}