/*
 --Author: 0xPrince
 --UPDATE- 1.1
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
		std::cout << "\n Error: Process Not Found";
		return;
	}

	Memory memory;
	if (!memory.AttachProcess(procid))
	{
		std::cout << "\n Error: AttachProcess";
		return;
	}

#pragma region FindPattern Example
	//std::vector<DWORD_PTR> SearchResult;
	//BYTE Searchpattern[] = {  0x00, 0x00, 0xB4, 0x43, 0x78, 0x17, 0x9C, 0x03, 0x84, 0xAE, 0x7E, 0x03, 0x30, 0x48 };//--PUBGM-1.7 IPadView Pattern
   //memory.FindPattern(0x70000000/*Start Range*/, 0x90000000/*End Range*/, new BYTE[]{0x00, 0x00, 0xB4, 0x43, 0xC4, '?', 0x6C, 0x03, 0x34, 0xA3, 0x4F, 0x03, 0x30, 0x48}/*Search Bytes*/, SearchResult);
	//MessageBoxA(0, std::to_string(SearchResult.size()).append( " Search Results Found").c_str(), "", MB_ICONINFORMATION);
#pragma endregion

#pragma region ReplacePattern Example Code
	/*
	  Search Range For IPADVIEW
	--SMARTGAGA=0x70000000, 0x90000000
	--GAMELOOP-4.4=0x20000000, 0x30000000
	--GAMELOOP-7.1=0x3D000000, 0x60000000
	*/
	//--PUBGM-1.7 IPadView Pattern
	bool Status = memory.ReplacePattern(0x70000000/*Start Range*/, 0x90000000/*End Range*/, new BYTE[]{ 0x00, 0x00, 0xB4, 0x43, 0x78, 0x17, 0x9C, 0x03, 0x84, 0xAE, 0x7E, 0x03, 0x30, 0x48 }/*Search Bytes*/, new BYTE[]{ 0x00, 0x00, 0x82 }/*Replace Bytes*/, true/*Force Write(Change Protection to Write)*/);

	if (Status)
	{
		std::cout << "\n Success: IPad View Activated";
	}
	else
	{
		std::cout << "\n Error Occured:: Make sure the driver is running.";
	}
	
#pragma endregion



#pragma region ReadMemory Example

//	float readvalue = memory.ReadMemory<float>(0x00000000/*Address*/);
#pragma endregion
#pragma region WriteMemory Example

   //memory.WriteMemory<float>(0x00000000/*Address*/,280.0f/*WriteValue*/,true/*Force Write(Change Protection)*/);
#pragma endregion

#pragma region WriteBytes Example

	//bool status = memory.WriteBytes(0x00000000/*Address*/, new  BYTE[]{ 0x00, 0x00, 0xB4 },true/*Force Write(Change Protection)*/);
#pragma endregion

#pragma region ChangeProtection Example
	
//	DWORD oldProtect;
	//memory.ChangeProtection(0x00000000/*Address*/, sizeof(int)/*Byte Size*/, PAGE_EXECUTE_READWRITE/*New Protection*/, oldProtect/*old Protection*/);
	
#pragma endregion

	CloseHandle(memory.ProcessHandle);
	
}
int main()
{
	
	StartMainProcess();
	getchar();
	return EXIT_SUCCESS;
}

