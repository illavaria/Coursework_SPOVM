#include "String.h"

String::String()
{
	str = nullptr;
}

String::String(const char s[])
{
	str = new char[count(s) + 1];
	add(str, s, 0);
}

String::String(const String& obj)
{
	str = new char[count(obj.str) + 1];
	add(str, obj.str, 0);
}

String::~String()
{
	delete[] str;
	str = nullptr;
}

void String::output()
{
	if (str != nullptr)
		cout << str;
	return;
}

int String::length()   //считает длину строки без \0
{
	if (str == nullptr)
		return 0;
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}

String String::operator+(String obj2)
{
	char* tempStr = new char[this->length() + obj2.length() + 1];
	add(tempStr, this->str, 0);
	add(tempStr, obj2.str, length());
	String obj(tempStr);
	return obj;
}

String String::operator+(const char s[])
{
	int j = 0;
	while (s[j] != '\0')
	{
		j++;
	}
	char* tempStr = new char[this->length() + j + 1];
	add(tempStr, this->str, 0);
	add(tempStr, s, length());
	String obj(tempStr);
	return obj;
}

const char* String::cstr()
{
	return str;
}

string String::turn_str()
{
	string s = str;
	return s;
}

void String::operator=(String obj)
{
	delete[] str;
	str = new char[obj.length() + 1];
	add(str, obj.str, 0);
}

bool String::operator==(String obj)
{
	if (strcmp(str, obj.str))
		return false;
	else return true;
}

bool String::operator!=(String obj)
{
	if (strcmp(str, obj.str))
		return true;
	else return false;
}

void String::operator+=(String obj2)
{
	*this = *this + obj2;
}

void String::operator++()
{
	int l = length();
	if (str != nullptr)
		for (int i = 0; i < l; i++)
			str[i] = str[i] + 1;
}

String String::operator()(int a1, int a2)
{
	int i = 0;
	if (a2 >= a1 && a1 < length())
	{
		char* tempStr = new char[a2 - a1 + 1];

		for (a1; a1 <= a2; a1++)
		{
			tempStr[i] = str[a1];
			i++;
		}
		tempStr[i] = '\0';
		String obj(tempStr);
		return obj;
	}
	char* tempStr = new char[1];
	tempStr[0] = '\0';
	String obj(tempStr);
	return obj;
}

bool String::operator<(String obj)
{
	int i = 0;
	if (*this == obj)
		return false;
	while (str[i] && obj.str[i])
	{
		if (str[i] > obj.str[i])
			return false;
		else if (str[i] < obj.str[i])
			return true;
		i++;
	}
	if (str[i] != '\0' && obj.str[i] == '\0')
		return false;
	else return true;
}
bool String::operator>(String obj)
{
	int i = 0;
	if (*this == obj)
		return false;
	while (str[i] && obj.str[i])
	{
		if (str[i] < obj.str[i])
			return false;
		else if (str[i] > obj.str[i])
			return true;
		i++;
	}
	if (str[i] == '\0' && obj.str[i] != '\0')
		return false;
	else return true;
}

char String::operator[](int n)
{
	if (n < length())
		return str[n];
	else {
		//cout << "в стороке меньше символов" << endl;
		return 0;
	}
}

String operator+(const char s[], String obj2)
{
	int j = 0;
	while (s[j] != '\0')
	{
		j++;
	}
	char* tempStr = new char[obj2.length() + j + 1];
	add(tempStr, s, 0);
	add(tempStr, obj2.str, j);
	String obj(tempStr);
	return obj;

}

ostream& operator<<(ostream& stream, const String& obj)
{
	stream << obj.str;
	return stream;
}

istream& operator>>(istream& stream, String& obj)
{
	char* tempStr = new char[256];
	cin.getline(tempStr, 256, '\n');
	delete obj.str;
	obj.str = new char[count(tempStr) + 1];
	add(obj.str, tempStr, 0);
	return stream;
}


void add(char s1[], const char s2[], int i)
{
	if (s2 != nullptr)
	{
		int j = 0;
		while (s2[j] != '\0')
		{
			s1[j + i] = s2[j];
			j++;
		}
		s1[j + i] = '\0';
	}
	return;
}

int count(const char s[])
{
	if (s == nullptr)
		return 0;
	int i = 0;
	while (s[i] != '\0')
		i++;
	return i;
}

