#ifndef SIDPLAYFP_PROTOS_H
#define SIDPLAYFP_PROTOS_H

#ifndef EXEC_TYPES_H
# include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
# include <utility/tagitem.h>
#endif

struct SidplayFpNew
{
	BYTE sfn_Emulation;
	BYTE sfn_SidModel;
	BOOL sfn_SidModelForce;
	BYTE sfn_MachineType;
	BOOL sfn_MachineTypeForce;
	BYTE sfn_CiaModel;
	BYTE sfn_SamplingMethod;
	BOOL sfn_Filter;
	BOOL sfn_Digiboost;
	BYTE sfn_Playback;
	STRPTR sfn_RomKernal;
	STRPTR sfn_RomBasic;
	STRPTR sfn_RomChargen;
	FLOAT sfn_ResidBias;
	FLOAT sfn_ResidFpFilterCurve6581;
	FLOAT sfn_ResidFpFilterCurve8580;
    ULONG sfn_AudioFrequency;
	STRPTR sfn_Database;
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

#define SFNA_DUMMY                  (TAG_USER)
#define SFNA_Emulation              (SFNA_DUMMY + 1)
#define SFNA_SidModel               (SFNA_DUMMY + 2)
#define SFNA_SidModelForce          (SFNA_DUMMY + 3) 
#define SFNA_MachineType            (SFNA_DUMMY + 4)
#define SFNA_MachineTypeForce       (SFNA_DUMMY + 5)
#define SFNA_CiaModel               (SFNA_BUMMY + 6)
#define SFNA_SamplingMethod         (SFNA_DUMMY + 7)
#define SFNA_Filter                 (SFNA_DUMMY + 8)
#define SFNA_Digiboost              (SFNA_DUMMY + 9)
#define SFNA_RomKernal              (SFNA_DUMMY + 10)
#define SFNA_RomBasic               (SFNA_DUMMY + 11) 
#define SFNA_RomChargen             (SFNA_DUMMY + 12)
#define SFNA_ResidBias              (SFNA_DUMMY + 13)
#define SFNA_ResidFpFilterCurve6581 (SFNA_DUMMY + 14)
#define SFNA_ResidFpFilterCurve8580 (SFNA_DUMMY + 15)
#define SFNA_AudioFequency          (SFNA_DUMMY + 16)
#define SFNA_Database               (SFNA_DUMMY + 17)

struct SidplayFpSongInfo
{
    STRPTR sfsi_Title;
    STRPTR sfsi_Artist;
    UWORD sfsi_Subtunes;
};

struct SidplayFpError
{
    STRPTR sfe_Message;
    LONG sfe_Error;
};

struct SidplayFp
{
    BOOL sf_Loaded; // is in loaded state
    struct SidplayFpSongInfo sf_SongInfo;
    struct SidplayFpError sf_error;
    APTR sf_PrivateData; /* C++ stuff is here */
    ULONG sf_Reserved[5];
};

struct SidplayFp *SidplayFpCreate( struct SidplayFpNew *NewPlayer );
struct SidplayFp *SidplayFpCreateTagList( struct TagItem *Item );
struct SidplayFp *SidplayFpCreateTags( ULONG, ... );

/**
 * Loads SID. After load it is possible to get information and call play.
 *
 * Player - created SidplayFp struct 
 * SidData - whole SID in memory
 * SidDataLen - lenght of the SidData
 *
 * returns: TRUE if success 
 */
BOOL SidplayFpLoad( struct SidplayFp *Player, BYTE *SidData, LONG SidDataLen );

/**
 * Get SID tune info. Fills sf_SongInfo stucture.
 *
 * Player - created SidplayFp struct 
 *
 * returns: TRUE if success 
 */
BOOL SidplayFpInfo( struct SidplayFp *Player );

/**
 * Number of the subtunes in song
 *
 * Player - previously created SidplayFp struct
 *
 * returns: zero if fails
 */
UWORD SidplayFpSubtunes( struct SidplayFp *Player );

/**
 * Set SID subtune
 *
 * Player - previously created SidplayFp struct
 * Subtune - SIDs subtune starting from 1. 0 sets default tune or tune number 1.
 *
 * returns: TRUE if success 
 */
BOOL SidplayFpSetSubtune( struct SidplayFp *Player, UWORD Subtune );

/**
 * Fill sample buffer with signed  16bit audio samples
 *
 * Player - previously created SidplayFp struct
 * SampleBuffer - Preallocated buffer of samples
 * SampleBufferLength - length of the SampleBuffer
 *
 * returns: Number of samples put to SampleBuffer. In casee of error negative value.
 */
LONG SidplayFpPlay( struct SidplayFp *Player, SHORT *SampleBuffer, LONG SampleBufferLength ); // return number of samples

/**
 * Free SidplayFp
 *
 * Player - previously created SidplayFp struct
 */
void SidplayFpFree( struct SidplayFp *Player );

LONG SidplayFpCurrentSubtuneLength( struct SidplayFp *Player );
LONG SidplayFpSubtuneLength( struct SidplayFp *Player, UWORD Subtune );

#endif /* SIDPLAYFP_PROTOS_H */
