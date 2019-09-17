typedef int _bool;
#define bool _bool
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

// first 4 bytes of each version (with debug info)
#define BASE_ADDR (0x00401000)

#define GOTHIC101E		(*(unsigned int *)BASE_ADDR == 0x7F36E4A1) // Gothic.exe  1.01e_christmas_edition
#define GOTHIC104D		(*(unsigned int *)BASE_ADDR == 0x7D76B4A1) // Gothic.exe  1.04d
#define GOTHIC107C		(*(unsigned int *)BASE_ADDR == 0x7F06D4A1) // Gothic.exe  1.07c
#define GOTHIC108J		(*(unsigned int *)BASE_ADDR == 0x7F66D4A1) // Gothic.exe  1.08j
#define GOTHIC108K		(*(unsigned int *)BASE_ADDR == 0x7F56E4A1) // Gothic.exe  1.08k
#define GOTHIC108KM		(*(unsigned int *)BASE_ADDR == 0xE0EC05C7) // Gothic.exe  1.08k_mod
#define GOTHIC112F		(*(unsigned int *)BASE_ADDR == 0x8136D4A1) // Gothic.exe  1.12f
#define GOTHIC130		(*(unsigned int *)BASE_ADDR == 0x385C05C7) // Gothic2.exe 1.30
#define GOTHIC26		(*(unsigned int *)BASE_ADDR == 0x1E1405C7) // Gothic2.exe 2.6
#define SPACER141		(*(unsigned int *)BASE_ADDR == 0x8C3064A1) // Spacer.exe  1.41
#define SPACER142G		(*(unsigned int *)BASE_ADDR == 0x8DB064A1) // Spacer.exe  1.42g
#define SPACER150		(*(unsigned int *)BASE_ADDR == 0x8C4064A1) // Spacer.exe  1.50
#define SPACER26		(*(unsigned int *)BASE_ADDR == 0xF6E805C7) // Spacer2.exe 2.6

// file offset to image offset
#define _fo(file_addr) (file_addr + BASE_ADDR)

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

FILE *conin, *conout, *out = NULL;

void G12Log(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	printf("%s: ", G12DLL_NAME);
	vprintf(format, args);

	if (out)
	{
		fprintf(out, "%s.dll: ", G12DLL_NAME);
		vfprintf(out, format, args);
		fflush(out);
	}

	va_end(args);
}

#define printf G12Log

void G12AllocConsole(void)
{
	// does not belong here, but whatever
	_getcwd(G12LogPath, sizeof(G12LogPath));
	strncat(G12LogPath, "\\G12.log", sizeof(G12LogPath));

	out = fopen(G12LogPath, "wb");

	_getcwd(G12IniPath, sizeof(G12IniPath));
	strncat(G12IniPath, "\\G12.ini", sizeof(G12IniPath));

	if (G12GetPrivateProfileBool("AllocConsole", FALSE))
	{
		AllocConsole();
		freopen_s(&conin, "conin$", "r", stdin);
		freopen_s(&conout, "conout$", "w", stdout);
		freopen_s(&conout, "conout$", "w", stderr);
	}
}
