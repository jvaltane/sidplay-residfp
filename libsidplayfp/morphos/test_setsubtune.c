#include <stdio.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/playsidfp.h>

struct DosLibrary *DOSBase;

ULONG __stack = 20000;

int main (int argc, char **argv)
{
    int i = 0;
    int retval = 0;
    SHORT sampleBuf[4096];
    LONG sampleCount = 4096;
    LONG got = 0;
    BOOL tst = FALSE;
	struct PlaysidFp *player = NULL;
    BPTR fh = 0;
    LONG sidSize = 0;
    LONG actualSize = 0;
    UBYTE *sid = NULL;
	SHORT subtunes = 0;

    if (argc != 2) {
        printf("example <sid file>\n");
        return 1;
    }

    DOSBase = (struct DosLibrary *)OpenLibrary("dos.library",0L);
    if (DOSBase == NULL) {
        fprintf(stderr, "Could not open dos.library\n");
        return 1;
    }

    printf("dos.library should be open. Loading sid.\n");

    printf("OpenFile: %s\n", argv[1]);
    fh = Open(argv[1], MODE_OLDFILE);
    if (fh == 0) {
		fprintf(stderr, "Error: Could not open file: %s\n", argv[1]);
		retval = 1;
        goto error;
    }
    printf("Get file size...\n");
    (void)Seek(fh, 0, OFFSET_END);
    sidSize = Seek(fh, 0, OFFSET_CURRENT);
    (void)Seek(fh, 0, OFFSET_BEGINNING);
    printf("File size: %lu\n",sidSize);
    sid = AllocVec(sidSize, MEMF_PUBLIC|MEMF_CLEAR);
    if (sid == NULL) {
		fprintf(stderr, "Error: Could not alloc memory to file load. Wanted size: %lu\n", sidSize);
		retval = 1;
        goto error;
    }
    printf("Read file. Size %ld\n",sidSize);
    actualSize = Read(fh, sid, sidSize);
    if (actualSize != sidSize) {
		fprintf(stderr, "Error: could not read sid. actual: %ld wanted: %ld\n",actualSize, sidSize);
		retval = 1;
        goto error;
    }
    printf("Close file...\n");
    Close(fh);
    fh = 0;

    printf("Create player\n");
    player = PlaysidFpCreate(PFA_Emulation, PF_EMULATION_RESID, TAG_END);
	if (player == NULL) {
		fprintf(stderr, "Error: Could not create player\n");
		retval = 1;
        goto error;
	}
	printf("Init SID...\n");
    tst = PlaysidFpInit(player, sid, sidSize);
    if (tst == FALSE) {
		fprintf(stderr, "Error: Could not Load SID to PlaysidFP\n");
		retval = 1;
        goto error;
    }
    FreeVec(sid);
    sid = NULL;

    {
        CONST_STRPTR md5 = PlaysidFpTuneMD5(player);
        CONST struct PlaysidFpInfo *info = PlaysidFpInfo(player);
        if (info != NULL) {
            printf("Author: %s, Title: %s\n", info->Author?info->Author:"NULL", info->Title?info->Title:"NULL");
        }
        printf("MD5: %s\n", md5?md5:"NULL");
    }
    printf("Subtune (%d/%d)\n", PlaysidFpCurrentSubtune(player), PlaysidFpSubtunes(player));
    printf("Set invalid subtune: 255\n");
    tst = PlaysidFpSetSubtune(player, 255);
    if (tst == FALSE) {
        if (player != NULL) {
            printf("SF error: %ld\n", player->Error);
        }
    }
	subtunes = PlaysidFpSubtunes(player);
    printf("Subtune (%d/%d)\n", PlaysidFpCurrentSubtune(player), subtunes);
    printf("Set subtune: 2\n");
    tst = PlaysidFpSetSubtune(player, 2);
    if (tst == FALSE) {
        if (player != NULL) {
            printf("SF error: %ld\n", player->Error);
        }
    }
    printf("Subtune (%d/%d)\n", PlaysidFpCurrentSubtune(player), subtunes);

    i = 0;
    do {
        got = PlaysidFpPlay(player, sampleBuf, sampleCount);
        printf("Got samples: %d (bytes: %d)\n", got, got*sizeof(SHORT));
		PlaysidFpSetSubtune(player, ((i%(subtunes-1))+1));
        printf("Set subtune: %d/%d\n", (int)((i%(subtunes-1))+1), (int)subtunes);
    } while (got > 0 && i++ < 1000000);

error:
    printf("Releasing...\n");
    if (fh != 0) Close(fh);
	if (player != NULL) PlaysidFpFree(player);
    if (sid != NULL) FreeVec(sid);
    CloseLibrary((struct Library *)DOSBase);

	return retval;
}
