#include "Exception.h"

Exception::Exception(string code) : error_code(code)
{}

string Exception::get_error_code()
{
	return error_code;
}