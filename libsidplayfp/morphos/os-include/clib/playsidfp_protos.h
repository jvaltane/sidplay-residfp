#ifndef PLAYSIDFP_PROTOS_H
#define PLAYSIDFP_PROTOS_H

#ifndef EXEC_TYPES_H
# include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
# include <utility/tagitem.h>
#endif

/* defines for tags */
#define PF_EMULATION_RESIDFP 0
#define PF_EMULATION_RESID 1
#define PF_SID_MODEL_MOS6581 0
#define PF_SID_MODEL_MOS8580 1
#define PF_MACHINE_TYPE_PAL 0
#define PF_MACHINE_TYPE_NTSC 1
#define PF_MACHINE_TYPE_PAL_M 2
#define PF_MACHINE_TYPE_OLD_NTSC 3
#define PF_MACHINE_TYPE_DREAN 4
#define PF_CIA_MODEL_MOS6526 0
#define PF_CIA_MODEL_MOS8521 1
#define PF_SAMPLING_METHOD_INTERPOLATE 0
#define PF_SAMPLING_METHOD_RESAMPLE_INTERPOLATE 1
#define PF_PLAYBACK_MONO 0
#define PF_PLAYBACK_STEREO 1

/* PlaysidFpInfo defines */
#define PFI_CLOCK_UNKNOWN 0
#define PFI_CLOCK_PAL 1
#define PFI_CLOCK_NTSC 2
#define PFI_CLOCK_ANY 3
#define PFI_SID_MODE_UNKNOWN 0
#define PFI_SID_MODEL_MOS6581 1
#define PFI_SID_MODEL_MOS8580 2
#define PFI_SID_MODEL_ANY 3
#define PFI_COMPATIBILITY_C64 0
#define PFI_COMPATIBILITY_PSID 1
#define PFI_COMPATIBILITY_R64 2
#define PFI_COMPATIBILITY_BASIC 3

/** Tags
 *
 * See also: PF_XXX defines.
 *
 * If tag is not given or its value is invalid default is used.
 *
 * PFA_Emulation
 *
 *   Type of emulation. Possible options are RESID or RESIDFP
 *   Default: PF_EMULATION_RESIDFP
 *
 * PFA_SidModel
 *
 *   Type of SID model. Possible options MOS8580 or MOS6581
 *   Default: PF_SID_MODEL_MOS6581
 *
 * PFA_SidModelForce
 *
 *   Forces the SID model and does not let player choose. Boolean value.
 *   Default: FALSE
 *
 * PFA_MachineType
 *
 *   Machine type. Possible options PAL, NTSC, PAL_M, OLD_NTSC and DREAN.
 *   Default: PF_MACHINE_TYPE_PAL
 *
 * PFA_MachineTypeForce
 *
 *   Forces the machine type and does not let player choose. Boolean value.
 *   Default: FALSE
 *
 * PFA_CiaModel
 *
 *   CIA model. Possible options MOS6526 and MOS8521.
 *   Default PF_CIA_MODEL_MOS6526
 *
 * PFA_SamplingMethod
 *
 *   Sampling method. INTERPOLATE and RESAMPLE_INTERPOLATE
 *   Default: PF_SAMPLING_METHOD_INTERPOLATE
 *
 * PFA_Filter
 *
 *   Use filter. Boolean value.
 *   Default value: FALSE
 *
 * PFA_Digiboost
 *
 *   Use SID digiboost. For MOS8580 only, Boolean value.
 *   Defalt: FALSE
 *
 * PFA_Playback
 *
 *   Playback mode. MONO or STEREO
 *   Default: PF_PLAYPACK_MONO
 *
 * PFA_ResidBias
 *
 *   Bias of the RESID. Floating point value.
 *   Default: 0.5
 *
 * PFA_ResidFpFilterCurve6581
 *
 *   RESIDFPs MOS6581 filtere curve. Floating point value.
 *   Default: 0.5
 *
 * PFA_ResidFpFilterCurve8580
 *
 *   RESIDFPs MOS8580 filtere curve. Floating point value.
 *   Default: 0.5
 *
 * PFA_AudioFrequency
 *
 *   Frequency of the playback. Some reasonable unssigned intereg value.
 *   Default: 44100
 */
#define PFA_DUMMY                  (TAG_USER + 0x4000)
#define PFA_Emulation              (PFA_DUMMY + 1)
#define PFA_SidModel               (PFA_DUMMY + 2)
#define PFA_SidModelForce          (PFA_DUMMY + 3)
#define PFA_MachineType            (PFA_DUMMY + 4)
#define PFA_MachineTypeForce       (PFA_DUMMY + 5)
#define PFA_CiaModel               (PFA_DUMMY + 6)
#define PFA_SamplingMethod         (PFA_DUMMY + 7)
#define PFA_Filter                 (PFA_DUMMY + 8)
#define PFA_Digiboost              (PFA_DUMMY + 9)
#define PFA_Playback               (PFA_DUMMY + 10)
#define PFA_ResidBias              (PFA_DUMMY + 11)
#define PFA_ResidFpFilterCurve6581 (PFA_DUMMY + 12)
#define PFA_ResidFpFilterCurve8580 (PFA_DUMMY + 13)
#define PFA_AudioFrequency         (PFA_DUMMY + 14)

/* Errors */
#define PFE_OK                       (0UL)
#define PFE_PARAMETERS               (1UL)
#define PFE_PLAYER_NOT_ALLOCATED     (2UL)
#define PFE_PLAYER_NOT_INITIALIZED   (3UL)
#define PFE_PLAYER_TUNE_INFO         (4UL)
#define PFE_PLAYER_TUNE_LOAD         (5UL)
#define PFE_PLAYER_SUBTUNE           (6UL)
#define PFE_PLAYER_CONFIG            (7UL)
#define PFE_PLAYER_EMULATION_RESID   (8UL)
#define PFE_PLAYER_EMULATION_RESIDFP (9UL)

struct PlaysidFpInfo
{
    STRPTR Title;
    STRPTR Author;
	STRPTR Released;
	BYTE SidModel;
    BYTE SidsRequired;
    BYTE ClockSpeed;
    BYTE Compability;
};

struct PlaysidFp
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
 * returns: struct PlaysidFp allocated by library. Use PlaysidFpFree() to free it
 */
struct PlaysidFp *PlaysidFpCreateA( struct TagItem *Items );

/**
 * Wrapper for PlaysidFpCreateA()
 */
struct PlaysidFp *PlaysidFpCreate( Tag, ... );

/**
 * Free player
 *
 * Player - allocated PlaysidFp struct
 */
void PlaysidFpFree( struct PlaysidFp *Player );

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
BOOL PlaysidFpSetRoms( struct PlaysidFp *Player, CONST UBYTE *Kernal, CONST UBYTE *Basic, CONST UBYTE *Chargen);

/**
 * Initializes player with given SID-data. It is safe to free SidData after
 * call.
 *
 * Player - allocated PlaysidFp struct
 * SidData - whole SID-file in memory.
 * SidSize - size of the SidData
 *
 * returns: TRUE if success
 */
BOOL PlaysidFpInit( struct PlaysidFp *Player, CONST UBYTE *SidData, ULONG SidSize );

/**
 * Get current song information. Result is valid after init. When changing
 * subtune call this again.
 *
 * Player
 *
 * returns: const pointer to struct PlaysidFpInfo. Do not change or free it.
 */
CONST struct PlaysidFpInfo *PlaysidFpInfo( struct PlaysidFp *Player );

/**
 * Get current subtune
 *
 * Player
 *
 * returns: zero if fails
 */
UWORD PlaysidFpCurrentSubtune( struct PlaysidFp *Player );

/**
 * Number of the subtunes in song
 *
 * Player - allocated PlaysidFp struct
 *
 * returns: zero if fails
 */
UWORD PlaysidFpSubtunes( struct PlaysidFp *Player );

/**
 * Set SID subtune. Subtune numbering starts from 1. 0 is special case and sets
 * default tune or tune number 1 depends of SID-file type.
 *
 * Player - allocated PlaysidFp struct
 * Subtune - SIDs subtune
 *
 * returns: TRUE if success
 */
BOOL PlaysidFpSetSubtune( struct PlaysidFp *Player, UWORD Subtune );

/**
 * Fill sample buffer with signed 16bit audio samples. Library tries fill as
 * many samples as SampleCount tells.
 *
 * Player - allocated PlaysidFp struct
 * SampleBuffer - Preallocated buffer of samples to fill
 * SampleCount - SampleBuffer size in samples
 *
 * returns: Number of samples really put to SampleBuffer. In case of error
 *  negative value.
 */
LONG PlaysidFpPlay( struct PlaysidFp *Player, SHORT *SampleBuffer, LONG SampleCount );

/**
 * Kind of fast forward. Sets play speed as percentage. For example with
 * Percent value of 200 engine just skips every second sample while playing.
 *
 * Player
 * Percent - How fast to play tune. Valid values are between 100 and 3200.
 *
 * returns: FALSE if fails.
 */
BOOL PlaysidFpSpeed( struct PlaysidFp *Player, USHORT Percent );

/**
 * Mute or un mute voice channel of chosen SID. Three SIDs and voice channels
 * are supported.
 *
 * Player
 * SidNumber - Number of SID. Pssible values are 0-2.
 * VoiceChannel - Number of channel to mute. SIDs has three channels so possible values 0-2
 * Mute - TRUE to mute, FALSE to unmute
 *
 * returns: FALSE if fails.
 */
BOOL PlaysidFpMute( struct PlaysidFp *Player, UBYTE SidNumber, UBYTE VoiceChannel, BOOL Mute );

/**
 * Set filter on or off.
 *
 * Player
 * Filter - TRUE sets filter on.
 *
 * returns: FALSE if fails.
 */
BOOL PlaysidFpFilter( struct PlaysidFp *Player, BOOL Filter );

/**
 * Get current playing time.
 *
 * Player
 *
 * return: current playing time in ms. Negative if fails.
 */
LONG PlaysidFpTime( struct PlaysidFp *Player );

/**
 * Get tunes MD5 sum. MD5 sum can be used for example to get song length from
 * HVSC database. This should be called after init or if subtune is changed.
 *
 * Player
 *
 * returns: MD5 sum of tune. NULL if fails.
 */
CONST_STRPTR PlaysidFpTuneMD5( struct PlaysidFp *Player );


#endif /* PLAYSIDFP_PROTOS_H */
