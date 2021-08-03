#pragma once
#include <Windows.h>
#include <dwmapi.h>

#include <TlHelp32.h>
#include <tchar.h>
#include <string>
#include <iostream>

class Utils {
public:
	static DWORD FindTrueProcessId(std::wstring Procname) {
		if (Procname.empty())
			return 0;
		DWORD pid = 0;
		DWORD threadCount = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnap, &pe);
		while (Process32Next(hSnap, &pe)) {
			if (_tcsicmp(pe.szExeFile, Procname.c_str()) == 0) {
				if ((int)pe.cntThreads > threadCount) {
					threadCount = pe.cntThreads;
					pid = pe.th32ProcessID;
				}
			}
		}
		return pid;
	}
};

