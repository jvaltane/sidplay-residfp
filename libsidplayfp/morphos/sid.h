#ifndef SID_H
#define SID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <clib/sidplayfp_protos.h>

struct SidplayFp *sid_create_taglist (struct TagItem *item);
struct SidplayFp *sid_create_tags (Tag, ...);
void sid_free (struct SidplayFp *s);

BOOL sid_set_roms(struct SidplayFp *s, CONST UBYTE *kernal, CONST UBYTE *basic, CONST UBYTE *chargen);
BOOL sid_init (struct SidplayFp *s, CONST UBYTE *data, ULONG data_len);
LONG sid_play (struct SidplayFp *s, SHORT *buffer, LONG buffer_len);
BOOL sid_speed (struct SidplayFp *s, USHORT percent);
BOOL sid_mute (struct SidplayFp *s, UBYTE sid_number, UBYTE voice_channel, BOOL mute);
LONG sid_time (struct SidplayFp *s);

CONST struct SidplayFpInfo *sid_tune_info (struct SidplayFp *s);

UWORD sid_current_subtune (struct SidplayFp *s);
UWORD sid_subtunes (struct SidplayFp *s);
BOOL sid_subtune_set (struct SidplayFp *s, UWORD subtune);
CONST_STRPTR sid_tune_md5 (struct SidplayFp *s);

#ifdef __cplusplus
}
#endif

#endif /* SID_H */
