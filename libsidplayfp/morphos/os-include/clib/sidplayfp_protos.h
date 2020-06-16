#ifndef SIDPLAYFP_PROTOS_H
#define SIDPLAYFP_PROTOS_H

#ifndef EXEC_TYPES_H
# include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
# include <utility/tagitem.h>
#endif

/* defines */
#define SF_EMULATION_RESIDFP 0
#define SF_EMULATION_RESID 1
#define SF_SID_MODEL_MOS6581 0
#define SF_SID_MODEL_MOS8580 1
#define SF_MACHINE_TYPE_PAL 0
#define SF_MACHINE_TYPE_NTSC 1
#define SF_MACHINE_TYPE_PAL_M 2
#define SF_MACHINE_TYPE_OLD_NTSC 3
#define SF_MACHINE_TYPE_DREAN 4
#define SF_CIA_MODEL_MOS6526 0
#define SF_CIA_MODEL_MOS8521 1
#define SF_SAMPLING_METHOD_INTERPOLATE 0
#define SF_SAMPLING_METHOD_RESAMPLE_INTERPOLATE 1
#define SF_PLAYBACK_MONO 0
#define SF_PLAYBACK_STEREO 1

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

/* Tags */
#define SFA_DUMMY                  (TAG_USER + 0x4000)
#define SFA_Emulation              (SFA_DUMMY + 1)
#define SFA_SidModel               (SFA_DUMMY + 2)
#define SFA_SidModelForce          (SFA_DUMMY + 3)
#define SFA_MachineType            (SFA_DUMMY + 4)
#define SFA_MachineTypeForce       (SFA_DUMMY + 5)
#define SFA_CiaModel               (SFA_DUMMY + 6)
#define SFA_SamplingMethod         (SFA_DUMMY + 7)
#define SFA_Filter                 (SFA_DUMMY + 8)
#define SFA_Digiboost              (SFA_DUMMY + 9)
#define SFA_Playback               (SFA_DUMMY + 10)
#define SFA_ResidBias              (SFA_DUMMY + 11)
#define SFA_ResidFpFilterCurve6581 (SFA_DUMMY + 12)
#define SFA_ResidFpFilterCurve8580 (SFA_DUMMY + 13)
#define SFA_AudioFrequency         (SFA_DUMMY + 14)

/* Errors */
#define SFE_OK                       (0UL)
#define SFE_PARAMETERS               (1UL)
#define SFE_PLAYER_NOT_ALLOCATED     (2UL)
#define SFE_PLAYER_NOT_INITIALIZED   (3UL)
#define SFE_PLAYER_TUNE_INFO         (4UL)
#define SFE_PLAYER_TUNE_LOAD         (5UL)
#define SFE_PLAYER_SUBTUNE           (6UL)
#define SFE_PLAYER_CONFIG            (7UL)
#define SFE_PLAYER_EMULATION_RESID   (8UL)
#define SFE_PLAYER_EMULATION_RESIDFP (9UL)

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

struct SidplayFp
{
    BOOL Initialized;            /** Is player in initialised state */
    ULONG Error;                 /** Error information from library */
    APTR PrivateData;            /** Private C++ stuff is here */
    ULONG Reserved[5];           /** Some reserved bytes if needed in the future */
};

/**
 * Allocates player with Tags. If no tags given default values used instead.
 *
 * Items - Tag item list.
 *
 * returns: struct SidplayFp allocated by library. Use SidplayFpFree() to free it
 */
struct SidplayFp *SidplayFpCreateA( struct TagItem *Items );

/**
 * Wrapper for SidplayFpCreateTagList()
 */
struct SidplayFp *SidplayFpCreate( Tag, ... );

/**
 * Free player
 *
 * Player - allocated SidplayFp struct
 */
void SidplayFpFree( struct SidplayFp *Player );

/**
 * Sets rom files to player. Optional function. Required only for basic based
 * tunes. It is safe to free allocated memory for kernal, basic and chagen right
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
 * Initializes player with given SID-data. It is safe to free SidData after
 * call.
 *
 * Player - allocated SidplayFp struct
 * SidData - whole SID-file in memory.
 * SidSize - size of the SidData
 *
 * returns: TRUE if success
 */
BOOL SidplayFpInit( struct SidplayFp *Player, CONST UBYTE *SidData, ULONG SidSize );

/**
 * Get current song information. Result is valid after init. When changing
 * subtune call this again.
 *
 * Player
 *
 * returns: const pointer to struct SidplayFpInfo. Do not change or free it.
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
 * Set SID subtune. Subtune numbering starts from 1. 0 is special case and sets
 * default tune or tune number 1 depends of SID-file type.
 *
 * Player - allocated SidplayFp struct
 * Subtune - SIDs subtune
 *
 * returns: TRUE if success
 */
BOOL SidplayFpSetSubtune( struct SidplayFp *Player, UWORD Subtune );

/**
 * Fill sample buffer with signed 16bit audio samples. Library tries fill as
 * many samples as SampleCount tells.
 *
 * Player - allocated SidplayFp struct
 * SampleBuffer - Preallocated buffer of samples to library to fill
 * SampleCount - SampleBuffer size in samples.
 *
 * returns: Number of samples really put to SampleBuffer. In case of error
 *  negative value.
 */
LONG SidplayFpPlay( struct SidplayFp *Player, SHORT *SampleBuffer, LONG SampleCount );

/**
 * Get tunes MD5 sum. MD5 sum can be used for example to get song length from
 * HVSC database. This should be called after init or if subtune is changed.
 *
 * Player
 *
 * returns: MD5 sum of tune. NULL if fails.
 */
CONST_STRPTR SidplayFpTuneMD5( struct SidplayFp *Player );

#endif /* SIDPLAYFP_PROTOS_H */
