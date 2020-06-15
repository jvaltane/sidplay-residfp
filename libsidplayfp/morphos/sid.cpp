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
} sid_priv_t;

extern "C" {

static bool is_sidbuilder_valid(sidbuilder *b);
static SidConfig::sid_model_t sid_model_to_sid_config (BYTE sid_model);
static SidConfig::c64_model_t c64_model_to_sid_config (BYTE c64_model);
static SidConfig::cia_model_t cia_model_to_sid_config (BYTE cia_model);
static SidConfig::sampling_method_t sampling_method_to_sid_config (BYTE sampling_method);
static SidConfig::playback_t playback_to_sid_config (BYTE playback);

struct SidplayFp *sid_create (CONST struct SidplayFpNew *sfn)
{
  struct SidplayFp *s = static_cast<struct SidplayFp *> (AllocVec(sizeof(struct SidplayFp), MEMF_PUBLIC|MEMF_CLEAR));
  if (s == NULL) {
    return NULL;
  }

  s->sf_Loaded = FALSE;

  sid_priv_t *priv = static_cast<sid_priv_t *> (AllocVec(sizeof(sid_priv_t), MEMF_PUBLIC|MEMF_CLEAR));
  if (priv == NULL) {
    sid_free(s);
	return NULL;
  }

  /* fill private */
  priv->sp = new sidplayfp ();
  if (priv->sp == NULL) {
    sid_free(s);
	return NULL;
  }

  priv->emulation = sfn->sfn_Emulation;
  priv->sid_model = sid_model_to_sid_config (sfn->sfn_SidModel);
  priv->sid_model_force = sfn->sfn_SidModelForce?true:false;
  priv->c64_model = c64_model_to_sid_config (sfn->sfn_MachineType);
  priv->c64_model_force = sfn->sfn_MachineTypeForce?true:false;
  priv->cia_model = cia_model_to_sid_config (sfn->sfn_CiaModel);
  priv->sampling_method = sampling_method_to_sid_config (sfn->sfn_SamplingMethod);
  priv->filter = sfn->sfn_Filter?true:false;
  priv->digiboost = sfn->sfn_Digiboost?true:false;
  priv->playback = playback_to_sid_config (sfn->sfn_Playback);
  priv->resid_bias = sfn->sfn_ResidBias;
  priv->residfp_filter_curve_6581 = sfn->sfn_ResidFpFilterCurve6581;
  priv->residfp_filter_curve_8580 = sfn->sfn_ResidFpFilterCurve8580;
  priv->audio_frequency = sfn->sfn_AudioFrequency;
  priv->database = sfn->sfn_Database;

  s->sf_PrivateData = (APTR)priv;

  return s;
}

struct SidplayFp *sid_create_taglist (CONST struct SidplayFpNew *sfn, CONST struct TagItem *Item)
{
    return 0;
}

struct SidplayFp *sid_create_tags (ULONG, ...)
{
    return 0;
}

void sid_free (struct SidplayFp *s)
{
  sid_priv_t *priv = NULL;
  if (s) {
    priv = static_cast<sid_priv_t *>(s->sf_PrivateData);
    if (priv) {
      if (priv->tune) {
        delete priv->tune;
        priv->tune = NULL;
      }
      if (priv->sp) {
        delete priv->sp;
        priv->sp = NULL;
      }

      FreeVec (priv);
      s->sf_PrivateData = 0;
    }
    FreeVec (s);
  }
}

BOOL sid_load (struct SidplayFp *s, CONST UBYTE *data, CONST LONG data_len)
{
  if (s == NULL) {
    return FALSE;
  }

  sid_priv_t *priv = static_cast<sid_priv_t *> (s->sf_PrivateData);
  if (priv->tune) {
    delete priv->tune;
    priv->tune = NULL;
  }
  priv->tune = new SidTune (0);
  if (priv->tune == NULL) {
    return FALSE;
  }
  priv->tune->read (data, data_len);
  priv->tune->selectSong (0);

  bool rc = priv->sp->load (priv->tune);
  if (!rc) {
    return FALSE;
  }
  priv->config = priv->sp->config ();

  sidbuilder *sb = NULL;
  if (priv->emulation == SF_EMULATION_RESID) {
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
  s->sf_Loaded = TRUE;
  return TRUE;
}

LONG sid_play (struct SidplayFp *s, SHORT *buffer, const LONG buffer_len)
{
  if (s == NULL) {
    return -1;
  }
  if (s->sf_Loaded != TRUE) {
    return -2;
  }
  sid_priv_t *priv = static_cast<sid_priv_t *> (s->sf_PrivateData);
  return priv->sp->play (buffer, buffer_len);
}

BOOL sid_tune_info (struct SidplayFp *s)
{
  if (s == NULL) {
    return FALSE;
  }
  if (s->sf_Loaded != TRUE) {
    return FALSE;
  }
  return 0;
}

UWORD sid_subtunes (struct SidplayFp *s)
{
  if (s == NULL) {
    return 0;
  }
  if (s->sf_Loaded != TRUE) {
    return 0;
  }
  return 0;
}

BOOL sid_subtune_set (struct SidplayFp *s, UWORD subtune)
{
  if (s == NULL) {
    return FALSE;
  }
  if (s->sf_Loaded != TRUE) {
    return FALSE;
  }
  return 0;
}

LONG sid_subtune_length (struct SidplayFp *s, UWORD subtune)
{
  if (s == NULL) {
    return -1;
  }
  if (s->sf_Loaded != TRUE) {
    return -1;
  }
  return 0;
}

LONG sid_subtune_length_current (struct SidplayFp *s)
{
  if (s == NULL) {
    return -1;
  }
  if (s->sf_Loaded != TRUE) {
    return -1;
  }
  return 0;
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
  if (sid_model == SF_SID_MODEL_MOS8580) {
    return SidConfig::MOS8580;
  }
  return SidConfig::MOS6581;
}

SidConfig::c64_model_t c64_model_to_sid_config(BYTE c64_model)
{
  if (c64_model == SF_MACHINE_TYPE_NTSC) {
    return SidConfig::NTSC;
  } else if (c64_model == SF_MACHINE_TYPE_PAL_M) {
    return SidConfig::PAL_M;
  } else if (c64_model == SF_MACHINE_TYPE_OLD_NTSC) {
    return SidConfig::OLD_NTSC;
  } else if (c64_model == SF_MACHINE_TYPE_DREAN) {
    return SidConfig::DREAN;
  }
  return SidConfig::PAL;
}

SidConfig::cia_model_t cia_model_to_sid_config(BYTE cia_model)
{
  if (cia_model == SF_CIA_MODEL_MOS8521) {
    return SidConfig::MOS8521;
  }
  return SidConfig::MOS6526;
}

SidConfig::sampling_method_t sampling_method_to_sid_config(BYTE sampling_method)
{
  if (sampling_method == SF_SAMPLING_METHOD_RESAMPLE_INTERPOLATE) {
    return SidConfig::RESAMPLE_INTERPOLATE;
  }
  return SidConfig::INTERPOLATE;
}

SidConfig::playback_t playback_to_sid_config (BYTE playback)
{
  if (playback == SF_PLAYBACK_MONO) {
    return SidConfig::MONO;
  }
  return SidConfig::STEREO;
}

/* exterm c */
}
