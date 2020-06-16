#ifndef SIDPLAYFP_PROTOS_H
#define SIDPLAYFP_PROTOS_H

#ifndef EXEC_TYPES_H
# include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
# include <utility/tagitem.h>
#endif

/**
 * Pass config to player allocator.
 */
struct SidplayFpNew
{
    BYTE Emulation;               /** default RESIDFP */
    BYTE SidModel;                /** default MOS6581 */
    BOOL SidModelForce;           /** default FALSE */
    BYTE MachineType;             /** default PAL */
    BOOL MachineTypeForce;        /** default FALSE */
    BYTE CiaModel;                /** default MOS6526 */
    BYTE SamplingMethod;          /** default INTERPOLATE */
    BOOL Filter;                  /** default FALSE */
    BOOL Digiboost;               /** default FALSE */
    BYTE Playback;                /** default MONO */
    FLOAT ResidBias;              /** default 0.5 */
    FLOAT ResidFpFilterCurve6581; /** default 0.5 */
    FLOAT ResidFpFilterCurve8580; /** default 0.5 */
    ULONG AudioFrequency;         /** default 44100 */
    STRPTR Database;              /** default NULL */
};

/* SidplayFpNew defines */
#define SFN_EMULATION_RESIDFP 0
#define SFN_EMULATION_RESID 1
#define SFN_SID_MODEL_MOS6581 0
#define SFN_SID_MODEL_MOS8580 1
#define SFN_MACHINE_TYPE_PAL 0
#define SFN_MACHINE_TYPE_NTSC 1
#define SFN_MACHINE_TYPE_PAL_M 2
#define SFN_MACHINE_TYPE_OLD_NTSC 3
#define SFN_MACHINE_TYPE_DREAN 4
#define SFN_CIA_MODEL_MOS6526 0
#define SFN_CIA_MODEL_MOS8521 1
#define SFN_SAMPLING_METHOD_INTERPOLATE 0
#define SFN_SAMPLING_METHOD_RESAMPLE_INTERPOLATE 1
#define SFN_PLAYBACK_MONO 0
#define SFN_PLAYBACK_STEREO 1

/* SidplayFpInfo defines */
#define SFI_CLOCK_UNKNOWN 0
#define SFI_CLOCK_PAL 1
#define SFI_CLOCK_NTSC 2
#define SFI_CLOCK_ANY 3
#define SFI_SID_MODE_UNKNOWN 0
#define SFI_SID_MODEL_MOS6581 1
#define SFI_SID_MODEL_MOS8580 2
#define SFI_SID_MODEL_ANY 3
#define SFI_COMPATIBILITY_C64 0
#define SFI_COMPATIBILITY_PSID 1
#define SFI_COMPATIBILITY_R64 2
#define SFI_COMPATIBILITY_BASIC 3

/* tags */
#define SFA_DUMMY                  (TAG_USER + 0x4000)
#define SFA_Emulation              (SFNA_DUMMY + 1)
#define SFA_SidModel               (SFNA_DUMMY + 2)
#define SFA_SidModelForce          (SFNA_DUMMY + 3)
#define SFA_MachineType            (SFNA_DUMMY + 4)
#define SFA_MachineTypeForce       (SFNA_DUMMY + 5)
#define SFA_CiaModel               (SFNA_BUMMY + 6)
#define SFA_SamplingMethod         (SFNA_DUMMY + 7)
#define SFA_Filter                 (SFNA_DUMMY + 8)
#define SFA_Digiboost              (SFNA_DUMMY + 9)
#define SFA_ResidBias              (SFNA_DUMMY + 10)
#define SFA_ResidFpFilterCurve6581 (SFNA_DUMMY + 11)
#define SFA_ResidFpFilterCurve8580 (SFNA_DUMMY + 12)
#define SFA_AudioFrequency         (SFNA_DUMMY + 13)
#define SFA_Database               (SFNA_DUMMY + 14)

struct SidplayFpInfo
{
    STRPTR Title;
    STRPTR Author;
	STRPTR Released;
	BYTE SidModel;
    BYTE SidsRequired;
    BYTE ClockSpeed;
    BYTE Compability;
};

struct SidplayFpError
{
    LONG Error;
    STRPTR Message;
};

struct SidplayFp
{
    BOOL Loaded;                 /** Is player in loaded state */
    struct SidplayFpError Error; /** Error information from library */
    APTR PrivateData;            /** Private C++ stuff is here */
    ULONG Reserved[5];           /** Some reserved bytes if needed in the future */
};

/**
 * Allocates player.
 *
 * NewPlayer - struct SidplayFpNew to initialize player
 *
 * returns: struct SidplayFp allocated by library. Use SidplayFpFree() to free it
 */
struct SidplayFp *SidplayFpCreate( struct SidplayFpNew *NewPlayer );

/**
 * Allocates player with Tags. If no tags given default values used instead.
 *
 * NewPlayer - struct SidplayFpNew to initialize player
 *
 * returns: struct SidplayFp allocated by library. Use SidplayFpFree() to free it
 */
struct SidplayFp *SidplayFpCreateTagList( struct TagItem *Item );

/**
 * Wrapper for SidplayFpCreateTagList()
 */
struct SidplayFp *SidplayFpCreateTags( Tag, ... );

/**
 * Free player
 *
 * Player - allocated SidplayFp struct
 */
void SidplayFpFree( struct SidplayFp *Player );

/**
 * Sets rom files to player. Optional function. Required only for basic tunes.
 * It is safe to free allocated memory for kernal, basic and chagen right
 * after call. Settings roms before init is also safe.
 *
 * Player
 * Kernal - kernal rom. Size should be exacly 8192 bytes
 * Basic - basic rom. Size should be exacly 8192 bytes
 * Chargen - chargen rom. Can be NULL. If set, size should be exacly 4096 bytes
 *
 * returns: TRUE if success
 */
BOOL SidplayFpSetRoms( struct SidplayFp *Player, CONST UBYTE *Kernal, CONST UBYTE *Basic, CONST UBYTE *Chargen);

/**
 * Initializes player with given SID-data. After load it is possible to check
 * information from players SidplayFpInfo struct. It is safe to free SidData
 * after call.
 *
 * Player - allocated SidplayFp struct
 * SidData - whole SID-file in memory.
 * SidSize - size of the SidData
 *
 * returns: TRUE if success
 */
BOOL SidplayFpInit( struct SidplayFp *Player, CONST UBYTE *SidData, ULONG SidSize );

/**
 * Get current song information. Result is valid between init and free. There
 * are some subtune related information. After chancing subtune this shoould be
 * called again.
 *
 * Player
 *
 * returns: const pointer to struct SidplayFpInfo. Changing or freeing it is not
 *  alloved..
 */
CONST struct SidplayFpInfo *SidplayFpInfo( struct SidplayFp *Player );

/**
 * Get current subtune
 *
 * Player
 *
 * returns: zero if fails
 */
UWORD SidplayFpCurrentSubtune( struct SidplayFp *Player );

/**
 * Number of the subtunes in song
 *
 * Player - allocated SidplayFp struct
 *
 * returns: zero if fails
 */
UWORD SidplayFpSubtunes( struct SidplayFp *Player );

/**
 * Set SID subtune
 *
 * Player - allocated SidplayFp struct
 * Subtune - SIDs subtune starting from 1. 0 sets default tune or tune number 1.
 *
 * returns: TRUE if success
 */
BOOL SidplayFpSetSubtune( struct SidplayFp *Player, UWORD Subtune );

/**
 * Fill sample buffer with signed 16bit audio samples. Library tries fill as
 * many samples as SampleCount tells there is space.
 *
 * Player - allocated SidplayFp struct
 * SampleBuffer - Preallocated buffer of samples to library to fill
 * SampleCount - SampleBuffer size in samples.
 *
 * returns: Number of samples really put to SampleBuffer. In case of error negative value.
 */
LONG SidplayFpPlay( struct SidplayFp *Player, SHORT *SampleBuffer, LONG SampleCount );

/**
 * Get current subtune length. Requires HVSC database.
 *
 * Player - allocated SidplayFp struct
 *
 * returns: subtune length in ms. Negative if error.
 */
LONG SidplayFpCurrentSubtuneLength( struct SidplayFp *Player );

/**
 * Get current subtune length. Requires HVSC database.
 *
 * Player - allocated SidplayFp struct
 * Subtune - subtune to get length for
 *
 * returns: subtune length in ms. Negative if error.
 */
LONG SidplayFpSubtuneLength( struct SidplayFp *Player, UWORD Subtune );

#endif /* SIDPLAYFP_PROTOS_H */
