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
    STRPTR RomKernal;             /** default NULL */
    STRPTR RomBasic;              /** default NULL */
    STRPTR RomChargen;            /** default NULL */
    FLOAT ResidBias;              /** default 0.5 */
    FLOAT ResidFpFilterCurve6581; /** default 0.5 */
    FLOAT ResidFpFilterCurve8580; /** default 0.5 */
    ULONG AudioFrequency;         /** default 44100 */
    STRPTR Database;              /** default NULL */
};

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
#define SFA_RomKernal              (SFNA_DUMMY + 10)
#define SFA_RomBasic               (SFNA_DUMMY + 11)
#define SFA_RomChargen             (SFNA_DUMMY + 12)
#define SFA_ResidBias              (SFNA_DUMMY + 13)
#define SFA_ResidFpFilterCurve6581 (SFNA_DUMMY + 14)
#define SFA_ResidFpFilterCurve8580 (SFNA_DUMMY + 15)
#define SFA_AudioFrequency         (SFNA_DUMMY + 16)
#define SFA_Database               (SFNA_DUMMY + 17)

struct SidplayFpInfo
{
    STRPTR Title;
    STRPTR Artist;
    UWORD SubtuneCount;
};

struct SidplayFpError
{
    STRPTR Message;
    LONG Error;
};

struct SidplayFp
{
    BOOL Loaded;                 /** Is player in loaded state */
    UWORD CurrentSubtune;        /** Currently loaded subtune. On error or not loded zero */
    struct SidplayFpError error; /** Error information from library */
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
 * Allocates player with Tags. Ifno tags given default values used instead.
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
 * Loads SID. After load it is possible to get information and call play.
 * After call it ispossible to free SidData.
 *
 * Player - allocated SidplayFp struct
 * SidData - whole SID-file in memory.
 * SidSize - size of the SidData
 *
 * returns: TRUE if sguccess
 */
BOOL SidplayFpLoad( struct SidplayFp *Player, CONST UBYTE *SidData, ULONG SidSize );

/**
 * Get SID tune info. Fills SidplayFpInfo stucture.
 *
 * Player - allocated SidplayFp struct
 * Info - struct SidplayFpInfo. User must alloc this before pall.
 *
 * returns: TRUE if success
 */
BOOL SidplayFpInfo( struct SidplayFp *Player, struct SidplayFpInfo *Info );

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
 * Free player
 *
 * Player - allocated SidplayFp struct
 */
void SidplayFpFree( struct SidplayFp *Player );

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
