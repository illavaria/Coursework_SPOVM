#pragma once
#include <iostream>
using namespace std;
class String
{
private:
	char* str;
public:
	String();
	String(const char s[]);
	String(const String& obj);
	~String();
	void output();
	int length(); //возвращает длину строки
	String operator+(String obj2);
	String operator+(const char s[]);
	const char* cstr();
	string turn_str();
	friend String operator+(const char s[], String obj2);
	void operator=(String obj);
	bool operator==(String obj);
	bool operator!=(String obj);
	void operator+=(String obj2);
	void operator++();
	String operator()(int a1, int a2);
	bool operator<(String obj);
	bool operator>(String obj);
	char operator[](int n);
	friend ostream& operator<<(ostream& stream, const String& obj);
	friend istream& operator>>(istream& stream, String& obj);
};
void add(char s1[], const char s2[], int i);
int count(const char s[]);
