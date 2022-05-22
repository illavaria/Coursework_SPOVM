#pragma once
#include "String.h"
#include "InputException.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
class Value
{
private:
	String name;
	DWORD type;
	BYTE* data;
	DWORD data_size;
public:
	Value();
	Value(String name1, DWORD type1, BYTE* data1, DWORD data_size1);
	Value(const Value& obj);
	~Value();
	String get_name();
	DWORD get_type();
	BYTE* get_data();
	DWORD get_data_size();
	friend ostream& operator<<(ostream& stream, Value& obj);
	void output_num_data();
	void output_bin_data();
	void output_string_data();
	friend istream& operator>>(istream& stream, Value& obj);
};
int convert_letter_to_numb(char b);

