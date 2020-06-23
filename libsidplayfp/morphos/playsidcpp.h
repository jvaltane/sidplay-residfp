#ifndef PLAYSIDCPP_H
#define PLAYSIDCPP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <clib/playsidfp_protos.h>

struct PlaysidFp *playsidcpp_create_taglist (struct TagItem *item);
struct PlaysidFp *playsidcpp_create_tags (Tag, ...);
void playsidcpp_free (struct PlaysidFp *s);

BOOL playsidcpp_set_roms(struct PlaysidFp *s, CONST UBYTE *kernal, CONST UBYTE *basic, CONST UBYTE *chargen);
BOOL playsidcpp_init (struct PlaysidFp *s, CONST UBYTE *data, ULONG data_len);
LONG playsidcpp_play (struct PlaysidFp *s, SHORT *buffer, LONG buffer_len);
BOOL playsidcpp_speed (struct PlaysidFp *s, USHORT percent);
BOOL playsidcpp_mute (struct PlaysidFp *s, UBYTE playsidcpp_number, UBYTE voice_channel, BOOL mute);
BOOL playsidcpp_filter (struct PlaysidFp *s, BOOL filter);
LONG playsidcpp_time (struct PlaysidFp *s);

CONST struct PlaysidFpInfo *playsidcpp_tune_info (struct PlaysidFp *s);

UWORD playsidcpp_current_subtune (struct PlaysidFp *s);
UWORD playsidcpp_subtunes (struct PlaysidFp *s);
BOOL playsidcpp_subtune_set (struct PlaysidFp *s, UWORD subtune);
CONST_STRPTR playsidcpp_tune_md5 (struct PlaysidFp *s);

#ifdef __cplusplus
}
#endif

#endif /* PLAYSIDCPP_H */
