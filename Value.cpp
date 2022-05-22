#include "Value.h"

Value::Value()
{
	name = "";
	type = 0;
	data = NULL;
	data_size = 0;
}

Value::Value(String name1, DWORD type1, BYTE* data1, DWORD data_size1)
{
	name = name1;
	type = type1;
	//	delete[] data;
	data = new BYTE[data_size1];
	data = data1;
	data_size = data_size1;
}

Value::Value(const Value& obj)
{
	name = obj.name;
	type = obj.type;
	delete[] data;
	data = new BYTE[obj.data_size];
	data = obj.data;
	data_size = obj.data_size;
}

Value::~Value()
{}

String Value::get_name()
{
	return name;
}

DWORD Value::get_type()
{
	return type;
}

BYTE* Value::get_data()
{
	return data;
}

DWORD Value::get_data_size()
{
	return data_size;
}

ostream& operator<<(ostream& stream, Value& obj)
{
	cout << setw(42) << obj.name << "|" << setw(13);
	switch (obj.type)
	{
	case REG_DWORD: {
		cout << "REG_DWORD" << "|";
		obj.output_num_data();
	}break;
	case REG_QWORD: {
		cout << "REG_QWORD" << "|";
		obj.output_num_data();
	}break;
	case REG_BINARY: {
		cout << "REG_BINARY" << "|";
		obj.output_bin_data();
	}break;
	case REG_SZ: {
		cout << "REG_SZ" << "|";
		obj.output_string_data();
	}break;
	case REG_MULTI_SZ: {
		cout << "REG_MULTI_SZ" << "|";
		obj.output_string_data();
	}break;
	case REG_EXPAND_SZ: {
		cout << "REG_EXPAND_SZ" << "|";
		obj.output_string_data();
	}break;
	default: {
		cout << obj.type << "|";
		obj.output_string_data();
	}
		   break;
	}
	return stream;
}

istream& operator>>(istream& stream, Value& obj)
{
	cout << endl << "Введите имя: ";
	cin >> obj.name;
	cout << "Выберите один из предложенных типов данных:" << endl << "1-REG_DWORD" << endl << "2-REG_QWORD" << endl << "3-REG_BINARY"
		<< endl << "4-REG_SZ" << endl << "5-REG_EXPAND_SZ" << endl;
	int c = input_int_limited(stream, 1, 5);
	switch (c)
	{
	case 1: {
		DWORD dword_value;
		obj.type = REG_DWORD;
		dword_value = input_dword_value(stream);
		obj.data = new BYTE[sizeof(dword_value)];
		obj.data_size = sizeof(dword_value);
		memcpy(obj.data, &dword_value, 4);
	}break;
	case 2: {
		DWORD64 qword_value;
		obj.type = REG_QWORD;
		qword_value = input_qword_value(stream);
		obj.data = new BYTE[sizeof(qword_value)];
		obj.data_size = sizeof(qword_value);
		memcpy(obj.data, &qword_value, 8);
	}break;
	case 3: {
		String binary_value;
		obj.type = REG_BINARY;
		binary_value = input_binary_value(stream);
		obj.data_size = binary_value.length() / 2;
		if (binary_value.length() % 2 != 0)
			obj.data_size++;
		obj.data = new BYTE[obj.data_size];
		for (int i = 0, j = 0; i < obj.data_size; i++, j += 2)
		{
			if (binary_value[j] <= '9')
				obj.data[i] = binary_value[j] - '0';
			else obj.data[i] = convert_letter_to_numb(binary_value[j]);
			if (j + 1 == binary_value.length())
				break;
			obj.data[i] *= 16;
			if (binary_value[j + 1] <= '9')
				obj.data[i] += binary_value[j + 1] - '0';
			else obj.data[i] += convert_letter_to_numb(binary_value[j + 1]);
		}
	}break;
	case 4: {
		String sz_value;
		cout  << "Введите значение:";
		cin >> sz_value;
		if (sz_value.length() == 0)
		{
			cout << endl << "Значение не может иметь нулевую длину";
			break;
		}
		obj.type = REG_SZ;
		obj.data_size = sz_value.length();
		obj.data = new BYTE[obj.data_size];
		for (int i = 0; i < obj.data_size; i++)
			obj.data[i] = (BYTE)sz_value[i];
	}break;
	case 5:
	{
		String esz_value;
		cout << "Введите значение:";
		cin >> esz_value;
		if (esz_value.length() == 0)
		{
			cout << endl << "Значение не может иметь нулевую длину";
			break;
		}
		obj.type = REG_EXPAND_SZ;
		obj.data_size = esz_value.length();
		obj.data = new BYTE[obj.data_size];
		for (int i = 0; i < obj.data_size; i++)
			obj.data[i] = (BYTE)esz_value[i];
	}break;
	default: break;
	}
	return stream;
}

int convert_letter_to_numb(char b)
{
	switch (b)
	{
	case 'a': return 10;
	case 'b': return 11;
	case 'c': return 12;
	case 'd': return 13;
	case 'e': return 14;
	case 'f': return 15;
	default:
		break;
	}
	return 0;
}

void Value::output_num_data()
{
	cout.unsetf(ios::left);
	cout << "0x";
	for (int j = data_size - 1; j >= 0; j--)
		cout << setfill('0') << setw(2) << hex << (int)data[j];
	cout << setfill(' ');
}

void Value::output_bin_data()
{
	cout.unsetf(ios::left);
	for (int j = 0; j < data_size; j++)
		cout << setfill('0') << setw(2) << hex << (int)data[j] << " ";
	cout << setfill(' ');
}

void Value::output_string_data()
{
	for (int j = 0; j < data_size; j++)
		cout << hex << data[j];
}
