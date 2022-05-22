#include <Windows.h>
#include <winreg.h>
#include <chrono> 
#include <ctime> 
#include <stack>
#include <list>
#include <WinBase.h>
#include "InputException.h"
#include "String.h"
#include "Value.h"
using namespace std;

HKEY root_keys[3] =
{
	HKEY_CURRENT_USER,
	HKEY_CLASSES_ROOT,
	HKEY_CURRENT_CONFIG,
};
String file_names[3] =
{
	"D:\\regfiles\\hkey_current_user.reg",
	"D:\\regfiles\\hkey_classes_root.reg",
	"D:\\regfiles\\hkey_current_config.reg",
};

BOOL SetPrivilege(
	HANDLE hToken,
	LPCTSTR lpszPrivilege,
	BOOL bEnablePrivilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,
		lpszPrivilege,
		&luid))
	{
		printf("������: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("������: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("���������� �� �������. \n");
		return FALSE;
	}
	return TRUE;
}

int output(HKEY hkey);
int open(stack <HKEY>& s_hkey, LPCSTR str);
int close(stack  <HKEY>& s_hkey);
int open_root_dir(stack <HKEY>& s_hkey, list <String>& current_dir);
int create_dir(stack <HKEY>& s_hkey, list <String>& current_dir, LPCSTR subkey);
int create_value(stack  <HKEY>& s_hkey);
int rename_dir(stack <HKEY>& s_hkey, list <String>& current_dir);
bool save_keys_to_files();
bool save_files_to_git();
bool restore_keys_from_files();
bool restore_files_from_git();

int main()
{
	setlocale(LC_ALL, "russian");
	int c = 0;
	stack <HKEY> s_hkey;
	String strr = "";
	String file_name = "D:\\regfile_1024.reg";
	list <String> current_dir;
	cout << endl << "***************************************************************************";
	cout << endl << "* ������ ���� ����-������������� ��� ������ ������� ��������� �����������, *"
		<< endl << "* �������� � ���� ����� ��� ������ �� ����,                                *"
		<< endl << "* ����� ��������� � ��������� ������ ������������ �������                  *"
		<< endl << "* � �������� ��� ���� �������.                                             *";
	cout << endl << "***************************************************************************" << endl;
	if (open_root_dir(s_hkey, current_dir))
		return 0;
	do {
		cout << endl << "������� ������: ";
		copy(current_dir.begin(), current_dir.end(), ostream_iterator<String>(cout, "/"));
		cout << endl << "�������� ��������: " << endl << "1-������� � ��������� ������" << endl << "2-��������� � ���������� ������" << endl << "3-������� ���������� �������" << endl << "4-��������� ��� ������� � ����" << endl
			<< "5-�������� ����� ��������� � ������� �������" << endl << "6-�������� ����� �������� � ������� ������� ��� �������� ��� �/��� ���������� ��� ������������� ��������" << endl << "7-������������� ������� ������"
			<< endl << "8-������� �������� � ������� �������" << endl << "9-������� ���������(�� ����� ��� ����������) � ������� �������"
			<< endl << "10-������� ���������� ��������� �������� ������� co ����� ��� ������������ � ��������" <<
			endl << "11-�������� ��� ������� ������� �� �����" << endl
			<< "0-����������" << endl;
		c = input_int_limited(cin, 0, 11);
		switch (c)
		{
		case 1: {
			if (s_hkey.empty())
			{
				open_root_dir(s_hkey, current_dir);
				break;
			}
			cout << "������� �������� �������: ";
			cin >> strr;
			if (open(s_hkey, strr.cstr()))
			{
				cout << endl << "�������� �������� �������" << endl;
				break;
			}
			current_dir.push_back(strr);
		}break;
		case 2: {
			if (!s_hkey.empty())
			{
				close(s_hkey);
				current_dir.pop_back();
			}
		}break;
		case 3: {
			if (s_hkey.empty())
			{
				cout << endl << "��������� � ����� ������ ��� ������ ��� �����������";
				break;
			}
			output(s_hkey.top());
		}break;
		case 4: {
			if (!save_keys_to_files())
				break;
			if (!save_files_to_git())
				cout << endl << "������ ���������� � ���";
		}break;
		case 5: {
			String dir_name;
			cout << "������� ��� ������������ �������:" << endl;
			cin >> dir_name;
			create_dir(s_hkey, current_dir, dir_name.cstr());
		}break;
		case 6: {
			create_value(s_hkey);
		}break;
		case 7: {
			rename_dir(s_hkey, current_dir);
		}break;
		case 8: {
			String value_name;
			cout << "������� ��� ���������� ��������: ";
			cin >> value_name;
			if (RegDeleteValueA(s_hkey.top(), value_name.cstr()) != ERROR_SUCCESS)
			{
				cout << endl << "������ �������� ��������: � ������� ������� ��� �������� � ����� ������";
				break;
			}
			cout << "�������� ������� �������";
		}break;
		case 9: {
			String subkey_name;
			cout  << "������� ��� ���������� ����������: ";
			cin >> subkey_name;
			if (RegDeleteKeyA(s_hkey.top(), subkey_name.cstr()) != ERROR_SUCCESS)
			{
				cout << "������ �������� ����������: � ������� ������� ��� ���������� � ����� ������ ��� � ���������� ���� ������ ����������";
				break;
			}
			cout << "��������� ������� �����";
		}break;
		case 10: {
			String subkey_name;
			cout << "������� ��� ����������: ";
			cin >> subkey_name;
			if (RegDeleteTreeA(s_hkey.top(), subkey_name.cstr()) != ERROR_SUCCESS)
			{
				cout << "������ ������������ �������� ����������� �������: � ������� ������� ��� ���������� � ����� ������";
				break;
			}
			cout << "��������� ������� �����";
		}break;
		case 11: {
			/*if (!restore_files_from_git())
			{
				cout << endl << "������ �������� ������ �� ����";
				break;
			}*/
			if (!restore_keys_from_files())
				break;
		}break;
		case 0: {
			while (!s_hkey.empty())
			{
				close(s_hkey);
				current_dir.pop_back();
			}
		}break;
		default:
			return 0;
		}
	} while (c != 0);
	return 0;
}

int output(HKEY hkey)
{
	char buff[255];
	int i = 0;
	DWORD blen = 255;
	cout << endl << "����������:";
	cout << endl << "_______________________________________________________________________________________________________________________";
	cout << endl << " �  |���";
	cout << endl << "_______________________________________________________________________________________________________________________";
	while (1)
	{
		blen = 255;
		if (RegEnumKeyExA(hkey, static_cast<DWORD>(i), buff, &blen, NULL, NULL, NULL, NULL) == ERROR_NO_MORE_ITEMS)
			break;
		cout << dec << endl << setw(2) << i << setw(3) << "|" << buff;
		cout << endl << "_______________________________________________________________________________________________________________________";
		i++;
	}
	i = 0;

	DWORD number_subkeys = 0;
	DWORD number_values = 0;
	DWORD max_name = 0;
	DWORD max_data = 0;
	RegQueryInfoKeyA(hkey, NULL, NULL, NULL, &number_subkeys, NULL, NULL, &number_values, &max_name, &max_data, NULL, NULL);

	char* buff_name;
	if (!(buff_name = (char*)malloc(max_name * sizeof(char))))
	{
		cout << "error malloc";
		return 1; //error 
	}
	DWORD buff_type = 0;
	BYTE* buff_data;
	if (!(buff_data = (BYTE*)malloc(max_data * sizeof(BYTE))))
	{
		cout << "error malloc";
		return 1;
	}
	DWORD buff_data_size = max_data;
	RegEnumValueA(hkey, static_cast<DWORD>(i), buff_name, &blen, NULL, &buff_type, buff_data, &buff_data_size);
	cout << endl << endl << "��������:";
	cout << endl << "_______________________________________________________________________________________________________________________";
	cout << endl << "�    |���                                       |���          |��������";
	cout << endl << "_______________________________________________________________________________________________________________________";
	while (1)
	{
		buff_data_size = max_data;
		blen = 255;
		if (RegEnumValueA(hkey, static_cast<DWORD>(i), buff_name, &blen, NULL, &buff_type, buff_data, &buff_data_size) == ERROR_NO_MORE_ITEMS)
			break;
		Value temp_value(buff_name, buff_type, buff_data, buff_data_size);

		cout << dec << endl << left << setw(5) << i << "|";
		cout << temp_value;
		cout << endl << "_______________________________________________________________________________________________________________________" << dec;
		i++;
	}
	return 0; // no errors
}

int open(stack <HKEY>& s_hkey, LPCSTR str)
{
	HKEY hkey;
	hkey = s_hkey.top();
	if (str == NULL)
		s_hkey.pop();
	if (RegOpenKeyExA(hkey, str, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY | KEY_QUERY_VALUE | KEY_WRITE, &hkey) != ERROR_SUCCESS) //HKEY_CURRENT_USER, "Console" Volatile Environment
	{
		cout << endl << "������ �������� �����";
		return 1;
	}
	s_hkey.push(hkey);
	return 0;
}

int close(stack<HKEY>& s_hkey)
{
	HKEY hkey;
	hkey = s_hkey.top();
	s_hkey.pop();
	RegCloseKey(hkey);
	return 0;
}

int open_root_dir(stack<HKEY>& s_hkey, list <String>& current_dir)
{
	int c;
	HKEY hkey;
	cout << "�������� �������� ������: " << endl << "1-HKEY_CLASSES_ROOT" << endl << "2-HKEY_CURRENT_USER" << endl << "3-HKEY_LOCAL_MACHINE" << endl << "4-HKEY_USERS" << endl << "5-HKEY_CURRENT_CONFIG" << endl << "0-����������" << endl;
	c = input_int_limited(cin, 0, 5);
	switch (c)
	{
	case 1: {
		hkey = HKEY_CLASSES_ROOT;
		current_dir.push_back("HKEY_CLASSES_ROOT");
	}break;
	case 2: {
		hkey = HKEY_CURRENT_USER;
		current_dir.push_back("HKEY_CURRENT_USER");
	}break;
	case 3: {
		hkey = HKEY_LOCAL_MACHINE;
		current_dir.push_back("HKEY_LOCAL_MACHINE");
	}break;
	case 4: {
		hkey = HKEY_USERS;
		current_dir.push_back("HKEY_USERS");
	}break;
	case 5:
	{
		hkey = HKEY_CURRENT_CONFIG;
		current_dir.push_back("HKEY_CURRENT_CONFIG");
	}break;
	default:
		return 1;
	}
	s_hkey.push(hkey);
	if (open(s_hkey, NULL))
		current_dir.pop_back();
	return 0;
}

int create_dir(stack<HKEY>& s_hkey, list<String>& current_dir, LPCSTR subkey)
{
	HKEY hkey;
	DWORD disposition;
	if (RegCreateKeyExA(s_hkey.top(), subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &disposition) != ERROR_SUCCESS)
	{
		TCHAR* buf = new TCHAR[128];
		cout << endl << "������ �������� ����������";
		return 1;
	}
	if (disposition == REG_CREATED_NEW_KEY)
		cout << "����� ��������� �������� �������";
	else cout << "��������� � ����� ������ ��� ���������� � ��� ������";
	s_hkey.push(hkey);
	current_dir.push_back(subkey);
	return 0;
}

int create_value(stack <HKEY>& s_hkey)
{
	Value new_value;
	cin >> new_value;
	if (RegSetValueExA(s_hkey.top(), new_value.get_name().cstr(), 0, new_value.get_type(), new_value.get_data(), new_value.get_data_size()) != ERROR_SUCCESS)
	{
		cout << endl << "������ ��������/��������� ��������";
		return 1;
	}
	cout << endl << "�������� �������/�������� �������";
	return 0;
}

int rename_dir(stack<HKEY>& s_hkey, list<String>& current_dir)
{
	String new_name;
	size_t size = 0;
	wchar_t name[255];
	cout << "������� ����� �������� �������: ";
	cin >> new_name;
	mbstowcs_s(&size, name, new_name.cstr(), new_name.length() + 1);
	if (RegRenameKey(s_hkey.top(), NULL, name) != ERROR_SUCCESS)
	{
		cout << endl << "������ �������������� �������";
		return 1;
	}
	cout << "������ "<<current_dir.back() << " ��� ������� ������������ � " << new_name;
	current_dir.pop_back();
	current_dir.push_back(new_name);
	return 0;
}

bool save_keys_to_files()
{
	HANDLE ProcessToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ProcessToken))
	{
		cout << endl << "error open process token";
		return false;
	}
	SetPrivilege(ProcessToken, SE_BACKUP_NAME, TRUE);
	for (int i = 0; i < 3; i++)
	{
		if (remove(file_names[i].cstr()))
		{
			//cout << endl << "������ �������� ������� �����";
			//return false;
		}
		if (RegSaveKeyA(root_keys[i], file_names[i].cstr(), NULL) != ERROR_SUCCESS)
		{
			cout << endl << "������ ������ � ����";
			return false;
		}
		cout << endl << "������ " << root_keys[i] << " ������� ������� � ���� " << file_names[i];
	}
	return true;
}

bool save_files_to_git()
{
	char buf[255];
	time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	ctime_s(buf, 255, &time);
	cout << endl;
	system("cd D:\\regfiles\\ && git add .");
	string commit_script = "cd D:\\regfiles\\ && git commit -a -m\"" + (string)buf + '"';
	system(commit_script.c_str());
	return true;
}

bool restore_keys_from_files()
{
	HANDLE ProcessToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ProcessToken))
	{
		cout << endl << "error open process token";
		return false;
	}
	if (!SetPrivilege(ProcessToken, SE_RESTORE_NAME, TRUE))
		cout << endl << "error set";
	for (int i = 0; i < 3; i++)
	{
		if (RegRestoreKeyA(root_keys[i], file_names[i].cstr(), REG_FORCE_RESTORE) != ERROR_SUCCESS)
		{
			cout << endl << "������ ������ ������� ���������� �� �����";
			return false;
		}
		cout << endl << "������ " << root_keys[i] << " ������� ����������� ���������� ����� " << file_names[i];
	}
	return true;
}

bool restore_files_from_git()
{
	system("cd D:\\sem4\\course_work_spovm\\regfiles\\ && git revert HEAD -n");
	return true;
}
