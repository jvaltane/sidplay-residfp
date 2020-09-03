/**
 * PlaysidFp - Copyright (c) 2020 Joni Valtanen <jvaltane at kapsi dot fi>
 *
 * PlaysidFp is C implementation of SidplayFp.
 *
 * License: LGPL2
 */

#ifndef PLAYSIDFP_PROTOS_H
#define PLAYSIDFP_PROTOS_H

#ifndef EXEC_TYPES_H
# include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
# include <utility/tagitem.h>
#endif

/** Defines for PlaysidFpCreate Tags
 *
 * See also: PlaysidFpCreate Tags
 *
 */
#define PF_EMULATION_RESIDFP                    0 /** Use ReSIDfp emulation */
#define PF_EMULATION_RESID                      1 /** Use ReSID emulation */
#define PF_SID_MODEL_MOS6581                    0 /** Try to use MOS6581 as SID model with emulation. */
#define PF_SID_MODEL_MOS8580                    1 /** Try to use MOS8589 as SID model with emulation. */
#define PF_MACHINE_TYPE_PAL                     0 /** Try to use PAL machine model with emulation. */
#define PF_MACHINE_TYPE_NTSC                    1 /** Try to use NTSC machine model with emulation. */
#define PF_MACHINE_TYPE_PAL_M                   2 /** Try to use PAL_M machine model with emulation. */
#define PF_MACHINE_TYPE_OLD_NTSC                3 /** Try to use OLD_NTSC machine model with emulation. */
#define PF_MACHINE_TYPE_DREAN                   4 /** Try to use DREAN machine model with emulation. */
#define PF_CIA_MODEL_MOS6526                    0 /** Try to use MOS6526 as CIA model with emulation. */
#define PF_CIA_MODEL_MOS8521                    1 /** Try to use MOS8521 as CIA model with emulation. */
#define PF_SAMPLING_METHOD_INTERPOLATE          0 /** Use interpolate as sampling method with emulation. */
#define PF_SAMPLING_METHOD_RESAMPLE_INTERPOLATE 1 /** Use resample interpolate as sampling method with emulation. */
#define PF_PLAYBACK_MONO                        0 /** Use mono playback. */
#define PF_PLAYBACK_STEREO                      1 /** Use stereo playback. */

/** PlaysidFpInfo defines
 *
 * Defines for struct PlaysidFpInfo.
 *
 * See also: struct PlaysidFpInfo
 */
#define PFI_CLOCK_UNKNOWN       0 /** SID tune does not tell preferable clock */
#define PFI_CLOCK_PAL           1 /** PAL is prefeerable clock */
#define PFI_CLOCK_NTSC          2 /** NTSC is prefeerable clock */
#define PFI_CLOCK_ANY           3 /** Any clock should work */
#define PFI_SID_MODEL_UNKNOWN   0 /** SID tune does not tell which SID model is the best */
#define PFI_SID_MODEL_MOS6581   1 /** SID tune prefers MOS6581 SID model */
#define PFI_SID_MODEL_MOS8580   2 /** SID tune prefers MOS8580 SID model */
#define PFI_SID_MODEL_ANY       3 /** Any SID model should work */
#define PFI_COMPATIBILITY_C64   0 /** SID works with C64 */
#define PFI_COMPATIBILITY_PSID  1 /** PSID player is required */
#define PFI_COMPATIBILITY_R64   2 /** R64 player is required */
#define PFI_COMPATIBILITY_BASIC 3 /** Basic used to create SID tune */

/** PlaysidFpCreate Tags
 *
 * See also: Defines for PlaysidFpCreate Tags and PlaysidFpCreate.
 *
 * If tag is not given or its value is invalid, default is used.
 *
 */
#define PFA_DUMMY                  (TAG_USER + 0x4000)
#define PFA_Emulation              (PFA_DUMMY + 1)  /** Type of emulation. Possible options are RESID or RESIDFP.
                                                        Default: PF_EMULATION_RESIDFP */
#define PFA_SidModel               (PFA_DUMMY + 2)  /** Type of SID model. Possible options MOS8580 or MOS6581.
                                                        Default: PF_SID_MODEL_MOS6581 */
#define PFA_SidModelForce          (PFA_DUMMY + 3)  /** Forces the SID model and does not let player choose. Boolean value.
                                                        Default: FALSE */
#define PFA_MachineType            (PFA_DUMMY + 4)  /** Machine type. Possible options PAL, NTSC, PAL_M, OLD_NTSC and DREAN.
                                                        Default: PF_MACHINE_TYPE_PAL */
#define PFA_MachineTypeForce       (PFA_DUMMY + 5)  /** Forces the machine type and does not let player choose. Boolean value.
                                                        Default: FALSE */
#define PFA_CiaModel               (PFA_DUMMY + 6)  /** CIA model. Possible options MOS6526 and MOS8521.
                                                        Default PF_CIA_MODEL_MOS6526 */
#define PFA_SamplingMethod         (PFA_DUMMY + 7)  /** Sampling method. INTERPOLATE and RESAMPLE_INTERPOLATE.
                                                        Default: PF_SAMPLING_METHOD_INTERPOLATE */
#define PFA_Filter                 (PFA_DUMMY + 8)  /** Use filter. Boolean value.
                                                        Default value: FALSE */
#define PFA_Digiboost              (PFA_DUMMY + 9)  /** Use SID digiboost. For MOS8580 only, Boolean value.
                                                        Default: FALSE */
#define PFA_Playback               (PFA_DUMMY + 10) /** Playback mode. Mono or stereo.
                                                        Default: PF_PLAYPACK_MONO */
#define PFA_ResidBias              (PFA_DUMMY + 11) /** Bias of the RESID. Floating point value.
                                                        Default: 0.5 */
#define PFA_ResidFpFilterCurve6581 (PFA_DUMMY + 12) /** RESIDFPs MOS6581 filtere curve. Floating point value.
                                                        Default: 0.5 */
#define PFA_ResidFpFilterCurve8580 (PFA_DUMMY + 13) /** RESIDFPs MOS8580 filtere curve. Floating point value.
                                                        Default: 0.5 */
#define PFA_AudioFrequency         (PFA_DUMMY + 14) /** Frequency of the playback. Some reasonable unssigned intereg value.
                                                        Default: 44100 */

/** Errors
 *
 * Functions set PlaysidFP structs Error if those fails. Error tells more
 * specific information why function fails.
 *
 */
#define PFE_OK                0  /** Everything is OK. */
#define PFE_PARAMETERS        1  /** Parameters for function are invalid. PlaysidFpCreate() is exception it sets default values if invalid values are passed. */
#define PFE_NOT_ALLOCATED     2  /** PlaysidFpCreate() has failed, not called or some other reason Player is not valid. */
#define PFE_EMULATION_RESID   3  /** ReSID init in PlaysidFpInit() failed. */
#define PFE_EMULATION_RESIDFP 4  /** ReSIDfp init in PlaysidFpInit() failed. */
#define PFE_NOT_INITIALIZED   5  /** This is set if function which requires PlaysidInit() is called and init has failed or not called at all. */
#define PFE_TUNE_INFO         6  /** Could not get tune info. */
#define PFE_TUNE_LOAD         7  /** Could not load tune. */
#define PFE_TUNE_MD5          8  /** Could not get md5 of tune. */
#define PFE_SUBTUNE           9  /** Could not get current subtune. */
#define PFE_SUBTUNE_SET       10 /** Could not set current subtune. */
#define PFE_SUBTUNES          11 /** Could not get number of subtunes. */
#define PFE_CONFIG            12 /** Players config has problem. */
#define PFE_TIME              13 /** Could not get current time. */
#define PFE_SPEED             14 /** Could not set tune speed. */
#define PFE_PLAY              15 /** Could not get buffer from play. */
#define PFE_SET_ROMS          16 /** Could not set roms. */
#define PFE_MUTE              17 /** Could not set mute value. */
#define PFE_FILTER            18 /** Could not set filter. */

/** struct PlaysidFpInfo
 *
 *  Info about SID tune. Returned by PlaysidFpTuneInfo.
 */
struct PlaysidFpInfo
{
    STRPTR Title;      /** Title of the tune */
    STRPTR Author;     /** Author of the tune */
	STRPTR Released;   /** Releeased information of the tune */
	BYTE SidModel;     /** SID model of the tune */
    BYTE SidsRequired; /** Number of SIDs required by tune */
    BYTE ClockSpeed;   /** Clock speed of the SID tune */
    BYTE Compability;  /** Compability of the tune */
};

/** struct PlaysidFp
 *
 *   Player structure.
 */
struct PlaysidFp
{
    BOOL Initialized;  /** Is player in initialised state */
    BYTE Error;        /** Error information from library */
    APTR PrivateData;  /** Private data */
    ULONG Reserved[5]; /** Some reserved bytes if needed in the future */
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
 *
 * Example:
 *
 *     PlaysidFp *player = PlaysidFPCreate (
 *       PFA_Emulation, PF_EMULATION_RESIDFP,
 *       NULL);
 */
struct PlaysidFp *PlaysidFpCreate( Tag, ... );

/**
 * Free player
 *
 * Player - allocated PlaysidFp struct
 *
 * Example:
 *
 *     PlaysidFp *player = PlaysidFPCreate (
 *       PFA_Emulation, PF_EMULATION_RESIDFP,
 *       NULL);
 *     if (player == NULL) return -1;
 *     PlaysidFpFree (player);
 *
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
 * HVSC database. This should be called after init.
 *
 * Player
 *
 * returns: MD5 sum of tune. NULL if fails.
 */
CONST_STRPTR PlaysidFpTuneMD5( struct PlaysidFp *Player );


#endif /* PLAYSIDFP_PROTOS_H */
