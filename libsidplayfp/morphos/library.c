#include <exec/resident.h>

#include <proto/exec.h>
#include <proto/utility.h>

#include "library.h"
#include "playsidcpp.h"

#define COMPILE_VERSION  1
#define COMPILE_REVISION 0
#define COMPILE_DATE     "(24.6.2020)"
#define PROGRAM_VER      "1.0"

/**********************************************************************
	Function prototypes
**********************************************************************/

static struct Library *LIB_Open(void);
static BPTR            LIB_Close(void);
static BPTR            LIB_Expunge(void);
static ULONG           LIB_Reserved(void);
static struct Library *LIB_Init(struct LibPlaysidFpLibrary *LibBase, BPTR SegList, struct ExecBase *LibPlaysidFpSysBase);

/**********************************************************************
	Library Header
**********************************************************************/

static const char VerString[]	= "\0$VER: playsidfp.library " PROGRAM_VER " "COMPILE_DATE;
static const char LibName[]	= "playsidfp.library";

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
	(APTR)   playsidcpp_create_taglist,
	(APTR)   playsidcpp_create_tags,
	(APTR)   playsidcpp_free,
    (APTR)   playsidcpp_set_roms,
	(APTR)   playsidcpp_init,
	(APTR)   playsidcpp_play,
	(APTR)   playsidcpp_speed,
	(APTR)   playsidcpp_mute,
	(APTR)   playsidcpp_filter,
	(APTR)   playsidcpp_time,
	(APTR)   playsidcpp_tune_info,
	(APTR)   playsidcpp_current_subtune,
	(APTR)   playsidcpp_subtunes,
	(APTR)   playsidcpp_subtune_set,
	(APTR)   playsidcpp_tune_md5,
	(APTR)   -1,

	(APTR)   FUNCARRAY_END
};

static const struct LibPlaysidFpInitData InitData	=
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
	sizeof(struct LibPlaysidFpLibrary),
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

static struct Library *LIB_Init(struct LibPlaysidFpLibrary *LibBase, BPTR SegList, struct ExecBase *LibPlaysidFpSysBase)
{
	LibBase->SegList	= SegList;
	SysBase				= LibPlaysidFpSysBase;
	return &LibBase->Library;
}

/**********************************************************************
	RemoveLibrary
**********************************************************************/

static VOID RemoveLibrary(struct LibPlaysidFpLibrary *LibBase)
{
	Remove(&LibBase->Library.lib_Node);
	FreeMem((APTR)((ULONG)(LibBase) - (ULONG)(LibBase->Library.lib_NegSize)), LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize);
}

/**********************************************************************
	LIB_Expunge
 ********************************************************************/

static BPTR LIB_Expunge(void)
{
	struct LibPlaysidFpLibrary *LibBase = (struct LibPlaysidFpLibrary *)REG_A6;

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
	struct LibPlaysidFpLibrary *LibBase = (struct LibPlaysidFpLibrary *)REG_A6;
	BPTR	SegList	= 0;

	Forbid();
	LibBase->Library.lib_OpenCnt--;

	if (LibBase->Library.lib_Flags & LIBF_DELEXP && LibBase->Library.lib_OpenCnt == 0)
	{
		CallFuncArray(__dtrslist);
        CloseLibrary((struct Library *)UtilityBase);
        SegList = LibBase->SegList;
		RemoveLibrary(LibBase);
	}
	Permit();

	return SegList;
}

/**********************************************************************
	LIB_Open
**********************************************************************/

static struct Library *LIB_Open(void)
{
	struct LibPlaysidFpLibrary *LibBase = (struct LibPlaysidFpLibrary *)REG_A6;

	Forbid();
	LibBase->Library.lib_Flags &= ~LIBF_DELEXP;
	LibBase->Library.lib_OpenCnt++;
	Permit();

	if (LibBase->Library.lib_OpenCnt == 1)
	{
        UtilityBase = OpenLibrary("utility.library", 0L);
		CallFuncArray(__ctrslist);
	}

	return &LibBase->Library;
}
