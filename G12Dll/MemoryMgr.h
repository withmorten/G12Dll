typedef uintptr_t addr;

#define WRAPPER __declspec(naked)
#define DEPRECATED __declspec(deprecated)
#define EAXJMP(a) { _asm mov eax, a _asm jmp eax }
#define VARJMP(a) { _asm jmp a }
#define WRAPARG(a) UNREFERENCED_PARAMETER(a)

#define RET(n)	__asm push	n __asm retn
#define ASM(name) void __declspec(naked) name(void)

#define NOVMT __declspec(novtable)
#define SETVMT(a) *((DWORD_PTR *)this) = (DWORD_PTR)a

#pragma warning(disable:4731) // -- suppress C4731:"frame pointer register 'ebp' modified by inline assembly code"

#define XCALL(uAddr)			\
	__asm { mov esp, ebp	}	\
	__asm { pop ebp			}	\
	__asm { mov eax, uAddr	}	\
	__asm { jmp eax			}

#define FIELD(type, var, offset) *(type *)((unsigned char *)var + offset)

template<typename T> inline void Patch(DWORD address, T value)
{
	DWORD dwProtect[2];
	VirtualProtect((void *)address, sizeof(T), PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	*(T *)address = value;
	VirtualProtect((void *)address, sizeof(T), dwProtect[0], &dwProtect[1]);
}

inline void PatchBytes(DWORD address, void *value, size_t nCount)
{
	DWORD dwProtect[2];
	VirtualProtect((void *)address, nCount, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	memcpy((void *)address, value, nCount);
	VirtualProtect((void *)address, nCount, dwProtect[0], &dwProtect[1]);
}

inline void ReadBytes(DWORD address, void *out, size_t nCount)
{
	DWORD dwProtect[2];
	VirtualProtect((void *)address, nCount, PAGE_EXECUTE_READ, &dwProtect[0]);
	memcpy(out, (void *)address, nCount);
	VirtualProtect((void *)address, nCount, dwProtect[0], &dwProtect[1]);
}

inline void Nop(DWORD address, size_t nCount = 1)
{
	DWORD dwProtect[2];
	VirtualProtect((void *)address, nCount, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	memset((void *)address, 0x90, nCount);
	VirtualProtect((void *)address, nCount, dwProtect[0], &dwProtect[1]);
}

inline void NopTo(DWORD address, DWORD to)
{
	Nop(address, to - address);
}

enum
{
	PATCH_CALL,
	PATCH_JUMP,
	PATCH_NOTHING,
	HOOK_SIZE = 5,
};

template<typename T> inline void InjectHook(DWORD address, T hook, int nType = PATCH_NOTHING)
{
	DWORD dwProtect[2];
	switch (nType)
	{
	case PATCH_JUMP:
		VirtualProtect((void *)address, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		*(BYTE *)address = 0xE9;
		break;
	case PATCH_CALL:
		VirtualProtect((void *)address, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		*(BYTE *)address = 0xE8;
		break;
	default:
		VirtualProtect((void *)((DWORD)address + 1), HOOK_SIZE - 1, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		break;
	}
	DWORD dwHook;
	// DWORD PTR for casting 8 byte function pointers down to 4
	_asm
	{
		mov	eax, DWORD PTR hook
		mov DWORD PTR dwHook, eax
	}

	*(ptrdiff_t *)((DWORD)address + 1) = (DWORD)dwHook - (DWORD)address - HOOK_SIZE;

	if (nType == PATCH_NOTHING)
		VirtualProtect((void *)((DWORD)address + 1), HOOK_SIZE - 1, dwProtect[0], &dwProtect[1]);
	else
		VirtualProtect((void *)address, HOOK_SIZE, dwProtect[0], &dwProtect[1]);
}

inline void PatchJump(DWORD address, DWORD to)
{
	InjectHook(address, to, PATCH_JUMP);
}

inline void ExtractCall(void *dst, addr a)
{
	*(addr *)dst = (addr)(*(addr *)(a + 1) + a + 5);
}

template<typename T> inline void InterceptCall(void *dst, T func, addr a)
{
	ExtractCall(dst, a);
	InjectHook(a, func);
}

template<typename T> inline void InterceptVmethod(void *dst, T func, addr a)
{
	*(addr *)dst = *(addr *)a;
	Patch(a, func);
}
