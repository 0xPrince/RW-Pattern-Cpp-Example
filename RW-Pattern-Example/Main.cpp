/*
 --Author: Copy Paster 0xPrince
*/

#include "Memory.h"
#include "Utils.h"




void StartMainProcess()
{
	
	/*
	ProcessNames
	--Smartgaga = AndroidProcess.exe
	--Gameloop = aow_exe.exe
	*/
	DWORD procid = Utils::FindTrueProcessId(L"AndroidProcess.exe");
	if (procid == 0)
	{
		MessageBoxA(0, "Process Not Found", "Error", MB_ICONERROR);
		return;
	}
	Memory memory;
	if (!memory.AttachProcess(procid))
	{
		MessageBoxA(0, "Error: AttachProcess", "Error", MB_ICONERROR);
		return;
	}

#pragma region ReplacePattern Example Code
	BYTE Searchpattern[] = { 0x00, 0x00, 0xB4, 0x43, 0xC4, 0xFE, 0x6C, 0x03, 0x34, 0xA3, 0x4F, 0x03, 0x30, 0x48 };//1.5 IPAD VIEW
	BYTE Replacebyte[] = { 0x00, 0x00, 0x82, 0x43, 0xC4, 0xFE, 0x6C, 0x03, 0x34, 0xA3, 0x4F, 0x03, 0x30, 0x48 };
	/*
	  Search Range For IPADVIEW
	--SMARTGAGA=0x70000000, 0x90000000
	--GAMELOOP-4.4=0x20000000, 0x30000000
	*/
	bool Status = memory.ReplacePattern(0x70000000, 0x90000000, Searchpattern, sizeof(Searchpattern), Replacebyte, sizeof(Replacebyte));
	
	if (Status)
	{
		MessageBoxA(0, "IPad View Activated", "Success", MB_ICONINFORMATION);
	}
	else
	{
		MessageBoxA(0, "Error Occured: Make sure the driver is running.", "Error", MB_ICONERROR);
	}
#pragma endregion

#pragma region FindPattern Example
	/*std::vector<DWORD_PTR> foundedAddress;
	BYTE Searchpattern[] = { 0x00, 0x00, 0xB4, 0x43, 0xC4, 0xFE, 0x6C, 0x03, 0x34, 0xA3, 0x4F, 0x03, 0x30, 0x48 };//1.5 IPAD VIEW
	memory.FindPattern(0x70000000, 0x90000000, Searchpattern, sizeof(Searchpattern), foundedAddress);*/
	
#pragma endregion
	
	CloseHandle(memory.ProcessHandle);
	
}
int main()
{
	
	StartMainProcess();

	return 0;
}

