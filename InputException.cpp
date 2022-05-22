#include "InputException.h"
InputException::InputException(string code, string message1) :Exception(code), message(message1)
{}

string InputException::get_message()
{
	return message;
}

int input_int_limited(istream& stream, int value1, int value2)
{
	while (true)
	{
		try {
			int i;
			stream.unsetf(ios::skipws);
			cout << "Введите целое число oт " << value1 << " до " << value2 << ": ";
			stream >> i;
			if (stream.good() && stream.peek() == '\n')
			{
				rewind(stdin);
				if (i >= value1 && i <= value2)
					return i;
				throw InputException("1", "Введено число, выходящее за границы");
			}
			throw InputException("2", "Введено не целое число");
		}
		catch (InputException e)
		{
			rewind(stdin);
			stream.clear();
			cout << "Код ошибки: " << e.get_error_code() << " - " << e.get_message() << endl;
		}
	}
}

DWORD input_dword_value(istream& stream)
{
	while (true)
	{
		try {
			String str;
			cin.unsetf(ios::skipws);
			cout << "Введите значение типа DWORD(в 16-ричной с/с): ";
			cin >> str;
			if (check_num_value(str) && str.length() <= 8)
			{
				rewind(stdin);
				return (DWORD)stoi(str.turn_str(), 0, 16);
			}
			throw InputException("3", "Введена неверная строка, имеющая символы отличные от цифр 16-ой системы счисления, или строка, содержащая больше 8 символов");
		}
		catch (InputException e)
		{
			rewind(stdin);
			cin.clear();
			cout << "Код ошибки: " << e.get_error_code() << " - " << e.get_message() << endl;
		}
	}
}

DWORD64 input_qword_value(istream& stream)
{
	while (true)
	{
		try {
			String str;
			cin.unsetf(ios::skipws);
			cout << "Введите значение типа QWORD(в 16-ричной с/с): ";
			cin >> str;
			if (check_num_value(str) && str.length() <= 16)
			{
				rewind(stdin);
				return (DWORD64)stoll(str.turn_str(), 0, 16);
			}
			throw InputException("4", "Введена неверная строка, имеющая символы отличные от цифр 16-ой системы счисления, или строка, содержащая больше 16 символов");
		}
		catch (InputException e)
		{
			rewind(stdin);
			cin.clear();
			cout << "Код ошибки: " << e.get_error_code() << " - " << e.get_message() << endl;
		}
	}
}

String input_binary_value(istream& stream)
{
	while (true)
	{
		try {
			String str;
			cin.unsetf(ios::skipws);
			cout << "Введите значение типа BINARY: ";
			cin >> str;
			if (check_num_value(str))
			{
				rewind(stdin);
				return str;
			}
			throw InputException("5", "Введена строка, не соответствующая бинарному типу");
		}
		catch (InputException e)
		{
			rewind(stdin);
			cin.clear();
			cout << "Код ошибки: " << e.get_error_code() << " - " << e.get_message() << endl;
		}
	}
}

bool check_num_value(String str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f'))
			i++;
		else return false;
	}
	if (i != 0)
		return true;
	else return false;
}

bool check_binary_value(String str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '0' || str[i] == '1')
			i++;
		else return false;
	}
	if (i != 0)
		return true;
	else return false;
}

