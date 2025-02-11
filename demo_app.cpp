#include <iostream>
#include "message_manager_lib.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <atomic>

using namespace std;

queue <pair<message, string>> message_queue;
mutex queue_mutex;
condition_variable queue_cv;
atomic<bool> running(true); //флаг завершения


void message_writer(message_manager& msg_manager){
	while(running || !message_queue.empty()){
		unique_lock<mutex> lock(queue_mutex);
		queue_cv.wait(lock, [] {return !message_queue.empty() || !running;});

		while(!message_queue.empty()){
			string operation_type = message_queue.front().second;
			if(operation_type == "print"){
				message msg = message_queue.front().first;
				message_queue.pop();
				lock.unlock();
				msg_manager.print(msg);
				lock.lock();
			}else if(operation_type == "changefile"){
				string new_file_name = message_queue.front().first.get_text();
				message_queue.pop();
				try{
					lock.unlock();
					msg_manager.set_file_name(new_file_name);
					lock.lock();
					cout<<"filename was changed successfully"<<endl;
					}catch (const std::runtime_error &e){
						cout<<e.what()<<endl;
					}
			}else if(operation_type == "changeimportance"){
				string new_str_importance = message_queue.front().first.get_text();
				message_queue.pop();
				try{
					importance new_importance = to_importance(new_str_importance);
					lock.unlock();
					msg_manager.set_base_importance(new_importance);
					lock.lock();
					cout<<"importance was changed successfully"<<endl;
				}catch(std::runtime_error& e){
					cout<<e.what()<<endl;
				}
			}
		}
	}
}

void help(){
	cout<<"--------MESSAGE MANAGER COMMANDS--------"<<endl<<endl;
	cout<<"   end program:                              -exit                                       "<<endl;
	cout<<"   change file to filename:                  -changefile filename                        "<<endl;
	cout<<"   change base importance level:             -changeimportance new_importance            "<<endl;
	cout<<"   write message with importance level:      -msg some text | importance                 "<<endl;
	cout<<"   write message with no importance level:   -msg some text                              "<<endl;
	cout<<"   display this text:                        -help                      					"<<endl;
	cout<<"   importance levels:                        low | medium | high              		    "<<endl<<endl;
}


int main(){
	string filename, importance_level_st;
	importance importance_level;
	message_manager msg_manager;
	help();
	//в цикле принимаем входные данные пока все не заработает
	while(true){
		cout<<"Enter output file name: ";
		cin>>filename;
		cout<<endl<<"Enter message base importance level: ";
		cin>>importance_level_st;
		try{
			importance_level = to_importance(importance_level_st);
			msg_manager.set_file_name(filename);
			msg_manager.set_base_importance(importance_level);
			break;
		}catch (const std::runtime_error& e){
			cout<<e.what()<<endl;
		}
	}

	thread msg_writer_thread(message_writer, std::ref(msg_manager)); //поток на запись
	help();
	while(true){
		string input;
		getline(cin, input);

		if(input == "-exit"){
			running = false;
			queue_cv.notify_all();
			msg_writer_thread.join();
			break;
		}

		if(input == "-help") help();

		if(input.substr(0, 12) =="-changefile "){
			string new_file_name = input.substr(12);
			message msg(new_file_name);
			{
				std::lock_guard<std::mutex> lock(queue_mutex); //добавление в очередь
				message_queue.push({msg, "changefile"});
			}
			queue_cv.notify_one();

		}

		if(input.substr(0, 18)=="-changeimportance "){
			string new_importance = input.substr(18);
			message msg(new_importance);
			{
				std::lock_guard<std::mutex> lock(queue_mutex); //добавление в очередь
				message_queue.push({msg, "changeimportance"});
			}
			queue_cv.notify_one();
		}

		if(input.substr(0, 5)=="-msg "){
			string text_msg;
			string msg_importance_str;

			if(input.find('|') != std::string::npos){
				int separation_index = input.find('|');
				text_msg = input.substr(5, separation_index-6);
				msg_importance_str = input.substr(separation_index+2);

				try{
					importance importance_msg = to_importance(msg_importance_str);
					message msg(text_msg, importance_msg);
					{
						std::lock_guard<std::mutex> lock(queue_mutex); //добавление в очередь
						message_queue.push({msg, "print"});
					}
					queue_cv.notify_one();
				}catch (std::runtime_error& e){
					cout<<e.what()<<endl;
				}


			}else{
				text_msg = input.substr(5);
				try{
					message msg(text_msg);
					{
						std::lock_guard<std::mutex> lock(queue_mutex); //добавление в очередь
						message_queue.push({msg, "print"});
					}
					queue_cv.notify_one();
				}catch (std::runtime_error& e){
					cout<<e.what()<<endl;
				}
		}

	}




	}
}
