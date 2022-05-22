#pragma once
#include <iostream>
#include <Windows.h>
#include "String.h"
#include <string>
using namespace std;

class Exception
{
private:
	string error_code; //номер ошибки
public:
	Exception(string code = "0");
	string get_error_code();
};

