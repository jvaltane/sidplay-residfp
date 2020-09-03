#include <sidplayfp/sidplayfp.h>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/builders/residfp.h>
#include <sidplayfp/builders/resid.h>
#include <sidplayfp/SidDatabase.h>

extern "C" {
#include <proto/exec.h>
#include <proto/utility.h>
}

#include <string>
#include <stdlib.h>

#include "playsidcpp.h"

struct PlaysidFpNew
{
    BYTE Emulation;
    BYTE SidModel;
    BOOL SidModelForce;
    BYTE MachineType;
    BOOL MachineTypeForce;
    BYTE CiaModel;
    BYTE SamplingMethod;
    BOOL Filter;
    BOOL Digiboost;
    BYTE Playback;
    FLOAT ResidBias;
    FLOAT ResidFpFilterCurve6581;
    FLOAT ResidFpFilterCurve8580;
    ULONG AudioFrequency;
};

typedef struct {
  sidplayfp *sp;
  SidTune *tune;
  SidConfig config;
  BYTE emulation;
  SidConfig::sid_model_t sid_model;
  bool sid_model_force;
  SidConfig::c64_model_t c64_model;
  bool c64_model_force;
  SidConfig::cia_model_t cia_model;
  SidConfig::sampling_method_t sampling_method;
  bool filter;
  bool digiboost;
  SidConfig::playback_t playback;
  float resid_bias;
  float residfp_filter_curve_6581;
  float residfp_filter_curve_8580;
  std::string database;
  uint32_t audio_frequency;
  struct PlaysidFpInfo *info;
  int32_t current_subtune;
} playsidcpp_priv_t;

extern "C" {

static struct PlaysidFp *create (struct PlaysidFpNew *sfn);

static SidConfig::sid_model_t sid_model_to_sidplay_config (BYTE sid_model);
static SidConfig::c64_model_t c64_model_to_sidplay_config (BYTE c64_model);
static SidConfig::cia_model_t cia_model_to_sidplay_config (BYTE cia_model);
static SidConfig::sampling_method_t sampling_method_to_sidplay_config (BYTE sampling_method);
static SidConfig::playback_t playback_to_sidplay_config (BYTE playback);

static BYTE tune_info_sid_model_convert(SidTuneInfo::model_t s);
static BYTE tune_info_clock_speed_convert(SidTuneInfo::clock_t c);
static BYTE tune_info_compability_convert(SidTuneInfo::compatibility_t c);

struct PlaysidFp *create (struct PlaysidFpNew *sfn)
{
  struct PlaysidFp *s = static_cast<struct PlaysidFp *> (AllocVec(sizeof(struct PlaysidFp), MEMF_PUBLIC|MEMF_CLEAR));
  if (s == NULL) {
    return NULL;
  }

  s->Initialized = FALSE;

  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (AllocVec(sizeof(playsidcpp_priv_t), MEMF_PUBLIC|MEMF_CLEAR));
  if (priv == NULL) {
    playsidcpp_free(s);
	return NULL;
  }

  priv->info = static_cast<struct PlaysidFpInfo *> (AllocVec(sizeof(struct PlaysidFpInfo), MEMF_PUBLIC|MEMF_CLEAR));
  if (priv->info == NULL) {
    playsidcpp_free(s);
	return NULL;
  }

  /* fill private */
  try {
    priv->sp = new sidplayfp ();
    if (priv->sp == NULL) {
      playsidcpp_free(s);
	  return NULL;
    }
  } catch (...) {
      playsidcpp_free(s);
	  return NULL;
  }
  try {
    priv->tune = new SidTune (0);
    if (priv->tune == NULL) {
      playsidcpp_free(s);
      return NULL;
    }
  } catch (...) {
      playsidcpp_free(s);
      return NULL;
  }

  priv->emulation = sfn->Emulation;
  priv->sid_model = sid_model_to_sidplay_config (sfn->SidModel);
  priv->sid_model_force = sfn->SidModelForce?true:false;
  priv->c64_model = c64_model_to_sidplay_config (sfn->MachineType);
  priv->c64_model_force = sfn->MachineTypeForce?true:false;
  priv->cia_model = cia_model_to_sidplay_config (sfn->CiaModel);
  priv->sampling_method = sampling_method_to_sidplay_config (sfn->SamplingMethod);
  priv->filter = sfn->Filter?true:false;
  priv->digiboost = sfn->Digiboost?true:false;
  priv->playback = playback_to_sidplay_config (sfn->Playback);
  priv->resid_bias = sfn->ResidBias;
  priv->residfp_filter_curve_6581 = sfn->ResidFpFilterCurve6581;
  priv->residfp_filter_curve_8580 = sfn->ResidFpFilterCurve8580;
  priv->audio_frequency = sfn->AudioFrequency;
  priv->current_subtune = 0;

  s->PrivateData = (APTR)priv;
  s->Error = PFE_OK;

  return s;
}

struct PlaysidFp *playsidcpp_create_taglist (struct TagItem *Item)
{
    struct PlaysidFpNew sfn;

    sfn.Emulation = (BYTE)GetTagData(PFA_Emulation, PF_EMULATION_RESIDFP, Item);
    sfn.SidModel = (BYTE)GetTagData(PFA_SidModel, PF_SID_MODEL_MOS6581, Item);
    sfn.SidModelForce = (BOOL)GetTagData(PFA_SidModelForce, FALSE, Item);
    sfn.MachineType = (BYTE)GetTagData(PFA_MachineType, PF_MACHINE_TYPE_PAL, Item);
    sfn.MachineTypeForce = (BOOL)GetTagData(PFA_MachineTypeForce, FALSE, Item);
    sfn.CiaModel = (BYTE)GetTagData(PFA_CiaModel, PF_CIA_MODEL_MOS6526, Item);
    sfn.SamplingMethod = (BYTE)GetTagData(PFA_SamplingMethod, PF_SAMPLING_METHOD_INTERPOLATE, Item);
    sfn.Filter = (BOOL)GetTagData(PFA_Filter, FALSE, Item);
    sfn.Digiboost = (BOOL)GetTagData(PFA_Digiboost, FALSE, Item);
    sfn.Playback = (BYTE)GetTagData(PFA_Playback, PF_PLAYBACK_MONO, Item);
    sfn.ResidBias = (FLOAT)GetTagData(PFA_ResidBias, 0.5f, Item);
    sfn.ResidFpFilterCurve6581 = (FLOAT)GetTagData(PFA_ResidFpFilterCurve6581, 0.5f, Item);
    sfn.ResidFpFilterCurve8580 = (FLOAT)GetTagData(PFA_ResidFpFilterCurve8580, 0.5f, Item);
    sfn.AudioFrequency = GetTagData(PFA_AudioFrequency, 44100, Item);

    return create(&sfn);
}

struct PlaysidFp *playsidcpp_create_tags ( Tag tag1, ...)
{
    return playsidcpp_create_taglist ((struct TagItem *)&tag1);
}

void playsidcpp_free (struct PlaysidFp *s)
{
  playsidcpp_priv_t *priv = NULL;
  if (s) {
    priv = static_cast<playsidcpp_priv_t *>(s->PrivateData);
    if (priv) {
      if (priv->info) {
        FreeVec (priv->info);
		priv->info = NULL;
      }
      if (priv->tune) {
        delete priv->tune;
        priv->tune = NULL;
      }
      delete priv->config.sidEmulation;
      priv->config.sidEmulation = NULL;
      if (priv->sp) {
        delete priv->sp;
        priv->sp = NULL;
      }

      FreeVec (priv);
      s->PrivateData = 0;
    }
    FreeVec (s);
  }
}

BOOL playsidcpp_set_roms(struct PlaysidFp *s, CONST UBYTE *kernal, CONST UBYTE *basic, CONST UBYTE *chargen)
{
  if (s == NULL) {
    return FALSE;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  if (priv->sp == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  if (kernal == NULL || basic == NULL) {
    s->Error = PFE_PARAMETERS;
    return FALSE;
  }
  try {
    priv->sp->setRoms(kernal, basic, chargen);
  } catch(...) {
    s->Error = PFE_SET_ROMS;
    return FALSE;
  }
  try {
    bool rc = priv->sp->load (priv->tune);
    if (!rc) {
        s->Error = PFE_TUNE_LOAD;
        return FALSE;
    }
  } catch (...) {
    s->Error = PFE_TUNE_LOAD;
    return FALSE;
  }
  return TRUE;
}


BOOL playsidcpp_init (struct PlaysidFp *s, CONST UBYTE *data, ULONG data_len)
{
  if (s == NULL) {
    return FALSE;
  }

  s->Initialized = FALSE;

  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  if (priv->tune == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  priv->tune->read (data, data_len);
  priv->config = priv->sp->config ();
  priv->tune->selectSong (priv->current_subtune);

  bool rc = priv->sp->load (priv->tune);
  if (!rc) {
    s->Error = PFE_TUNE_LOAD;
    return FALSE;
  }

  sidbuilder *sb = NULL;
  if (priv->emulation == PF_EMULATION_RESID) {
    ReSIDBuilder *rsb = NULL;
    try {
      ReSIDBuilder *rsb = new ReSIDBuilder ("ReSID");
      if (rsb == NULL) {
        s->Error = PFE_EMULATION_RESID;
        return FALSE;
      }
      const SidInfo &info = priv->sp->info();
      rsb->create (info.maxsids());
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESID;
        return FALSE;
      }
      rsb->filter (priv->filter);
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESID;
        return FALSE;
      }
      rsb->bias (priv->resid_bias);
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESID;
        return FALSE;
      }
      sb = rsb;
    } catch (...) {
      delete rsb;
      s->Error = PFE_EMULATION_RESID;
      return FALSE;
    }
  } else { /* EMULATION_RESIDFP */
    ReSIDfpBuilder *rsb = NULL;
    try {
      ReSIDfpBuilder *rsb = new ReSIDfpBuilder ("ReSIDfp");
      if (rsb == NULL) {
        s->Error = PFE_EMULATION_RESIDFP;
        return FALSE;
      }
      const SidInfo &info = priv->sp->info();
      rsb->create (info.maxsids());
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESIDFP;
        return FALSE;
      }
      rsb->filter (priv->filter);
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESIDFP;
        return FALSE;
      }
      rsb->filter6581Curve (priv->residfp_filter_curve_6581);
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESIDFP;
        return FALSE;
      }
      rsb->filter8580Curve (priv->residfp_filter_curve_8580);
      if (!rsb->getStatus()) {
        delete rsb;
        s->Error = PFE_EMULATION_RESIDFP;
        return FALSE;
      }
      sb = rsb;
    } catch (...) {
      s->Error = PFE_EMULATION_RESIDFP;
      return FALSE;
    }
  }

  SidConfig &conf = priv->config;
  conf.frequency = priv->audio_frequency;
  conf.playback = priv->playback;
  conf.defaultSidModel = priv->sid_model;
  conf.forceSidModel = priv->sid_model_force;
  conf.defaultC64Model = priv->c64_model;
  conf.forceC64Model = priv->c64_model_force;
  conf.ciaModel = priv->cia_model;
  conf.samplingMethod = priv->sampling_method;

  try {
    sb->filter(priv->filter); // is this really needed
  } catch (...) {
    delete sb;
    s->Error = PFE_FILTER;
  }
  delete conf.sidEmulation;
  conf.sidEmulation = sb;

  try {
    rc = priv->sp->config (conf);
    if (rc == false) {
      s->Error = PFE_CONFIG;
      return FALSE;
    }
  } catch (...) {
    delete conf.sidEmulation;
    s->Error = PFE_CONFIG;
    return FALSE;
  }

  s->Initialized = TRUE;

  return TRUE;
}

LONG playsidcpp_play (struct PlaysidFp *s, SHORT *buffer, LONG buffer_len)
{
  if (s == NULL) {
    return -1;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return -1;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return -1;
  }
  if (priv->sp == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return -1;
  }

  try {
    return static_cast<LONG>(priv->sp->play (buffer, buffer_len));
  } catch (...) {
    s->Error = PFE_PLAY;
    return -1;
  }
}

BOOL playsidcpp_speed (struct PlaysidFp *s, USHORT percent)
{
  if (s == NULL) {
    return FALSE;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return FALSE;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  if (priv->sp == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }

  try {
    bool retval = priv->sp->fastForward (static_cast<unsigned int>(percent));
    return retval?TRUE:FALSE;
  } catch (...) {
    s->Error = PFE_SPEED;
  }
  return FALSE;
}

BOOL playsidcpp_mute (struct PlaysidFp *s, UBYTE sid_number, UBYTE voice_channel, BOOL mute)
{
  if (s == NULL) {
    return FALSE;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return FALSE;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  if (priv->sp == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }

  try {
    priv->sp->mute (static_cast<unsigned int>(sid_number), static_cast<unsigned int>(voice_channel), mute?true:false);
    return TRUE;
  } catch (...) {
    s->Error = PFE_MUTE;
    return FALSE;
  }
}

BOOL playsidcpp_filter (struct PlaysidFp *s, BOOL filter)
{
  if (s == NULL) {
    return FALSE;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return FALSE;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  if (priv->config.sidEmulation == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }

  try {
    priv->config.sidEmulation->filter((filter==TRUE)?true:false);
  } catch (...) {
    s->Error = PFE_FILTER;
    return FALSE;
  }
  return TRUE;
}

LONG playsidcpp_time (struct PlaysidFp *s)
{
  if (s == NULL) {
    return -1;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return -1;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return -1;
  }
  if (priv->sp == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return -1;
  }
  try {
    return static_cast<ULONG>(priv->sp->timeMs());
  } catch (...) {
    s->Error = PFE_TIME;
    return -1;
  }
}

CONST struct PlaysidFpInfo *playsidcpp_tune_info (struct PlaysidFp *s)
{
  if (s == NULL) {
    return NULL;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return NULL;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return NULL;
  }
  if (priv->info == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return NULL;
  }
  if (priv->tune == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return NULL;
  }

  try {
    const SidTuneInfo *tuneInfo = priv->tune->getInfo();
    if (tuneInfo == NULL) {
      s->Error = PFE_TUNE_INFO;
      return NULL;
    }

    struct PlaysidFpInfo *i = priv->info;
    unsigned int current = tuneInfo->currentSong();
    i->Title = (STRPTR)tuneInfo->infoString(0);
    i->Author = (STRPTR)tuneInfo->infoString(1);
    i->Released = (STRPTR)tuneInfo->infoString(2);

    i->SidsRequired = tuneInfo->sidChips();
    i->SidModel = tune_info_sid_model_convert(tuneInfo->sidModel(current));
    i->ClockSpeed = tune_info_clock_speed_convert(tuneInfo->clockSpeed());
    i->Compability = tune_info_compability_convert(tuneInfo->compatibility());

    return static_cast<CONST struct PlaysidFpInfo *>(i);
  } catch (...) {
    s->Error = PFE_TUNE_INFO;
    return NULL;
  }
}

UWORD playsidcpp_current_subtune (struct PlaysidFp *s)
{
  if (s == NULL) {
    return 0;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return 0;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return 0;
  }
  if (priv->info == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return 0;
  }

  try {
    const SidTuneInfo *tuneInfo = priv->tune->getInfo();
    if (tuneInfo) return static_cast<UWORD>(tuneInfo->currentSong());
    s->Error = PFE_SUBTUNE;
  } catch (...) {
    s->Error = PFE_SUBTUNE;
  }
  return 0;
}

UWORD playsidcpp_subtunes (struct PlaysidFp *s)
{
  if (s == NULL) {
    return 0;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return 0;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return 0;
  }
  if (priv->info == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return 0;
  }

  try {
    const SidTuneInfo *tuneInfo = priv->tune->getInfo();
    if (tuneInfo) return static_cast<UWORD>(tuneInfo->songs());
    s->Error = PFE_SUBTUNES;
  } catch (...) {
    s->Error = PFE_SUBTUNES;
  }
  return 0;
}

BOOL playsidcpp_subtune_set (struct PlaysidFp *s, UWORD subtune)
{
  UWORD subtunes = 0;
  UWORD csubtune = 0;
  if (s == NULL) {
    return FALSE;
  }
  if (s->Initialized != TRUE) {
    return FALSE;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return FALSE;
  }
  subtunes = playsidcpp_subtunes (s);
  if (subtunes == 0 || subtunes < subtune || subtune < 0) {
    s->Error = PFE_PARAMETERS;
    return FALSE;
  }

  try {
    csubtune = priv->tune->selectSong (subtune);
    if (0 == csubtune) {
      s->Error = PFE_SUBTUNE_SET;
      return FALSE;
    }
  } catch (...) {
    s->Error = PFE_SUBTUNE_SET;
    return FALSE;
  }
  try {
    bool rc = priv->sp->load (priv->tune);
    if (!rc) {
        s->Error = PFE_TUNE_LOAD;
        return FALSE;
    }
  } catch (...) {
    s->Error = PFE_TUNE_LOAD;
    return FALSE;
  }
  priv->current_subtune = csubtune;
  return TRUE;
}

CONST_STRPTR playsidcpp_tune_md5 (struct PlaysidFp *s)
{
  if (s == NULL) {
    return NULL;
  }
  if (s->Initialized != TRUE) {
    s->Error = PFE_NOT_INITIALIZED;
    return NULL;
  }
  playsidcpp_priv_t *priv = static_cast<playsidcpp_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return NULL;
  }
  if (priv->tune == NULL) {
    s->Error = PFE_NOT_ALLOCATED;
    return NULL;
  }

  try {
    return (CONST_STRPTR)priv->tune->createMD5New(0); // creates internally new
  } catch (...) {
    s->Error = PFE_TUNE_MD5;
  }
  return NULL;
}

/* static */
SidConfig::sid_model_t sid_model_to_sidplay_config(BYTE sid_model)
{
  if (sid_model == PF_SID_MODEL_MOS8580) {
    return SidConfig::MOS8580;
  }
  return SidConfig::MOS6581;
}

SidConfig::c64_model_t c64_model_to_sidplay_config(BYTE c64_model)
{
  if (c64_model == PF_MACHINE_TYPE_NTSC) {
    return SidConfig::NTSC;
  } else if (c64_model == PF_MACHINE_TYPE_PAL_M) {
    return SidConfig::PAL_M;
  } else if (c64_model == PF_MACHINE_TYPE_OLD_NTSC) {
    return SidConfig::OLD_NTSC;
  } else if (c64_model == PF_MACHINE_TYPE_DREAN) {
    return SidConfig::DREAN;
  }
  return SidConfig::PAL;
}

SidConfig::cia_model_t cia_model_to_sidplay_config(BYTE cia_model)
{
  if (cia_model == PF_CIA_MODEL_MOS8521) {
    return SidConfig::MOS8521;
  }
  return SidConfig::MOS6526;
}

SidConfig::sampling_method_t sampling_method_to_sidplay_config(BYTE sampling_method)
{
  if (sampling_method == PF_SAMPLING_METHOD_RESAMPLE_INTERPOLATE) {
    return SidConfig::RESAMPLE_INTERPOLATE;
  }
  return SidConfig::INTERPOLATE;
}

SidConfig::playback_t playback_to_sidplay_config (BYTE playback)
{
  if (playback == PF_PLAYBACK_MONO) {
    return SidConfig::MONO;
  }
  return SidConfig::STEREO;
}

// for info
static BYTE tune_info_sid_model_convert(SidTuneInfo::model_t s)
{
    if (s == SidTuneInfo::SIDMODEL_6581) {
        return PFI_SID_MODEL_MOS6581;
    } else if (s == SidTuneInfo::SIDMODEL_8580) {
        return PFI_SID_MODEL_MOS8580;
    } if (s == SidTuneInfo::SIDMODEL_ANY) {
        return PFI_SID_MODEL_ANY;
    }
    return PFI_SID_MODEL_ANY;
}

static BYTE tune_info_clock_speed_convert(SidTuneInfo::clock_t c)
{
  if (c == SidTuneInfo::CLOCK_PAL) {
    return PFI_CLOCK_PAL;
  } else if (c == SidTuneInfo::CLOCK_NTSC) {
    return PFI_CLOCK_NTSC;
  } else if (c == SidTuneInfo::CLOCK_ANY) {
    return PFI_CLOCK_ANY;
  }
  return PFI_CLOCK_UNKNOWN;
}

static BYTE tune_info_compability_convert(SidTuneInfo::compatibility_t c)
{
  if (c == SidTuneInfo::COMPATIBILITY_C64) {
    return PFI_COMPATIBILITY_C64;
  } else if (c == SidTuneInfo::COMPATIBILITY_PSID) {
    return PFI_COMPATIBILITY_PSID;
  } else if (c == SidTuneInfo::COMPATIBILITY_R64) {
    return PFI_COMPATIBILITY_R64;
  } else if (c == SidTuneInfo::COMPATIBILITY_BASIC) {
    return PFI_COMPATIBILITY_BASIC;
  }
  return -1;
}

} /* extern "C" */
