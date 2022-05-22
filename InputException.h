#pragma once
#include "Exception.h"

class InputException : public Exception {
private:
	string message; //сообщение
public:
	InputException(string code = "1", string message1 = "Ошибка ввода");
	string get_message();
};

int input_int_limited(istream& stream, int value1, int value2);
DWORD input_dword_value(istream& stream);
DWORD64 input_qword_value(istream& stream);
String input_binary_value(istream& stream);
bool check_num_value(String str);
bool check_binary_value(String str);
//String input_name_value(istream& stream);