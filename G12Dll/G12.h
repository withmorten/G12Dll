typedef int _bool;
#define bool _bool
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

#define DEBUG_BREAK() { if (IsDebuggerPresent()) __asm int 3 }

// first 4 bytes of each version (with debug info)
#define BASE_ADDR (0x00401000)

#define GOTHIC101D_VER		"1.01d_christmas_edition"
#define GOTHIC101E_VER		"1.01e_christmas_edition"
#define GOTHIC104D_VER		"1.04d"
#define GOTHIC107C_VER		"1.07c"
#define GOTHIC108J_VER		"1.08j"
#define GOTHIC108K_VER		"1.08k"
#define GOTHIC108KMOD_VER	"1.08k_mod"
#define GOTHIC112F_VER		"1.12f"
#define GOTHIC130FIX_VER	"1.30 (fix)"
#define GOTHIC26FIX_VER		"2.6 (fix)"
#define SPACER141_VER		"1.41"
#define SPACER142G_VER		"1.42g"
#define SPACER150_VER		"1.5"
#define SPACER26MOD_VER		"2.6 (mod)"

#define GOTHIC101D		(!memcmp((void *)0x008C36D8, GOTHIC101D_VER, sizeof(GOTHIC101D_VER)))			// Gothic.exe  1.01d_christmas_edition
#define GOTHIC101E		(!memcmp((void *)0x00856014, GOTHIC101E_VER, sizeof(GOTHIC101E_VER)))			// Gothic.exe  1.01e_christmas_edition
#define GOTHIC104D		(!memcmp((void *)0x0083ED94, GOTHIC104D_VER, sizeof(GOTHIC104D_VER)))			// Gothic.exe  1.04d
#define GOTHIC107C		(!memcmp((void *)0x0085A2CC, GOTHIC107C_VER, sizeof(GOTHIC107C_VER)))			// Gothic.exe  1.07c
#define GOTHIC108J		(!memcmp((void *)0x0086088C, GOTHIC108J_VER, sizeof(GOTHIC108J_VER)))			// Gothic.exe  1.08j
#define GOTHIC108K		(!memcmp((void *)0x0086088C, GOTHIC108K_VER, sizeof(GOTHIC108K_VER)))			// Gothic.exe  1.08k
#define GOTHIC108KMOD	(!memcmp((void *)0x00839D18, GOTHIC108KMOD_VER, sizeof(GOTHIC108KMOD_VER)))		// Gothic.exe  1.08k_mod
#define GOTHIC112F		(!memcmp((void *)0x0087F910, GOTHIC112F_VER, sizeof(GOTHIC112F_VER)))			// Gothic.exe  1.12f
#define GOTHIC130FIX	(!memcmp((void *)0x0088F84C, GOTHIC130FIX_VER, sizeof(GOTHIC130FIX_VER)))		// Gothic2.exe 1.30 (fix)
#define GOTHIC26FIX		(!memcmp((void *)0x0089DA98, GOTHIC26FIX_VER, sizeof(GOTHIC26FIX_VER)))			// Gothic2.exe 2.6 (fix)
#define SPACER141		(!memcmp((void *)0x00945870, SPACER141_VER, sizeof(SPACER141_VER)))				// Spacer.exe  1.41
#define SPACER142G		(!memcmp((void *)0x0095D83C, SPACER142G_VER, sizeof(SPACER142G_VER)))			// Spacer.exe  1.42g
#define SPACER150		(!memcmp((void *)0x00946880, SPACER150_VER, sizeof(SPACER150_VER)))				// Spacer.exe  1.5
#define SPACER26MOD		(!memcmp((void *)0x00966990, SPACER26MOD_VER, sizeof(SPACER26MOD_VER)))			// Spacer2.exe 2.6 (mod)

// file offset to image offset
#define _fo(file_addr) (file_addr + BASE_ADDR)

char G12Cwd[MAX_PATH];
char G12LogPath[MAX_PATH];
char G12IniPath[MAX_PATH];

UINT G12GetPrivateProfileInt(LPCTSTR lpKeyName, INT nDefault)
{
	return GetPrivateProfileInt(G12DLL_NAME, lpKeyName, nDefault, G12IniPath);
}

BOOL G12GetPrivateProfileBool(LPCTSTR lpKeyName, INT nDefault)
{
	return G12GetPrivateProfileInt(lpKeyName, nDefault);
}

DWORD G12GetPrivateProfileString(LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	return GetPrivateProfileString(G12DLL_NAME, lpKeyName, lpDefault, lpReturnedString, nSize, G12IniPath);
}

FLOAT G12GetPrivateProfileFloat(LPCTSTR lpKeyName, LPCTSTR lpDefault)
{
	CHAR lpReturnedString[MAX_PATH];

	G12GetPrivateProfileString(lpKeyName, lpDefault, lpReturnedString, sizeof(lpReturnedString));

	return (FLOAT)atof(lpReturnedString);
}

FILE *conin, *conout, *fileout = NULL;

void G12Log(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	printf("%s: ", G12DLL_NAME);
	vprintf(format, args);

	if (fileout)
	{
		fprintf(fileout, "%s.dll: ", G12DLL_NAME);
		vfprintf(fileout, format, args);
		_fflush_nolock(fileout);
	}

	va_end(args);
}

#define printf G12Log

void G12AllocConsole(void)
{
	// does not belong here, but whatever
	_getcwd(G12Cwd, sizeof(G12Cwd));

	strncpy(G12LogPath, G12Cwd, sizeof(G12LogPath));
	strncat(G12LogPath, "\\G12.log", sizeof(G12LogPath));

	fileout = fopen(G12LogPath, "wb");

	strncpy(G12IniPath, G12Cwd, sizeof(G12IniPath));
	strncat(G12IniPath, "\\G12.ini", sizeof(G12IniPath));

	if (G12GetPrivateProfileBool("AllocConsole", FALSE))
	{
		AllocConsole();
		freopen_s(&conin, "conin$", "r", stdin);
		freopen_s(&conout, "conout$", "w", stdout);
		freopen_s(&conout, "conout$", "w", stderr);
	}
}
