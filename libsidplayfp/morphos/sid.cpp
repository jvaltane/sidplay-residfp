#include <sidplayfp/sidplayfp.h>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/builders/residfp.h>
#include <sidplayfp/builders/resid.h>
#include <sidplayfp/SidDatabase.h>

extern "C" {
#include <proto/exec.h>
}

#include <string>
#include <stdlib.h>

#include "sid.h"

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
  struct SidplayFpInfo *info;
  int32_t current_subtune;
} sid_priv_t;

extern "C" {

static bool is_sidbuilder_valid(sidbuilder *b);
static SidConfig::sid_model_t sid_model_to_sid_config (BYTE sid_model);
static SidConfig::c64_model_t c64_model_to_sid_config (BYTE c64_model);
static SidConfig::cia_model_t cia_model_to_sid_config (BYTE cia_model);
static SidConfig::sampling_method_t sampling_method_to_sid_config (BYTE sampling_method);
static SidConfig::playback_t playback_to_sid_config (BYTE playback);

static BYTE tune_info_sid_model_convert(SidTuneInfo::model_t s);
static BYTE tune_info_clock_speed_convert(SidTuneInfo::clock_t c);
static BYTE tune_info_compability_convert(SidTuneInfo::compatibility_t c);

struct SidplayFp *sid_create (struct SidplayFpNew *sfn)
{
  struct SidplayFp *s = static_cast<struct SidplayFp *> (AllocVec(sizeof(struct SidplayFp), MEMF_PUBLIC|MEMF_CLEAR));
  if (s == NULL) {
    return NULL;
  }

  s->Loaded = FALSE;

  sid_priv_t *priv = static_cast<sid_priv_t *> (AllocVec(sizeof(sid_priv_t), MEMF_PUBLIC|MEMF_CLEAR));
  if (priv == NULL) {
    sid_free(s);
	return NULL;
  }

  priv->info = static_cast<struct SidplayFpInfo *> (AllocVec(sizeof(struct SidplayFpInfo), MEMF_PUBLIC|MEMF_CLEAR));
  if (priv->info == NULL) {
    sid_free(s);
	return NULL;
  }

  /* fill private */
  priv->sp = new sidplayfp ();
  if (priv->sp == NULL) {
    sid_free(s);
	return NULL;
  }

  priv->emulation = sfn->Emulation;
  priv->sid_model = sid_model_to_sid_config (sfn->SidModel);
  priv->sid_model_force = sfn->SidModelForce?true:false;
  priv->c64_model = c64_model_to_sid_config (sfn->MachineType);
  priv->c64_model_force = sfn->MachineTypeForce?true:false;
  priv->cia_model = cia_model_to_sid_config (sfn->CiaModel);
  priv->sampling_method = sampling_method_to_sid_config (sfn->SamplingMethod);
  priv->filter = sfn->Filter?true:false;
  priv->digiboost = sfn->Digiboost?true:false;
  priv->playback = playback_to_sid_config (sfn->Playback);
  priv->resid_bias = sfn->ResidBias;
  priv->residfp_filter_curve_6581 = sfn->ResidFpFilterCurve6581;
  priv->residfp_filter_curve_8580 = sfn->ResidFpFilterCurve8580;
  priv->audio_frequency = sfn->AudioFrequency;
  priv->current_subtune = 0;

  s->PrivateData = (APTR)priv;

  return s;
}

struct SidplayFp *sid_create_taglist (struct TagItem *Item)
{
    return 0;
}

struct SidplayFp *sid_create_tags ( Tag, ...)
{
    return 0;
}

void sid_free (struct SidplayFp *s)
{
  sid_priv_t *priv = NULL;
  if (s) {
    priv = static_cast<sid_priv_t *>(s->PrivateData);
    if (priv) {
      if (priv->info) {
        FreeVec (priv->info);
		priv->info = NULL;
      }
      if (priv->tune) {
        delete priv->tune;
        priv->tune = NULL;
      }
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

BOOL sid_set_roms(struct SidplayFp *s, CONST UBYTE *kernal, CONST UBYTE *basic, CONST UBYTE *chargen)
{
  if (s == NULL) {
    return FALSE;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    return FALSE;
  }
  if (priv->sp == NULL) {
    return FALSE;
  }
  if (kernal == NULL || basic == NULL) {
    return FALSE;
  }
  priv->sp->setRoms(kernal, basic, chargen);
  return TRUE;
}


BOOL sid_init (struct SidplayFp *s, CONST UBYTE *data, ULONG data_len)
{
  if (s == NULL) {
    return FALSE;
  }

  s->Loaded = FALSE;

  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv->tune) {
    delete priv->tune;
    priv->tune = NULL;
  }
  priv->tune = new SidTune (0);
  if (priv->tune == NULL) {
    return FALSE;
  }
  priv->tune->read (data, data_len);
  priv->tune->selectSong (priv->current_subtune);

  bool rc = priv->sp->load (priv->tune);
  if (!rc) {
    return FALSE;
  }
  priv->config = priv->sp->config ();

  sidbuilder *sb = NULL;
  if (priv->emulation == SFN_EMULATION_RESID) {
    ReSIDBuilder *rsb = new ReSIDBuilder ("ReSID");
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    const SidInfo &info = priv->sp->info();
    rsb->create (info.maxsids());
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    rsb->filter (priv->filter);
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    rsb->bias (priv->resid_bias);
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    sb = rsb;
  } else { /* EMULATION_RESIDFP */
    ReSIDfpBuilder *rsb = new ReSIDfpBuilder ("ReSIDfp");
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    const SidInfo &info = priv->sp->info();
    rsb->create (info.maxsids());
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    rsb->filter (priv->filter);
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    rsb->filter6581Curve (priv->residfp_filter_curve_6581);
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    rsb->filter8580Curve (priv->residfp_filter_curve_8580);
    if (is_sidbuilder_valid(rsb) == false) {
      return FALSE;
    }
    sb = rsb;
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

  sb->filter(priv->filter); // is this really needed
  conf.sidEmulation = sb;

  rc = priv->sp->config (conf);
  if (rc == false) {
    return FALSE;
  }

  s->Loaded = TRUE;

  return TRUE;
}

LONG sid_play (struct SidplayFp *s, SHORT *buffer, LONG buffer_len)
{
  if (s == NULL) {
    return -1;
  }
  if (s->Loaded != TRUE) {
    return -2;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  return priv->sp->play (buffer, buffer_len);
}

CONST struct SidplayFpInfo *sid_tune_info (struct SidplayFp *s)
{
  if (s == NULL) {
    return NULL;
  }
  if (s->Loaded != TRUE) {
    return NULL;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    return NULL;
  }
  if (priv->info == NULL) {
    return NULL;
  }
  if (priv->tune == NULL) {
    return NULL;
  }
  const SidTuneInfo *tuneInfo = priv->tune->getInfo();
  if (tuneInfo == NULL) {
    return NULL;
  }
  struct SidplayFpInfo *i = priv->info;
  unsigned int current = tuneInfo->currentSong();
  i->Title = (STRPTR)tuneInfo->infoString(0);
  i->Author = (STRPTR)tuneInfo->infoString(1);
  i->Released = (STRPTR)tuneInfo->infoString(2);

  i->SidsRequired = tuneInfo->sidChips();
  i->SidModel = tune_info_sid_model_convert(tuneInfo->sidModel(current));
  i->ClockSpeed = tune_info_clock_speed_convert(tuneInfo->clockSpeed());
  i->Compability = tune_info_compability_convert(tuneInfo->compatibility());

  return static_cast<CONST struct SidplayFpInfo *>(i);
}

UWORD sid_current_subtune (struct SidplayFp *s)
{
  if (s == NULL) {
    return 0;
  }
  if (s->Loaded != TRUE) {
    return 0;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    return 0;
  }
  if (priv->info == NULL) {
    return 0;
  }

  const SidTuneInfo *tuneInfo = priv->tune->getInfo();
  if (tuneInfo) return static_cast<UWORD>(tuneInfo->currentSong());

  return 0;
}

UWORD sid_subtunes (struct SidplayFp *s)
{
  if (s == NULL) {
    return 0;
  }
  if (s->Loaded != TRUE) {
    return 0;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    return 0;
  }
  if (priv->info == NULL) {
    return 0;
  }
  const SidTuneInfo *tuneInfo = priv->tune->getInfo();
  if (tuneInfo) return static_cast<UWORD>(tuneInfo->songs());
  return 0;
}

BOOL sid_subtune_set (struct SidplayFp *s, UWORD subtune)
{
  UWORD subtunes = 0;
  if (s == NULL) {
    return FALSE;
  }
  if (s->Loaded != TRUE) {
    return FALSE;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    return FALSE;
  }
  subtunes = sid_subtunes (s);
  if (subtunes == 0 || subtunes < subtune || subtune < 0) {
    return FALSE;
  }
  priv->current_subtune = subtune;
  return 0;
}

CONST_STRPTR sid_tune_md5 (struct SidplayFp *s)
{
  if (s == NULL) {
    return NULL;
  }
  if (s->Loaded != TRUE) {
    return NULL;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->PrivateData);
  if (priv == NULL) {
    return 0;
  }
  if (priv->tune == NULL) {
    return NULL;
  }
  return (CONST_STRPTR)priv->tune->createMD5New(0); // creates interrnally new
}

/* static */
bool is_sidbuilder_valid(sidbuilder *b)
{
  if (b == NULL) {
    return false;
  }
  if (!b->getStatus()) {
    return false;
  }
  return true;
}

SidConfig::sid_model_t sid_model_to_sid_config(BYTE sid_model)
{
  if (sid_model == SFN_SID_MODEL_MOS8580) {
    return SidConfig::MOS8580;
  }
  return SidConfig::MOS6581;
}

SidConfig::c64_model_t c64_model_to_sid_config(BYTE c64_model)
{
  if (c64_model == SFN_MACHINE_TYPE_NTSC) {
    return SidConfig::NTSC;
  } else if (c64_model == SFN_MACHINE_TYPE_PAL_M) {
    return SidConfig::PAL_M;
  } else if (c64_model == SFN_MACHINE_TYPE_OLD_NTSC) {
    return SidConfig::OLD_NTSC;
  } else if (c64_model == SFN_MACHINE_TYPE_DREAN) {
    return SidConfig::DREAN;
  }
  return SidConfig::PAL;
}

SidConfig::cia_model_t cia_model_to_sid_config(BYTE cia_model)
{
  if (cia_model == SFN_CIA_MODEL_MOS8521) {
    return SidConfig::MOS8521;
  }
  return SidConfig::MOS6526;
}

SidConfig::sampling_method_t sampling_method_to_sid_config(BYTE sampling_method)
{
  if (sampling_method == SFN_SAMPLING_METHOD_RESAMPLE_INTERPOLATE) {
    return SidConfig::RESAMPLE_INTERPOLATE;
  }
  return SidConfig::INTERPOLATE;
}

SidConfig::playback_t playback_to_sid_config (BYTE playback)
{
  if (playback == SFN_PLAYBACK_MONO) {
    return SidConfig::MONO;
  }
  return SidConfig::STEREO;
}

// for info
static BYTE tune_info_sid_model_convert(SidTuneInfo::model_t s)
{
    if (s == SidTuneInfo::SIDMODEL_6581) {
        return SFI_SID_MODEL_MOS6581;
    } else if (s == SidTuneInfo::SIDMODEL_8580) {
        return SFI_SID_MODEL_MOS8580;
    } if (s == SidTuneInfo::SIDMODEL_ANY) {
        return SFI_SID_MODEL_ANY;
    }
    return SFI_SID_MODEL_ANY;
}

static BYTE tune_info_clock_speed_convert(SidTuneInfo::clock_t c)
{
  if (c == SidTuneInfo::CLOCK_PAL) {
    return SFI_CLOCK_PAL;
  } else if (c == SidTuneInfo::CLOCK_NTSC) {
    return SFI_CLOCK_NTSC;
  } else if (c == SidTuneInfo::CLOCK_ANY) {
    return SFI_CLOCK_ANY;
  }
  return SFI_CLOCK_UNKNOWN;
}

static BYTE tune_info_compability_convert(SidTuneInfo::compatibility_t c)
{
  if (c == SidTuneInfo::COMPATIBILITY_C64) {
    return SFI_COMPATIBILITY_C64;
  } else if (c == SidTuneInfo::COMPATIBILITY_PSID) {
    return SFI_COMPATIBILITY_PSID;
  } else if (c == SidTuneInfo::COMPATIBILITY_R64) {
    return SFI_COMPATIBILITY_R64;
  } else if (c == SidTuneInfo::COMPATIBILITY_BASIC) {
    return SFI_COMPATIBILITY_BASIC;
  }
  return -1;
}

} /* extern "C" */
