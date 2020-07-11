
#include "srdi.h"

#include <cstdlib>


FARPROC GetProcAddressR(UINT_PTR uiLibraryAddress, LPCSTR lpProcName)
{
	FARPROC fpResult = NULL;

	if (uiLibraryAddress == NULL)
		return NULL;

	UINT_PTR uiAddressArray = 0;
	UINT_PTR uiNameArray = 0;
	UINT_PTR uiNameOrdinals = 0;
	PIMAGE_NT_HEADERS pNtHeaders = NULL;
	PIMAGE_DATA_DIRECTORY pDataDirectory = NULL;
	PIMAGE_EXPORT_DIRECTORY pExportDirectory = NULL;

	
	pNtHeaders = (PIMAGE_NT_HEADERS)(uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew);

	pDataDirectory = (PIMAGE_DATA_DIRECTORY)&pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	
	pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(uiLibraryAddress + pDataDirectory->VirtualAddress);

	
	uiAddressArray = (uiLibraryAddress + pExportDirectory->AddressOfFunctions);

	
	uiNameArray = (uiLibraryAddress + pExportDirectory->AddressOfNames);

	
	uiNameOrdinals = (uiLibraryAddress + pExportDirectory->AddressOfNameOrdinals);

	
	if (((DWORD)lpProcName & 0xFFFF0000) == 0x00000000)
	{
		

		
		uiAddressArray += ((IMAGE_ORDINAL((DWORD)lpProcName) - pExportDirectory->Base) * sizeof(DWORD));

		
		fpResult = (FARPROC)(uiLibraryAddress + DEREF_32(uiAddressArray));
	}
	else
	{
		
		DWORD dwCounter = pExportDirectory->NumberOfNames;
		while (dwCounter--)
		{
			char* cpExportedFunctionName = (char*)(uiLibraryAddress + DEREF_32(uiNameArray));

			
			if (strcmp(cpExportedFunctionName, lpProcName) == 0)
			{
				
				uiAddressArray += (DEREF_16(uiNameOrdinals) * sizeof(DWORD));

				
				fpResult = (FARPROC)(uiLibraryAddress + DEREF_32(uiAddressArray));

				
				break;
			}

			
			uiNameArray += sizeof(DWORD);

			
			uiNameOrdinals += sizeof(WORD);
		}
	}

	return fpResult;
}

UINT_PTR srdi(LPSTR finalShellcode) {
	DWORD dwOldProtect1 = 0;
	SYSTEM_INFO sysInfo;
	UINT_PTR hLoadedDLL = NULL;

	GetNativeSystemInfo(&sysInfo);

	
	
	if (VirtualProtect(finalShellcode, sysInfo.dwPageSize, PAGE_EXECUTE_READWRITE, &dwOldProtect1)) {
		RDI rdi = (RDI)(finalShellcode);

		hLoadedDLL = rdi(); 
	}
	return hLoadedDLL;
};