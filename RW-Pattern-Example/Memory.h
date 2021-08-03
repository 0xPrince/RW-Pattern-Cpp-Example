#pragma once
#include <Windows.h>
#include <vector>

class Memory
{
	/*
	--Author: 0xPrince
	*/
public:
	DWORD ProcessId = 0;
	HANDLE ProcessHandle;

	typedef struct _MEMORY_REGION {
		DWORD_PTR dwBaseAddr;
		DWORD_PTR dwMemorySize;
	}MEMORY_REGION;

	BOOL AttachProcess(DWORD ProcId)
	{
		if (ProcId == 0)
			return false;
		ProcessId = ProcId;
		ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessId);
		
		return ProcessHandle != nullptr;
	}
	bool ReplacePattern(DWORD_PTR dwStartRange, DWORD_PTR dwEndRange, BYTE* SearchAob, size_t SearchBytesSize, BYTE ReplaceAob[], size_t RepBytesSize)
	{
		//Author: 0xPrince
		std::vector<DWORD_PTR> foundedAddress;
		FindPattern(dwStartRange, dwEndRange, SearchAob, SearchBytesSize, foundedAddress);
		if (foundedAddress.empty())
			return false;
		DWORD OldProtect;
		for (int i = 0; i < foundedAddress.size(); i++)
		{
			VirtualProtectEx(ProcessHandle, (LPVOID)foundedAddress[i], RepBytesSize, 0x40, &OldProtect);
			BOOL WriteStat = WriteProcessMemory(ProcessHandle, (LPVOID)foundedAddress[i], ReplaceAob, RepBytesSize, 0);
			if (OldProtect != 0)
			{
				VirtualProtectEx(ProcessHandle, (LPVOID)foundedAddress[i], RepBytesSize, OldProtect, &OldProtect);
			}
			if (!WriteStat)
				return false;
		}
		return true;
	}

	bool FindPattern(DWORD_PTR StartRange, DWORD_PTR EndRange, BYTE* bSearchData, DWORD nSearchSize, std::vector<DWORD_PTR>& AddressRet)
	{

		BYTE* pCurrMemoryData = NULL;
		MEMORY_BASIC_INFORMATION	mbi;
		std::vector<MEMORY_REGION> m_vMemoryRegion;
		mbi.RegionSize = 0x1000;
		DWORD dwAddress = StartRange;



		while (VirtualQueryEx(ProcessHandle, (LPCVOID)dwAddress, &mbi, sizeof(mbi)) && (dwAddress < EndRange) && ((dwAddress + mbi.RegionSize) > dwAddress))
		{

			if ((mbi.State == MEM_COMMIT) && ((mbi.Protect & PAGE_GUARD) == 0) && (mbi.Protect != PAGE_NOACCESS) && ((mbi.AllocationProtect & PAGE_NOCACHE) != PAGE_NOCACHE))
			{

				MEMORY_REGION mData = { 0 };
				mData.dwBaseAddr = (DWORD_PTR)mbi.BaseAddress;
				mData.dwMemorySize = mbi.RegionSize;
				m_vMemoryRegion.push_back(mData);

			}
			dwAddress = (DWORD)mbi.BaseAddress + mbi.RegionSize;

		}


		std::vector<MEMORY_REGION>::iterator it;
		for (it = m_vMemoryRegion.begin(); it != m_vMemoryRegion.end(); it++)
		{
			MEMORY_REGION mData = *it;


			    DWORD_PTR dwNumberOfBytesRead = 0;
				pCurrMemoryData = new BYTE[mData.dwMemorySize];
				ZeroMemory(pCurrMemoryData, mData.dwMemorySize);
				ReadProcessMemory(ProcessHandle, (LPCVOID)mData.dwBaseAddr, pCurrMemoryData, mData.dwMemorySize, &dwNumberOfBytesRead);
				if ((int)dwNumberOfBytesRead <= 0)
				{
					delete[] pCurrMemoryData;
					continue;
				}
				DWORD_PTR dwOffset = 0;
				int iOffset = Memfind(pCurrMemoryData, dwNumberOfBytesRead, bSearchData, nSearchSize);
				while (iOffset != -1)
				{
					dwOffset += iOffset;
					AddressRet.push_back(dwOffset + mData.dwBaseAddr);
					dwOffset += nSearchSize;
					iOffset = Memfind(pCurrMemoryData + dwOffset, dwNumberOfBytesRead - dwOffset - nSearchSize, bSearchData, nSearchSize);
				}
		
			if (pCurrMemoryData != NULL)
			{
				delete[] pCurrMemoryData;
				pCurrMemoryData = NULL;
			}

		}
		return TRUE;
	}

	
	int Memfind(BYTE* buffer, DWORD dwBufferSize, BYTE* bstr, DWORD dwStrLen) {
		if (dwBufferSize < 0) {
			return -1;
		}
		DWORD  i, j;
		for (i = 0; i < dwBufferSize; i++) {
			for (j = 0; j < dwStrLen; j++) {
				if (buffer[i + j] != bstr[j] && bstr[j] != '?')
					break;

			}
			if (j == dwStrLen)
				return i;
		}
		return -1;
	}
};