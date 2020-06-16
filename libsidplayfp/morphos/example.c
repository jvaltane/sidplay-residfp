#include <stdio.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/sidplayfp.h>

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
	struct SidplayFp *sf = NULL;
    BPTR fh = 0;
    LONG sidSize = 0;
    LONG actualSize = 0;
    UBYTE *sid = NULL;
	struct SidplayFpNew sfn =
	{
		SFN_EMULATION_RESIDFP,
		SFN_SID_MODEL_MOS6581,
		FALSE,
		SFN_MACHINE_TYPE_PAL,
		FALSE,
		SFN_CIA_MODEL_MOS6526,
		SFN_SAMPLING_METHOD_INTERPOLATE,
		FALSE,
		FALSE,
		SFN_PLAYBACK_MONO,
		0.5f,
		0.5f,
		0.5f,
		44100,
	};

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
    sf = SidplayFpCreate(&sfn);
	if (sf == NULL) {
		fprintf(stderr, "Error: Could not create player\n");
		retval = 1;
        goto error;
	}
	printf("Init SID...\n");
    tst = SidplayFpInit(sf, sid, sidSize);
    if (tst == FALSE) {
		fprintf(stderr, "Error: Could not Load SID to SidplayFP\n");
		retval = 1;
        goto error;
    }
    FreeVec(sid);
    sid = NULL;

    {
        CONST_STRPTR md5 = SidplayFpTuneMD5(sf);
        CONST struct SidplayFpInfo *info = SidplayFpInfo(sf);
        if (info != NULL) {
            printf("Author: %s, Title: %s\n", info->Author?info->Author:"NULL", info->Title?info->Title:"NULL");
        }
        printf("MD5: %s\n", md5?md5:"NULL");
        printf("Subtune (%d/%d)\n", SidplayFpCurrentSubtune(sf), SidplayFpSubtunes(sf));
    }

    i = 0;
    do {
        got = SidplayFpPlay(sf, sampleBuf, sampleCount);
        printf("Got samples: %d (bytes: %d)\n", got, got*sizeof(SHORT));
    } while (got > 0 && i++ < 10);

error:
    printf("Releasing...\n");
    if (fh != 0) Close(fh);
	if (sf != NULL) SidplayFpFree(sf);
    if (sid != NULL) FreeVec(sid);
    CloseLibrary((struct Library *)DOSBase);

	return retval;
}
