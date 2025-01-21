#ifndef MESSAGE_MANAGER_LIB_H
#define MESSAGE_MANAGER_LIB_H

#include <fstream>
#include <string>
#include <optional>
#include <chrono>

using namespace std;

//----------------- класс важности вспомогательные функции для него----------

enum class importance{low, medium, high};
string to_string(importance im);
importance to_importance(string st);

//----------------- класс сообщения хранит текст, время и важность-----------

class message{
private:
	string msg_text;
	chrono::system_clock::time_point time;
	optional<importance> msg_importance;
public:
	message(string text, optional<importance> importance = std::nullopt);
	inline chrono::system_clock::time_point get_time() const {return time;}
	inline optional<importance> get_importance() const {return msg_importance;}
	inline string get_text() const {return msg_text;}
};

//----------------- класс менеджера сообщений хранит выходной файл и важность по умолчанию-----------

class message_manager{
private:
	ofstream output;
	importance msg_base_importance;

public:
	message_manager(string name, importance base_importance);
	message_manager();
	~message_manager();
	void set_file_name(string new_name);
	void set_base_importance(importance new_importance);
	void print(message msg);
};

#endif
