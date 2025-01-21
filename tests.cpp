#include <iostream>
#include "message_manager_lib.h"

bool construcor_bad_file_test(){
	try{
	message_manager test2("./bad_file.txt", importance::low);
	cout<<"CONSTRUCTOR BAD FILE TEST NOT PASSED"<<endl;
	return false;
	}catch(const std::runtime_error &e){
		cout<<"CONSTRUCTOR_BAD_FILE_TEST PASSED"<<endl;
		return true;
	}
}

bool set_new_file_test(){
	try{
	message_manager test1("journal.txt",  importance::low);
	test1.set_file_name("./bad_file.txt");
	cout<<"SET_NEW_FILE TEST NOT PASSED"<<endl;
	return false;
	}catch(const std::runtime_error &e){
		cout<<"SET_NEW_FILE_TEST PASSED"<<endl;
		return true;
	}
}

bool bad_importance_test(){
	try{
		to_importance("aboba");
		cout<<"BAD_IMPORTANCE_TEST NOT PASSED";
		return false;
	}catch(const std::runtime_error &e){
		cout<<"BAD_IMPORTANCE_TEST PASSED"<<endl;
		return true;
	}
}

int main(){
	int counter=0, summary_count=3;
	counter+=construcor_bad_file_test();
	counter+=set_new_file_test();
	counter+=bad_importance_test();
	cout<<"PASSED "<<counter<<" TESTS OF "<<summary_count<<endl;
}
