#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
using namespace std;
void printMenu();
void stopExecutable();
void deleteExecutalbe();
void deleteFromStartup();
void doAll();
void cls(HANDLE hConsole);
void findRunningProcess(string process);
short killProcessById(DWORD dwProcessId, UINT uExitCode);
string getWindowsUsername();
int main(int argc, char* argv[]) {
	start:
	short userInput = 0;
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	cls(hStdout);
	printMenu();
	cin >> userInput;
	switch (userInput)
	{
	case 1: stopExecutable(); break;
	case 2: deleteExecutalbe(); break;
	case 3: deleteFromStartup(); break;
	case 4: doAll(); break;
	case 0: exit(0);
	default:
		cout << "Wrong Input\n";
		break;
	}
	system("PAUSE");
	goto start;
	return 0;
}
void printMenu() {
	cout << "1. Stop The Executable\n";
	cout << "2. Delete Chrome Prank Executable\n";
	cout << "3. Delete From Startup\n";
	cout << "4. Do All.\n";
	cout << "0. Exit.\n";
}

void stopExecutable() {
	findRunningProcess("ChromePrank.exe");
}

void deleteExecutalbe() {
	string username = getWindowsUsername();
	string fileName = "C:\\Users\\" + username + "\\AppData\\Local\\Temp\\ChromePrank.exe";
	if (DeleteFile(fileName.c_str()) == 0) {
		cout << "File Deleted Successfully\n";
	}
}

void deleteFromStartup() {
	HKEY hkey = HKEY_CURRENT_USER;
	RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	if (RegDeleteValue(hkey, "Microsawft") == 0) {
		cout << "Deleted From Startup Successfully!\n";
	}
	RegCloseKey(hkey);
}


void doAll() {
	stopExecutable();
	deleteExecutalbe();
	deleteFromStartup();
}

string getWindowsUsername() {
	char username[128];
	DWORD username_len = 128;
	GetUserName(username, &username_len);
	return(string)username;
}

void cls(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(hConsole, 
		(TCHAR) ' ',
		dwConSize,
		coordScreen,
		&cCharsWritten))
	{
		return;
	}
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}
	if (!FillConsoleOutputAttribute(hConsole, 
		csbi.wAttributes,
		dwConSize,
		coordScreen,
		&cCharsWritten))
	{
		return;
	}

	SetConsoleCursorPosition(hConsole, coordScreen);
}


void findRunningProcess(string process)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		if (pe32.szExeFile == process)
		{
			cout << "Process found:" << pe32.szExeFile << endl;
			if (TerminateProcess(hProcessSnap, 1) == 0) {
				cout << "Process Killed Successfully\n";
			}
		}
		else
		{
			while (Process32Next(hProcessSnap, &pe32))
			{
				if (pe32.szExeFile == process)
				{
					cout << "Process found:" << pe32.szExeFile << endl;
					if (killProcessById(pe32.th32ProcessID, 1) == 0) {
						cout << "Process Killed Successfully\n";
					}
					break;
				}
			}
		}
		CloseHandle(hProcessSnap);
	}
}

short killProcessById(DWORD dwProcessId, UINT uExitCode)
{
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	short status = TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	return status;
}