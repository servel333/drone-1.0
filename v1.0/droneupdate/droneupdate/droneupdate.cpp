// droneupdate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdint.h>
//typedef uint32_t DWORD;

#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>

#if USE_FTDI_API
#include "ftd2xx.h"
FT_STATUS GetComPort(LONG * comPort);
#else
#include <windows.h>
bool GetComPort(int * comPort, const int PORT_SCAN_LIMIT);
#endif

bool CheckAvrdudeVersion(int major, int minor);

const char AVRDUDE_CMD[] = "avrdude -v -v -p atxmega64d3 -c avr109 -b 57600 -D ";

//Methods to find the COM port without FTDI library
// http://www.ftdichip.com/Support/Knowledgebase/index.html?wherecanifindport.htm
// http://stackoverflow.com/questions/1388871/how-do-i-get-a-list-of-available-serial-ports-in-win32
// http://stackoverflow.com/questions/2674048/what-is-proper-way-to-detect-all-available-serial-ports-on-windows

/*
//http://www.ftdichip.com/Support/Knowledgebase/index.html?wherecanifindport.htm
#include <winreg.h>

#define SZ_FTDIBUS          "SYSTEM\\CurrentControlSet\\Enum\\FTDIBUS"
#define SZ_PORT_NAME        "PortName"

//void EnumerateKey(HKEY hKey, unsigned char *pszKeyName);

void EnumerateKey(HKEY hParentKey, unsigned char *pszKeyName)
{
	unsigned char pszName[MAX_PATH] = "";
	DWORD dwNameLen = MAX_PATH;
	DWORD dwResult;
	DWORD dwKeyCount;
	DWORD dwValueCount;
	DWORD i, gotIt;
	HKEY hKey;
	char Buf[256];
	DWORD BufLen;
	DWORD BufType;

	//dwResult = RegOpenKeyEx(hParentKey, pszKeyName, 0L, KEY_READ, &hKey);
	dwResult = RegOpenKeyExW(hParentKey, pszKeyName, 0L, KEY_READ, &hKey);
	if (ERROR_SUCCESS != dwResult) {
		//printf("Can't open %s\\%s (%d)\n", hParentKey, pszKeyName, dwResult);
		std::cout << "Cant open " << hParentKey << '\\' << pszKeyName << " (" << dwResult << ")" << std::endl;
		return;
	}
	//    printf("%s\n",pszKeyName);
	// get number of subkeys and values for this key
	dwResult = RegQueryInfoKey(hKey,NULL,NULL,NULL,&dwKeyCount,NULL,NULL,&dwValueCount,NULL,NULL,NULL,NULL);
	if (ERROR_SUCCESS != dwResult)
	{
		return;
	}
	// enumerate values and search for UpperFilters
	for (i = 0, gotIt = 0; i < dwValueCount && gotIt == 0; i++) {
		BufLen = 256;
		dwResult = RegEnumValue(hKey, i, pszName, &dwNameLen, NULL, &BufType, Buf, &BufLen);
		if (ERROR_SUCCESS == dwResult) {
			// test for UpperFilters value
			if (strcmpi(pszName, SZ_PORT_NAME) == 0) {
				//printf("%s\n", Buf);
				std::cout << Buf << std::endl;
				gotIt = 1;
			}
		}
		pszName[0] = '\0';
		dwNameLen = MAX_PATH;
	}
	// enumerate keys
	for (i = 0; i < dwKeyCount && gotIt == 0; i++) {
		dwResult = RegEnumKeyEx(hKey, i, pszName, &dwNameLen, NULL, NULL, NULL, NULL);
		if (ERROR_SUCCESS == dwResult) {
			EnumerateKey(hKey, pszName);
		}
		pszName[0] = '\0';
		dwNameLen = MAX_PATH;
	}
	RegCloseKey(hKey);
}





int GetComPortWithoutFTDI(void)
{
	unsigned char pszName[MAX_PATH] = "";
	DWORD dwNameLen = MAX_PATH;
	HKEY hStartKey = HKEY_LOCAL_MACHINE;
	HKEY hKey;
	DWORD dwSubKeys;
	DWORD dwResult;
	DWORD i;

	// open FTDIBUS key
	dwResult = RegOpenKeyEx(hStartKey,SZ_FTDIBUS,0L,KEY_READ,&hKey);

	if (ERROR_SUCCESS == dwResult) {
		// get number of subkeys under FTDIBUS
		dwResult = RegQueryInfoKey(hKey,NULL,NULL,NULL,&dwSubKeys,NULL,NULL,NULL,
			NULL, NULL, NULL, NULL
		);

		// enumerate the subkeys under FTDIBUS
		for (i = 0; i < dwSubKeys; i++) {
			dwResult = RegEnumKeyEx(hKey, i, pszName, &dwNameLen, NULL, NULL, NULL, NULL);
			if (ERROR_SUCCESS == dwResult) {
				//printf("FTDIBUS\\%s : ", pszName);
				std::cout << "FTDIBUS\\" << pszName << " : ";
				EnumerateKey(hKey, pszName);
			}
			pszName[0] = '\0';
			dwNameLen = MAX_PATH;
		}
		// close FTDIBUS key
		RegCloseKey(hKey);
	}
	else {
		//printf("Can't open %s\\%s (%d)\n", hStartKey, SZ_FTDIBUS, dwResult);
		std::cout << "Cant open " << hStartKey << "\\" << SZ_FTDIBUS << "(" << dwResult << ")" << std::endl;
	}
}

*/








int _tmain(int argc, _TCHAR* argv[])
{
	std::string hexFile;

	int result = 0;
	
	if (argc == 1)
	{
		hexFile = "Drone.hex";
	}
	else if(argc==2)
	{
		std::wstring wHexFile = argv[1];
		std::string tmp(wHexFile.begin(), wHexFile.end()); //not portable
		hexFile = tmp;
	}
	else
	{
		std::cerr << "Usage: droneupdate [Drone.hex]" << std::endl;
		return -1;
	}
	
	/*
	if (CheckAvrdudeVersion(6, 1 ) )
	{

	}
	else
	{
		std::cerr << "ERROR: avrdude is too old" << std::endl;
		std::cerr << "  download newer version -- tested with 6.01 http://download.savannah.gnu.org/releases/avrdude/avrdude-6.0.1-mingw32.zip" << std::endl;
		return -1;
	}
	*/
	
	/*
#if USE_FTDI_API

#else
	int comPort;
	const int PORT_SCAN_LIMIT = 1000;
	bool status = GetComPort(&comPort,PORT_SCAN_LIMIT);
	if(!status)
	{
		std::cerr << "ERROR: GetComPort()" << std::endl;
		std::cerr << "ERROR: No Serial Port COMx found (where x is 0 to " << PORT_SCAN_LIMIT << " checked)" << std::endl;
		std::cerr << "ERROR: Check that Drone is plugged-in and powered-up (hold start until it turns on)." << std::endl;
		return 1;
	}
#endif
	*/

#if USE_FTDI_API
	//scan for comPort here
	LONG comPort = -1;
	FT_STATUS ftStatus;
	ftStatus = GetComPort(&comPort);
	if (ftStatus == FT_OK)
	{
	}
	else
	{
		std::cerr << "ERROR: GetComPort()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}


	if (comPort != -1)
	{
#else
	const int PORT_SCAN_LIMIT = 1000;
	for (int comPort = 0; comPort < PORT_SCAN_LIMIT; comPort++)
	{

#endif
		std::ostringstream ossComPort;
		ossComPort << comPort;
		std::string strComPort(ossComPort.str());
		std::cout << "COM Port = " << strComPort << std::endl;

#if !(USE_FTDI_API)
		const char COM_PREFIX[] = "\\\\.\\";
		//wsprintf(szPort, "COM%d", nPort);
		std::string f = std::string(COM_PREFIX) + "COM" + strComPort;

		HANDLE m_hIDComDev = CreateFileA(f.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL);
		if (m_hIDComDev == INVALID_HANDLE_VALUE)
		{
			DWORD err = GetLastError();
		}
		else
		{
			if (!CloseHandle(m_hIDComDev))
			{
				DWORD err = GetLastError();
			}
			else
			{
#endif

				std::string cmd = std::string(AVRDUDE_CMD) + " -P com" + strComPort + " -U flash:w:" + hexFile + " ";
				std::cout << cmd << std::endl;

				//result = 0; //DEBUG
				result = system(cmd.c_str());

#if !( USE_FTDI_API )

				if (0 == result)
				{

					break;
				}
			}
		}
	}
	if (result)
	{
		std::cerr << "  Attempted update with serial port COMx where x is 0 to "<< PORT_SCAN_LIMIT << std::endl;
		std::cerr << "  Check that Drone is plugged-in and powered-up (hold start until it turns on)." << std::endl;
		std::cerr << "ERROR: Drone firmware update FAILED" << std::endl;
	}
	else
	{
		std::cout << "OK: Drone firmware update PASSED" << std::endl;
	}


#else

	}
	else
	{
		std::cerr << "ERROR: comPort==-1" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		result = comPort;
	}

#endif

	return result;
}


bool CheckAvrdudeVersion( const int reqMajor, const int reqMinor )
{
	int major;
	int minor;


	FILE * fp;
	if (NULL == (fp = _popen(AVRDUDE_CMD, "rt")))
	{
		std::cerr << "ERROR: avrdude problem" << std::endl;
		return false;
	}
	//capture avrdude stdout
	std::string str;
	char cStr[512];
	while (fgets(cStr, sizeof(cStr), fp) != NULL)
	{
		str += cStr;
	}
	_pclose(fp);
	//analyze avrdude stdout for '''avrdude.exe: AVR Part "atxmega64d3" not found.'''
	std::istringstream stream(str);
	std::string line;
	bool avrdudeVersionOk = true;
	while (std::getline(stream, line)) 
	{
		std::size_t found;

		found = line.find("avrdude.exe: Version");
		//"5.10," is pos=[21 to 25]
		if (found != std::string::npos)
		{
			std::string majorminor = line.substr(21, 6);
			std::size_t period = majorminor.find(".");
			std::string strMajor = majorminor.substr(0, period);
			major = atoi(strMajor.c_str());
			
			std::size_t comma = majorminor.find(",");
			std::string strMinor = majorminor.substr(period + 1, comma - period - 1);
			minor = atoi(strMinor.c_str());

			if (((major == reqMajor) && (minor >= reqMinor)) || (major > reqMajor))
			{
			}
			else
			{
				avrdudeVersionOk = false;
			}
		}


		found = line.find("avrdude.exe: AVR Part \"atxmega64d3\" not found.");
		if (found != std::string::npos)
		{
			avrdudeVersionOk = false;
			break;
		}
	}

	return avrdudeVersionOk;
}



#if USE_FTDI_API
FT_STATUS GetComPort( LONG * comPort )
{

	//FTD2xx: 
	//  FT_GetDriverVersion , FT_GetLibraryVersion
	//  FT_GetDeviceInfo , FT_CreateDeviceInfoList , FT_GetDeviceInfoDetail
	//  FT_GetComPortNumber
	//  FT_CyclePort , FT_ResetPort , FT_ResetDevice , FT_GetStatus

	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;

	uint8_t * version;
	#define VER_BUILD 0
	#define VER_MINOR 1
	#define VER_MAJOR 2
	#define	VER_UNUSED 3

	DWORD dwLibraryVer;
	// Get DLL version
	ftStatus = FT_GetLibraryVersion(&dwLibraryVer);
	if (ftStatus == FT_OK)
	{
		version = (uint8_t *)&dwLibraryVer; //ASSUMPTION: dwLibraryVer is little-endian
		std::cout << "FTD2xx Library " << int(version[VER_MAJOR]) << "." << int(version[VER_MINOR]) << " Build " << int(version[VER_BUILD]) << std::endl;
	}
	else
	{
		std::cerr << "ERROR: FT_GetLibraryVersion()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}

	DWORD dwDriverVer;
	// Get driver version
	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus == FT_OK)
	{
		ftStatus = FT_GetDriverVersion(ftHandle, &dwDriverVer);
		if (ftStatus == FT_OK)
		{
			version = (uint8_t *)&dwDriverVer; //ASSUMPTION: dwLibraryVer is little-endian
			std::cout << "FTD2xx Driver " << int(version[VER_MAJOR]) << "." << int(version[VER_MINOR]) << " Build " << int(version[VER_BUILD]) << std::dec << std::endl;
		}
		else
		{
			std::cerr << "ERROR: FT_GetDriverVersion()" << std::endl;
			std::cerr << "ERROR: returning ftStatus" << std::endl;
			return ftStatus;
		}
		FT_Close(ftHandle);
	}
	else
	{
		std::cerr << "ERROR: FT_Open()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}

	DWORD numDevs;
	ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
	if (ftStatus == FT_OK)
	{
		// FT_ListDevices OK, number of devices connected is in numDevs
	}
	else
	{
		// FT_ListDevices failed
		std::cerr << "ERROR: FT_ListDevices()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}

	DWORD numDevs2;
	// create the device information list
	ftStatus = FT_CreateDeviceInfoList(&numDevs2);
	if (ftStatus == FT_OK)
	{

	}
	else 
	{
		// FT_CreateDeviceInfoList failed
		std::cerr << "ERROR: FT_CreateDeviceInfoList()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}

	if (numDevs != numDevs2)
	{
		std::cerr << "ERROR: number of devices from FT_ListDevices() and FT_CreateDeviceInfoList() are different" << std::endl;
		return ftStatus;
	}

	if (numDevs == 0)
	{
		std::cerr << "ERROR: numDevs==0 ; ftStatus==FT_DEVICE_NOT_FOUND (2) ???" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}

	if (numDevs > 1)
	{
		std::cerr << "ERROR: numDevs > 1" << std::endl;
		std::cerr << "ERROR: returning numDevs" << std::endl;
		return numDevs;
	}

	std::cout << "1 FTDI device found" << std::endl;


	/*
	DWORD devIndex = 0; // first device
	char buffer[64]; // more than enough room!
	ftStatus = FT_ListDevices((PVOID)devIndex, buffer, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
	if (ftStatus == FT_OK)
	{
	// FT_ListDevices OK, serial number is in Buffer
	}
	else
	{
	// FT_ListDevices failed
	std::cerr << "ERROR: FT_ListDevices()" << std::endl;
	std::cerr << "ERROR: returning ftStatus" << std::endl;
	return ftStatus;
	}

	std::cout << buffer << std::endl;
	*/

	
	//FT_DEVICE_LIST_INFO_NODE * devInfo;
	// allocate storage for list based on numDevs
	//devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
	FT_DEVICE_LIST_INFO_NODE devInfo[1];
	// get the device information list
	ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
	if (ftStatus == FT_OK)
	{
		//for (unsigned int i = 0; i < numDevs; i++)
		//{
		unsigned int i = 0;
			//std::cout << "Dev " << i << ":" << std::endl;
			std::cout << std::hex;
			std::cout << "  Flags=0x" << devInfo[i].Flags << std::endl;
			std::cout << "  Type=0x" << devInfo[i].Type << std::endl;
			std::cout << "  ID=0x" << devInfo[i].ID << std::endl;
			std::cout << "  LocId=0x" << devInfo[i].LocId << std::endl;
			std::cout << "  ftHandle=0x" << devInfo[i].ftHandle << std::endl;
			std::cout << std::dec;
			std::cout << "  SerialNumber=" << devInfo[i].SerialNumber << std::endl;
			std::cout << "  Desc=" << devInfo[i].Description << std::endl;
		//}
	}
	else
	{
		std::cerr << "ERROR: FT_GetDeviceInfoList()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}
	


	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus == FT_OK)
	{
		ftStatus = FT_GetComPortNumber(ftHandle, comPort);
		if (ftStatus == FT_OK) 
		{
			if(*comPort == -1) 
			{
				// No COM port assigned 
			}
			else {
				// COM port assigned with number held in *comPort
			}
		}
		else 
		{
			// FT_GetComPortNumber FAILED!
			std::cerr << "ERROR: FT_GetComPortNumber()" << std::endl;
			std::cerr << "ERROR: returning ftStatus" << std::endl;
			return ftStatus;
		}
		FT_Close(ftHandle);
		if (ftStatus == FT_OK)
		{

		}
		else
		{
			std::cerr << "ERROR: FT_Close()" << std::endl;
			std::cerr << "ERROR: returning ftStatus" << std::endl;
			return ftStatus;
		}
	}
	else
	{
		std::cerr << "ERROR: FT_Open()" << std::endl;
		std::cerr << "ERROR: returning ftStatus" << std::endl;
		return ftStatus;
	}

	return FT_OK;
}

#else

bool GetComPort(int * comPort, const int PORT_SCAN_LIMIT)
{
	const DWORD ucchMax = 1000;
	TCHAR lpTargetPath[ucchMax]; // buffer to store the path of the COMPORTS

	*comPort = -1;
	for (int i = 0; i<=PORT_SCAN_LIMIT; i++)
	{
		std::ostringstream ossComPort;
		ossComPort << i;
		std::string strComPort(ossComPort.str());
		std::string comPortName = std::string("COM") + ossComPort.str();
		if(QueryDosDevice((LPCTSTR)comPortName.c_str(), (LPTSTR)lpTargetPath, ucchMax))
		{
			*comPort = i;
			return true;
		}
	}
	return false;
}

#endif



