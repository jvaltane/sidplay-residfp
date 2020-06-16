#include <exec/resident.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include "library.h"
#include "sid.h"

#define COMPILE_VERSION  1
#define COMPILE_REVISION 0
#define COMPILE_DATE     "(15.6.2020)"
#define PROGRAM_VER      "1.0"

/**********************************************************************
	Function prototypes
**********************************************************************/

static struct Library *LIB_Open(void);
static BPTR            LIB_Close(void);
static BPTR            LIB_Expunge(void);
static ULONG           LIB_Reserved(void);
static struct Library *LIB_Init(struct LibSidplayFpLibrary *LibBase, BPTR SegList, struct ExecBase *LibSidplayFpSysBase);

/**********************************************************************
	Library Header
**********************************************************************/

static const char VerString[]	= "\0$VER: sidplayfp.library " PROGRAM_VER " "COMPILE_DATE;
static const char LibName[]	= "sidplayfp.library";

static const APTR FuncTable[] =
{
	(APTR)   FUNCARRAY_BEGIN,

	// Start m68k block for system functions.

	(APTR)   FUNCARRAY_32BIT_NATIVE,
	(APTR)   LIB_Open,
	(APTR)   LIB_Close,
	(APTR)   LIB_Expunge,
	(APTR)   LIB_Reserved,
	(APTR)   -1,

	// Start sysv block for user functions.

	(APTR)   FUNCARRAY_32BIT_SYSTEMV,
	(APTR)   sid_create,
	(APTR)   sid_create_taglist,
	(APTR)   sid_create_tags,
	(APTR)   sid_free,
	(APTR)   sid_init,
	(APTR)   sid_play,
	(APTR)   sid_tune_info,
	(APTR)   sid_current_subtune,
	(APTR)   sid_subtunes,
	(APTR)   sid_subtune_set,
	(APTR)   sid_subtune_length,
	(APTR)   sid_subtune_length_current,
	(APTR)   -1,

	(APTR)   FUNCARRAY_END
};

static const struct LibSidplayFpInitData InitData	=
{
	0xa0,8,  NT_LIBRARY,0,
	0xa0,9,  0xfb,0,					/* 0xfb -> priority -5 */
	0x80,10, (ULONG)&LibName[0],
	0xa0,14, LIBF_SUMUSED|LIBF_CHANGED,0,
	0x90,20, COMPILE_VERSION,
	0x90,22, COMPILE_REVISION,
	0x80,24, (ULONG)&VerString[7],
	0
};

static const ULONG InitTable[] =
{
	sizeof(struct LibSidplayFpLibrary),
	(ULONG)	FuncTable,
	(ULONG)	&InitData,
	(ULONG)	LIB_Init
};

const struct Resident RomTag	=
{
	RTC_MATCHWORD,
	(struct Resident *)&RomTag,
	(struct Resident *)&RomTag+1,
	RTF_AUTOINIT | RTF_PPC | RTF_EXTENDED,
	COMPILE_VERSION,
	NT_LIBRARY,
	0,
	(char *)&LibName[0],
	(char *)&VerString[7],
	(APTR)&InitTable[0],
	COMPILE_REVISION, NULL
};

/**********************************************************************
	Globals
**********************************************************************/

const ULONG __abox__	= 1;
#if 1
int ThisRequiresConstructorHandling = 1;
void *libnix_mempool = 0L;
void exit(int status)
{
}
#endif

struct ExecBase *SysBase;

LONG NoExecute(void)
{
	return -1;
}

extern const void (*__ctrslist[])(void);
extern const void (*__dtrslist[])(void);

struct FuncSeg
{
	ULONG size;
	struct FuncSeg *next;
};

static void CallFuncArray(const void (*FuncArray[])(void))
{
	struct FuncSeg *seg;
	int i, num;

	seg = (struct FuncSeg *)(((IPTR)FuncArray) - sizeof(struct FuncSeg));
	num = (seg->size - sizeof(struct FuncSeg)) / sizeof(APTR);

	for (i=0; (i < num) && FuncArray[i]; i++)
	{
		if (FuncArray[i] != ((const void (*)(void))-1))
			(*FuncArray[i])();
	}
}


/**********************************************************************
	LIB_Reserved
**********************************************************************/

static ULONG LIB_Reserved(void)
{
	return 0;
}

/**********************************************************************
	LIB_Init
**********************************************************************/

static struct Library *LIB_Init(struct LibSidplayFpLibrary *LibBase, BPTR SegList, struct ExecBase *LibSidplayFpSysBase)
{
	LibBase->SegList	= SegList;
	SysBase				= LibSidplayFpSysBase;
	return &LibBase->Library;
}

/**********************************************************************
	RemoveLibrary
**********************************************************************/

static VOID RemoveLibrary(struct LibSidplayFpLibrary *LibBase)
{
	Remove(&LibBase->Library.lib_Node);
	FreeMem((APTR)((ULONG)(LibBase) - (ULONG)(LibBase->Library.lib_NegSize)), LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize);
}

/**********************************************************************
	LIB_Expunge
 ********************************************************************/

static BPTR LIB_Expunge(void)
{
	struct LibSidplayFpLibrary *LibBase = (struct LibSidplayFpLibrary *)REG_A6;

	Forbid();
	if (LibBase->Library.lib_OpenCnt == 0)
	{
		BPTR seglist = LibBase->SegList;

		RemoveLibrary(LibBase);
		Permit();
		return seglist;
	}

	LibBase->Library.lib_Flags |= LIBF_DELEXP;
	Permit();
	return 0;
}

/**********************************************************************
	LIB_Close
**********************************************************************/

static BPTR LIB_Close(void)
{
	struct LibSidplayFpLibrary *LibBase = (struct LibSidplayFpLibrary *)REG_A6;
	BPTR	SegList	= 0;

	Forbid();
	LibBase->Library.lib_OpenCnt--;

	if (LibBase->Library.lib_Flags & LIBF_DELEXP && LibBase->Library.lib_OpenCnt == 0)
	{
		CallFuncArray(__dtrslist);
		SegList = LibBase->SegList;
		RemoveLibrary(LibBase);
        CloseLibrary((struct Library *)DOSBase);
	}
	Permit();

	return SegList;
}

/**********************************************************************
	LIB_Open
**********************************************************************/

static struct Library *LIB_Open(void)
{
	struct LibSidplayFpLibrary *LibBase = (struct LibSidplayFpLibrary *)REG_A6;

	Forbid();
	LibBase->Library.lib_Flags &= ~LIBF_DELEXP;
	LibBase->Library.lib_OpenCnt++;
	Permit();

	if (LibBase->Library.lib_OpenCnt == 1)
	{
        DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 0L);
		CallFuncArray(__ctrslist);
	}

	return &LibBase->Library;
}
