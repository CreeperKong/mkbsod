#include<windows.h>
#include<Tlhelp32.h>
#include<stdlib.h>
#include<stdio.h>
DWORD GetProcessID(char *FileName) //��ȡ����ID
{
    HANDLE myhProcess;
    PROCESSENTRY32 mype;
    BOOL mybRet;
    //���н��̿���
    myhProcess=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); //TH32CS_SNAPPROCESS�������н���
    //��ʼ���̲���
    mybRet=Process32First(myhProcess,&mype);
    //ѭ���Ƚϣ��ó�ProcessID
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
    //ȡ��debugȨ��
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
