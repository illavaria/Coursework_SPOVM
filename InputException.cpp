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
			cout << "������� ����� ����� o� " << value1 << " �� " << value2 << ": ";
			stream >> i;
			if (stream.good() && stream.peek() == '\n')
			{
				rewind(stdin);
				if (i >= value1 && i <= value2)
					return i;
				throw InputException("1", "������� �����, ��������� �� �������");
			}
			throw InputException("2", "������� �� ����� �����");
		}
		catch (InputException e)
		{
			rewind(stdin);
			stream.clear();
			cout << "��� ������: " << e.get_error_code() << " - " << e.get_message() << endl;
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
			cout << "������� �������� ���� DWORD(� 16-������ �/�): ";
			cin >> str;
			if (check_num_value(str) && str.length() <= 8)
			{
				rewind(stdin);
				return (DWORD)stoi(str.turn_str(), 0, 16);
			}
			throw InputException("3", "������� �������� ������, ������� ������� �������� �� ���� 16-�� ������� ���������, ��� ������, ���������� ������ 8 ��������");
		}
		catch (InputException e)
		{
			rewind(stdin);
			cin.clear();
			cout << "��� ������: " << e.get_error_code() << " - " << e.get_message() << endl;
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
			cout << "������� �������� ���� QWORD(� 16-������ �/�): ";
			cin >> str;
			if (check_num_value(str) && str.length() <= 16)
			{
				rewind(stdin);
				return (DWORD64)stoll(str.turn_str(), 0, 16);
			}
			throw InputException("4", "������� �������� ������, ������� ������� �������� �� ���� 16-�� ������� ���������, ��� ������, ���������� ������ 16 ��������");
		}
		catch (InputException e)
		{
			rewind(stdin);
			cin.clear();
			cout << "��� ������: " << e.get_error_code() << " - " << e.get_message() << endl;
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
			cout << "������� �������� ���� BINARY: ";
			cin >> str;
			if (check_num_value(str))
			{
				rewind(stdin);
				return str;
			}
			throw InputException("5", "������� ������, �� ��������������� ��������� ����");
		}
		catch (InputException e)
		{
			rewind(stdin);
			cin.clear();
			cout << "��� ������: " << e.get_error_code() << " - " << e.get_message() << endl;
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

