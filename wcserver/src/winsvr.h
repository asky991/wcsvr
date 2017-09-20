#pragma once

#ifdef WIN32
#include <Windows.h>
#include <shellapi.h> 
#include <time.h>
#include <cstring>
#include <string> 
#include <stdio.h>
#include <time.h>
#include<iostream> 
#include <tchar.h>
using namespace std;


const int BUF_LEN = 300;

void log_info(int argc, char* argv[]);
void win_log(string msg);
int install_svr();
int uninstall_svr();
int exec_cmd(string cmd);
void init_daemon(void);
int init_proc();
int init_param();
int init_run();
string g_wcsvrexecname = "";
string g_wcsvrpath = "";
string g_wcsvrname = "";


//////////////////////////////////////////////////////////////////////////
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
void WINAPI service_main(int argc, char** argv);
void WINAPI ServiceHandler(DWORD fdwControl);


SC_HANDLE schSCManager;
SC_HANDLE schService;
int uaquit;


void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
void LogEvent(LPCTSTR pszFormat, ...);
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);

BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
DWORD dwThreadID;


//////////////////////////////////////////////////////////////////////////


int init_svr(int argc, char* argv[])
{


	init_param();
	if (0 == strcmp("-install", argv[1]) || 0 == strcmp("install", argv[1]))
	{
		install_svr();
	}
	else if (0 == strcmp("-uninstall", argv[1]) || 0 == strcmp("uninstall", argv[1]))
	{
		uninstall_svr();
	}
	else if (0 == strcmp("-run", argv[1]) || 0 == strcmp("run", argv[1]))
	{
		init_proc();
	}
	else
	{
		return -1;
	}

// 	FILE *fp; time_t t;
// 	init_daemon();//初始化为Daemon
// 	while (1)//每隔一分钟向test.log报告运行状态
// 	{
// 		Sleep(60);//睡 眠一分钟
// 		if ((fp = fopen("test.log", "a")) >= 0)
// 		{
// 			t = time(0);
// 			fprintf(fp, "Im here at %sn", asctime(localtime(&t)));
// 			fclose(fp);
// 		}
// 	}
	return 0;
}

void log_info(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		string str = "log_info:";
		str.append(argv[i]);
		win_log(str);
	}
}

void init_daemon(void)
{

}

int init_proc()
{
	init_daemon();//初始化为Daemon

	init_run();

// 	while (1)//每隔一分钟向test.log报告运行状态
// 	{
// 		Sleep(5000);//睡 眠一分钟
// 		win_log("");
// 	}
	return 0;
}

int init_run()
{

	SERVICE_TABLE_ENTRY ServiceTable[2];

	ServiceTable[0].lpServiceName =const_cast<char*>(g_wcsvrname.c_str());
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	// 启动服务的控制分派机线程  
	StartServiceCtrlDispatcher(ServiceTable);

	return 0;
}

int kill_proc()
{
	//ServiceStatus.
	string strcmd = "TASKKILL /F /IM " + g_wcsvrexecname;
	exec_cmd(strcmd);
	return 0;
}

int init_param()
{
	TCHAR _szPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, _szPath, MAX_PATH);	
	g_wcsvrpath = _szPath;
	g_wcsvrexecname = g_wcsvrpath.substr(g_wcsvrpath.rfind('\\')+1);
	g_wcsvrname = g_wcsvrexecname.substr(0,g_wcsvrexecname.find('.'));

	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;

	return 0;
}

void win_log(string msg)
{
	FILE *fp; time_t t;
	if ((fp = fopen("C:\\test.log", "a")) >= 0)
	{
		if (msg.empty())
		{
			t = time(0);
			fprintf(fp, "Im here at %sn\n", asctime(localtime(&t)));
		}
		else
		{
			fprintf(fp, "msg:%s\n", msg.c_str());
		}

		fclose(fp);
	}
}

int install_svr()
{
	uninstall_svr();
	string strcmd = "SC create " + g_wcsvrname + " binPath= \"" + g_wcsvrpath + " -run\" DisplayName= \"" + g_wcsvrname + "\"";
	exec_cmd(strcmd);
	strcmd = "SC start " + g_wcsvrname;
	exec_cmd(strcmd);
	return 0;
}

int uninstall_svr()
{
	//kill_proc();
	string strcmd;
	strcmd = "SC stop " + g_wcsvrname;
	exec_cmd(strcmd);
	strcmd = "SC delete " + g_wcsvrname;
	exec_cmd(strcmd);
	return 0;
}

int exec_cmd(string cmd)
{
	WinExec(cmd.c_str(), SW_NORMAL);


	//system(cmd.c_str());  

	
// 	SHELLEXECUTEINFO ShExecInfo = { 0 };
// 	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
// 	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
// 	ShExecInfo.hwnd = NULL;
// 	ShExecInfo.lpVerb = NULL;
// 	ShExecInfo.lpFile = "cmd.exe";
// 	ShExecInfo.lpParameters = cmd.c_str();
// 	ShExecInfo.lpDirectory = "";
// 	ShExecInfo.nShow = SW_HIDE;
// 	ShExecInfo.hInstApp = NULL;
// 	ShellExecuteEx(&ShExecInfo);

	return 0;
}



/*
DWORD WINAPI srv_core_thread(LPVOID para)
{
	int i = 0;
	for (;;)
	{
		if (uaquit)
		{
			break;
		}
		//fprintf(log, "srv_core_thread run time count:%d\n", i++);
		Sleep(5000);
		win_log("");
	}
	return NULL;
}

void WINAPI ServiceHandler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		uaquit = 1;
		//add you quit code here  
		//if (log != NULL)
		//	fclose(log);
		break;
	default:
		return;
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}
}

void WINAPI service_main(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hServiceStatusHandle = RegisterServiceCtrlHandler(g_wcsvrname.c_str(), ServiceHandler);
	if (hServiceStatusHandle == 0)
	{
		DWORD nError = GetLastError();
	}
	//add your init code here  
	//log = fopen("d:\\test.txt", "w");
	//add your service thread here  
	HANDLE task_handle = CreateThread(NULL, NULL, srv_core_thread, NULL, NULL, NULL);
	if (task_handle == NULL)
	{
		//fprintf(log, "create srv_core_thread failed\n");
		win_log("create srv_core_thread failed");
	}

	// Initialization complete - report running status   
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 9000;
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}

}

*/

//////////////////////////////////////////////////////////////////////////


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	Init();
	
	dwThreadID = ::GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{const_cast<char*>(g_wcsvrname.c_str()), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (stricmp(lpCmdLine, "/install") == 0)
	{
		Install();
	}
	else if (stricmp(lpCmdLine, "/uninstall") == 0)
	{
		Uninstall();
	}
	else
	{
		if (!::StartServiceCtrlDispatcher(st))
		{
			//LogEvent(_T("Register Service Main Function Error!"));
			win_log("Register Service Main Function Error!");
		}
	}

	return 0;
}

//*********************************************************  
//Functiopn:            Init  
//Description:            初始化  
//Calls:                main  
//Called By:                  
//Table Accessed:                  
//Table Updated:                  
//Input:                  
//Output:                  
//Return:                  
//Others:                  
//History:                  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
void Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
}

//*********************************************************  
//Functiopn:            ServiceMain  
//Description:            服务主函数，这在里进行控制对服务控制的注册  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:  
//Output:  
//Return:  
//Others:  
//History:  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
void WINAPI ServiceMain()
{
	// Register the control request handler  
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	//注册服务控制  
	hServiceStatus = RegisterServiceCtrlHandler(const_cast<char*>(g_wcsvrname.c_str()), ServiceStrl);
	if (hServiceStatus == NULL)
	{
		//LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(hServiceStatus, &status);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	//模拟服务的运行，10后自动退出。应用时将主要任务放于此即可  
	int i = 0;
	while (i < 10)
	{

		//新建文件  
		FILE * fp0 = fopen("c:/tt.txt", "a");
		fclose(fp0);
		Sleep(10000);
		i++;
	}
	//  

	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
	//LogEvent(_T("Service stopped"));
}

//*********************************************************  
//Functiopn:            ServiceStrl  
//Description:            服务控制主函数，这里实现对服务的控制，  
//                        当在服务管理器上停止或其它操作时，将会运行此处代码  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:                dwOpcode：控制服务的状态  
//Output:  
//Return:  
//Others:  
//History:  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		//LogEvent(_T("Bad service request"));
		;
	}
}
//*********************************************************  
//Functiopn:            IsInstalled  
//Description:            判断服务是否已经被安装  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:  
//Output:  
//Return:  
//Others:  
//History:  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//打开服务控制管理器  
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务  
		SC_HANDLE hService = ::OpenService(hSCM, const_cast<char*>(g_wcsvrname.c_str()), SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

//*********************************************************  
//Functiopn:            Install  
//Description:            安装服务函数  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:  
//Output:  
//Return:  
//Others:  
//History:  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
BOOL Install()
{
	if (IsInstalled())
		return TRUE;

	//打开服务控制管理器  
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		//MessageBox(NULL, _T("Couldn't open service manager"), const_cast<char*>(g_wcsvrname.c_str()), MB_OK);
		return FALSE;
	}

	// Get the executable file path  
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务  
	SC_HANDLE hService = ::CreateService(
		hSCM, const_cast<char*>(g_wcsvrname.c_str()), const_cast<char*>(g_wcsvrname.c_str()),
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), const_cast<char*>(g_wcsvrname.c_str()), MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

//*********************************************************  
//Functiopn:            Uninstall  
//Description:            删除服务函数  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:  
//Output:  
//Return:  
//Others:  
//History:  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
BOOL Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), const_cast<char*>(g_wcsvrname.c_str()), MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, const_cast<char*>(g_wcsvrname.c_str()), SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), const_cast<char*>(g_wcsvrname.c_str()), MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务  
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent(_T("Service could not be deleted"));
	return FALSE;
}

//*********************************************************  
//Functiopn:            LogEvent  
//Description:            记录服务事件  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:  
//Output:  
//Return:  
//Others:  
//History:  
//            <author>niying <time>2006-8-10        <version>        <desc>  
//*********************************************************  
void LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, const_cast<char*>(g_wcsvrname.c_str()));
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}


#endif // WIN32
