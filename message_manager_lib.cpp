#include "message_manager_lib.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <optional>
using namespace std;

//-------реализация функций для класса важности--------

string to_string(importance im){
	switch (im){
		case importance::low: return "low";
		case importance::medium: return "medium";
		case importance::high: return "high";
	}
	return "unknown";
}

importance to_importance(string st){
	if(st == "low")return importance::low;
	if(st == "medium")return importance::medium;
	if(st == "high")return importance::high;
	throw runtime_error("Importance level " +st + " doesn't exist");
}

//-------определение методов сообщений--------

message::message(string text, optional<importance> importance){
	msg_text = text;
	msg_importance = importance;
	time = chrono::system_clock::now();
}

//-------определение методов менеджера сообщений--------

message_manager::message_manager(string name, importance base_importance){ //чтобы проверить существование файла открываем поток на запись
	if(!ifstream(name)) throw runtime_error("File " + name + " cannot be opened");
	output.open(name, ios::app);
	msg_base_importance = base_importance;
}

message_manager::message_manager(){
	msg_base_importance = importance::low;
	ofstream output;
}

message_manager::~message_manager(){
	if(output.is_open())output.close();
}

void message_manager::set_file_name(string new_name){
	if(!ifstream(new_name)) throw runtime_error("File " +new_name + " cannot be opened");
	if(output.is_open())output.close();
	output.open(new_name, ios::app);
}

void message_manager::set_base_importance(importance new_importance){
	msg_base_importance = new_importance;
}

void message_manager::print(message msg){
	 importance msg_importance;
	 if(!msg.get_importance().has_value())
		 msg_importance = msg_base_importance;
	 else
		 msg_importance = msg.get_importance().value();
	 //importance msg_importance = (msg.get_importance()==std::nullopt) ? msg_base_importance : msg.get_importance();
	 if(msg_importance >= msg_base_importance){
		 output<<"Message: "<<msg.get_text()<<" | importance_level: "<<to_string(msg_importance)<<" | time: ";
		 auto time = msg.get_time();
		 time_t print_time = chrono::system_clock::to_time_t(time);
		 output << ctime(&print_time)<<endl;
	 }
}


