#ifndef SID_H
#define SID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <clib/sidplayfp_protos.h>

struct SidplayFp *sid_create (struct SidplayFpNew *sfn);
struct SidplayFp *sid_create_taglist (struct TagItem *item);
struct SidplayFp *sid_create_tags (Tag, ...);
void sid_free (struct SidplayFp *s);

BOOL sid_init (struct SidplayFp *s, CONST UBYTE *data, ULONG data_len);
LONG sid_play (struct SidplayFp *s, SHORT *buffer, LONG buffer_len);

CONST struct SidplayFpInfo *sid_tune_info (struct SidplayFp *s);

UWORD sid_current_subtune (struct SidplayFp *s);
UWORD sid_subtunes (struct SidplayFp *s);
BOOL sid_subtune_set (struct SidplayFp *s, UWORD subtune);
LONG sid_subtune_length (struct SidplayFp *s, UWORD subtune);
LONG sid_subtune_length_current (struct SidplayFp *s);

#ifdef __cplusplus
}
#endif

#endif /* SID_H */
