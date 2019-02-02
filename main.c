#include<windows.h>
#include<Tlhelp32.h>
#include<stdlib.h>
#include<stdio.h>
DWORD GetProcessID(char *FileName) //获取进程ID
{
    HANDLE myhProcess;
    PROCESSENTRY32 mype;
    BOOL mybRet;
    //进行进程快照
    myhProcess=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); //TH32CS_SNAPPROCESS快照所有进程
    //开始进程查找
    mybRet=Process32First(myhProcess,&mype);
    //循环比较，得出ProcessID
    while(mybRet)
    {
        if(strcmp(FileName,mype.szExeFile)==0)
            return mype.th32ProcessID;
        else
            mybRet=Process32Next(myhProcess,&mype);
    }
    return 0;
}

int main(void)
{
    //取得debug权限
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID Luid;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid);
    tp.PrivilegeCount=1;
    tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
    tp.Privileges[0].Luid=Luid;
    AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
    //Sleep(120000);
    DebugActiveProcess(GetProcessID("csrss.exe"));
    return EXIT_SUCCESS;
}
