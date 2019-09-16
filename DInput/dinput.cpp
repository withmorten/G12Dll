#include "..\G12Dll\G12Core.h"

struct dinput_dll
{
	HMODULE dll;
	FARPROC DirectInputCreateA;
	FARPROC DirectInputCreateEx;
	FARPROC DirectInputCreateW;
	FARPROC DllCanUnloadNow;
	FARPROC DllGetClassObject;
	FARPROC DllRegisterServer;
	FARPROC DllUnregisterServer;
} dinput;

WRAPPER void _DirectInputCreateA() { VARJMP(dinput.DirectInputCreateA); }
WRAPPER void _DirectInputCreateEx() { VARJMP(dinput.DirectInputCreateEx); }
WRAPPER void _DirectInputCreateW() { VARJMP(dinput.DirectInputCreateW); }
WRAPPER void _DllCanUnloadNow() { VARJMP(dinput.DllCanUnloadNow); }
WRAPPER void _DllGetClassObject() { VARJMP(dinput.DllGetClassObject); }
WRAPPER void _DllRegisterServer() { VARJMP(dinput.DllRegisterServer); }
WRAPPER void _DllUnregisterServer() { VARJMP(dinput.DllUnregisterServer); }

void DInput(HINSTANCE hInst)
{
	TCHAR DInputDllPath[MAX_PATH];

	GetEnvironmentVariable("windir", DInputDllPath, MAX_PATH);
	strcat_s(DInputDllPath, "\\System32\\dinput.dll");

	dinput.dll = LoadLibrary(DInputDllPath);

	dinput.DirectInputCreateA = GetProcAddress(dinput.dll, "DirectInputCreateA");
	dinput.DirectInputCreateEx = GetProcAddress(dinput.dll, "DirectInputCreateEx");
	dinput.DirectInputCreateW = GetProcAddress(dinput.dll, "DirectInputCreateW");
	dinput.DllCanUnloadNow = GetProcAddress(dinput.dll, "DllCanUnloadNow");
	dinput.DllGetClassObject = GetProcAddress(dinput.dll, "DllGetClassObject");
	dinput.DllRegisterServer = GetProcAddress(dinput.dll, "DllRegisterServer");
	dinput.DllUnregisterServer = GetProcAddress(dinput.dll, "DllUnregisterServer");
}

void Init(void)
{
	char path[MAX_PATH];
	char *p;

	FILE *f = fopen("g12.cfg", "r");

	if (f)
	{
		while (fgets(path, MAX_PATH, f))
		{
			if (*path != '\0' && *path != '#' && *path != ';')
			{
				for (p = path; *p; p++)
				{
					if (*p == '\r') *p = '\0';

					if (*p == '\n') *p = '\0';
				}

				LoadLibrary(path);
			}
		}

		fclose(f);
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DInput(hInst);

		Init();
	}

	if (reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(dinput.dll);
	}

	return TRUE;
}
