#pragma comment(lib, "IPHLPAPI.lib")
#include "serialize/ISerializeSystem.h"
#include "script/IScriptSystem.h"
#include "MachinestatusFunctionPlugin.h"
#include "AllRegister.hpp"
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#define WIDTH 7
#define DIV 1024

using namespace std;


namespace hmsfp
{
	RTTI_TYPE_IMPLEMENT(MachinestatusFunctionPlugin);	
	SYSTEM_SINGLETON_IMPLEMENT(MachinestatusFunctionPlugin);

	int MachinestatusFunctionPlugin::GetFreePhysicalmemory(){
		
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx(&statex);
		return statex.ullAvailPhys / 1024 / 1024;
	}

	int MachinestatusFunctionPlugin::GetTotalPhysicalmemory(){
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx(&statex);
		return statex.ullTotalPhys / 1024 / 1024;
	}

	int MachinestatusFunctionPlugin::GetFreeVirtualmemory(){
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx(&statex);
		return statex.ullAvailVirtual / 1024 / 1024;
	}

	int MachinestatusFunctionPlugin::GetTotalVirtualmemory(){
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx(&statex);
		return statex.ullTotalVirtual / 1024 / 1024;
	}

	__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
	{
		__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
		__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

		return   (b - a);
	}

	float MachinestatusFunctionPlugin::GetCpuBusypercentage(){

		BOOL res;

		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;

		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		double now = GetTickCount();

		__int64 idleTime_int = idleTime.dwHighDateTime << 32 | idleTime.dwLowDateTime;
		__int64 kernelTime_int = kernelTime.dwHighDateTime << 32 | kernelTime.dwLowDateTime;
		__int64 userTime_int = userTime.dwHighDateTime << 32 | userTime.dwLowDateTime;

		int idle = idleTime_int - preidleTime;
		int kernel = kernelTime_int - prekernelTime;
		int user = userTime_int - preuserTime;

		float cpu = ((kernel + user - idle) *100.0 / (kernel + user)) / ((now - lastcputime) / 1000);

		preidleTime = idleTime_int;
		prekernelTime = kernelTime_int;
		preuserTime = userTime_int;
		lastcputime = now;

		return cpu;
	}

	int gettotalreceive(){
		// Declare and initialize variables.

		DWORD dwSize = 0;
		DWORD dwRetVal = 0;

		unsigned int i, j;
		int rec = 0;
		/* variables used for GetIfTable and GetIfEntry */
		MIB_IFTABLE *pIfTable;
		MIB_IFROW *pIfRow;

		// Allocate memory for our pointers.
		pIfTable = (MIB_IFTABLE *)MALLOC(sizeof (MIB_IFTABLE));
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");
			return 1;
		}
		// Make an initial call to GetIfTable to get the
		// necessary size into dwSize
		dwSize = sizeof (MIB_IFTABLE);
		if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
			FREE(pIfTable);
			pIfTable = (MIB_IFTABLE *)MALLOC(dwSize);
			if (pIfTable == NULL) {
				printf("Error allocating memory needed to call GetIfTable\n");
				return 1;
			}
		}
		// Make a second call to GetIfTable to get the actual
		// data we want.
		if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
			//printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);

			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW *)& pIfTable->table[i];

				if (pIfRow->dwIndex == 11) {
					rec = pIfRow->dwInOctets;//总的收到的数据大小(字节)
				}


				/*printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
				printf("\tInterfaceName[%d]:\t %ws", i, pIfRow->wszName);
				printf("\n");
				printf("\tDescription[%d]:\t ", i);
				for (j = 0; j < pIfRow->dwDescrLen; j++)
				printf("%c", pIfRow->bDescr[j]);
				printf("\n");
				printf("\tType[%d]:\t ", i);
				switch (pIfRow->dwType) {
				case IF_TYPE_OTHER:
				printf("Other\n");
				break;
				case IF_TYPE_ETHERNET_CSMACD:
				printf("Ethernet\n");
				break;
				case IF_TYPE_ISO88025_TOKENRING:
				printf("Token Ring\n");
				break;
				case IF_TYPE_PPP:
				printf("PPP\n");
				break;
				case IF_TYPE_SOFTWARE_LOOPBACK:
				printf("Software Lookback\n");
				break;
				case IF_TYPE_ATM:
				printf("ATM\n");
				break;
				case IF_TYPE_IEEE80211:
				printf("IEEE 802.11 Wireless\n");
				break;
				case IF_TYPE_TUNNEL:
				printf("Tunnel type encapsulation\n");
				break;
				case IF_TYPE_IEEE1394:
				printf("IEEE 1394 Firewire\n");
				break;
				default:
				printf("Unknown type %ld\n", pIfRow->dwType);
				break;
				}
				printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
				printf("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);
				printf("\tPhysical Addr:\t ");
				if (pIfRow->dwPhysAddrLen == 0)
				printf("\n");
				for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
				if (j == (pIfRow->dwPhysAddrLen - 1))
				printf("%.2X\n", (int)pIfRow->bPhysAddr[j]);
				else
				printf("%.2X-", (int)pIfRow->bPhysAddr[j]);
				}
				printf("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
				printf("\tOper Status[%d]:\t ", i);
				switch (pIfRow->dwOperStatus) {
				case IF_OPER_STATUS_NON_OPERATIONAL:
				printf("Non Operational\n");
				break;
				case IF_OPER_STATUS_UNREACHABLE:
				printf("Unreachable\n");
				break;
				case IF_OPER_STATUS_DISCONNECTED:
				printf("Disconnected\n");
				break;
				case IF_OPER_STATUS_CONNECTING:
				printf("Connecting\n");
				break;
				case IF_OPER_STATUS_CONNECTED:
				printf("Connected\n");
				break;
				case IF_OPER_STATUS_OPERATIONAL:
				printf("Operational\n");
				break;
				default:
				printf("Unknown status %ld\n", pIfRow->dwAdminStatus);
				break;
				}
				printf("\n");*/
			}
		}
		else {
			printf("GetIfTable failed with error: \n", dwRetVal);
			if (pIfTable != NULL) {
				FREE(pIfTable);
				pIfTable = NULL;
			}
			return 1;
			// Here you can use FormatMessage to find out why 
			// it failed.
		}
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}
		return rec;
	}

	float MachinestatusFunctionPlugin::GetNetcardReceiveSpeed(){
		
		int rec = gettotalreceive();
		double now = GetTickCount();

		float result = (rec - lasttotalrec) / ((now - lastnetcardrectime) / 1000);
		
		lastnetcardrectime = now;
		lasttotalrec = rec;

		return result;
	}

	int gettotalsend(){
		// Declare and initialize variables.

		DWORD dwSize = 0;
		DWORD dwRetVal = 0;

		unsigned int i, j;
		int send = 0;
		/* variables used for GetIfTable and GetIfEntry */
		MIB_IFTABLE *pIfTable;
		MIB_IFROW *pIfRow;

		// Allocate memory for our pointers.
		pIfTable = (MIB_IFTABLE *)MALLOC(sizeof (MIB_IFTABLE));
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");
			return 1;
		}

		dwSize = sizeof (MIB_IFTABLE);
		if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
			FREE(pIfTable);
			pIfTable = (MIB_IFTABLE *)MALLOC(dwSize);
			if (pIfTable == NULL) {
				printf("Error allocating memory needed to call GetIfTable\n");
				return 1;
			}
		}

		if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
			//printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);

			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW *)& pIfTable->table[i];

				if (pIfRow->dwIndex == 11) {
					send = pIfRow->dwOutOctets;//总共发送的数据大小(字节)
				}
			}
		}
		else {
			printf("GetIfTable failed with error: \n", dwRetVal);
			if (pIfTable != NULL) {
				FREE(pIfTable);
				pIfTable = NULL;
			}
			return 1;

		}
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}
		return send;
	}

	float MachinestatusFunctionPlugin::GetNetcardSendSpeed(){
		
		int send = gettotalsend();
		double now = GetTickCount();

		float result = (send - lasttotalsend) / ((now - lastnetcardsendtime) / 1000);

		lastnetcardsendtime = now;
		lasttotalsend = send;

		return result;
		
	}

	//---------------------------------------------------------------------------------------------------------
	MachinestatusFunctionPlugin::MachinestatusFunctionPlugin()
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	MachinestatusFunctionPlugin::~MachinestatusFunctionPlugin()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void MachinestatusFunctionPlugin::Initialize()
	{
		hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
		Register_All("machinestatus", *sys);
	
		lastcputime = GetTickCount();
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		GetSystemTimes(&idleTime, &kernelTime, &userTime);
		preidleTime = idleTime.dwHighDateTime << 32 | idleTime.dwLowDateTime;
		prekernelTime = kernelTime.dwHighDateTime << 32 | kernelTime.dwLowDateTime;
		preuserTime = userTime.dwHighDateTime << 32 | userTime.dwLowDateTime;

		lastnetcardrectime = GetTickCount();
		lasttotalrec = gettotalreceive();

		lastnetcardsendtime = GetTickCount();
		lasttotalsend = gettotalsend();

	}
	//---------------------------------------------------------------------------------------------------------
	void MachinestatusFunctionPlugin::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void MachinestatusFunctionPlugin::Exit()
	{

	}

}
