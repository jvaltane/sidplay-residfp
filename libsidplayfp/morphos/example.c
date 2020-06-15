#include <stdio.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/sidplayfp.h>

#define DEFAULT_SAMPLE_BUF_SIZE 4096

struct DosLibrary *DOSBase;

ULONG __stack = 20000;

int main (int argc, char **argv)
{
	int i = 0;
	LONG samples = 0;
	LONG sampleBufSize = DEFAULT_SAMPLE_BUF_SIZE;
	SHORT sampleBuf[DEFAULT_SAMPLE_BUF_SIZE];
    int retval = 0;
    BOOL tst = FALSE;
	struct SidplayFp *sf = NULL;
    BPTR fh = 0;
    LONG sidSize = 0;
    LONG actualSize = 0;
    UBYTE *sid = NULL;
	struct SidplayFpNew sfn =
	{
		SF_EMULATION_RESIDFP,
		SF_SID_MODEL_MOS6581,
		FALSE,
		SF_MACHINE_TYPE_PAL,
		FALSE,
		SF_CIA_MODEL_MOS6526,
		SF_SAMPLING_METHOD_INTERPOLATE,
		FALSE,
		FALSE,
		SF_PLAYBACK_MONO,
		NULL,
		NULL,
		NULL,
		0.5f,
		0.5f,
		0.5f,
		44100,
		NULL
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

	printf("Load SID\n");
    tst = SidplayFpLoad(sf, sid, sidSize);
    if (tst == FALSE) {
		fprintf(stderr, "Error: Could not Load SID to SidplayFP\n");
		retval = 1;
        goto error;
    }
	printf("Loaded successfully.\n");

	do {
		samples = SidplayFpPlay(sf, sampleBuf, sampleBufSize);
		printf("Got samples %ld, bytes: %ld\n", samples, samples*sizeof(SHORT));
	} while(samples > 0 && i++ < 50);

error:
    if (fh != 0) Close(fh);
	if (sf != NULL) SidplayFpFree(sf);
    if (sid != NULL) FreeVec(sid);
    CloseLibrary((struct Library *)DOSBase);

	printf("Leaving with retval (%d)\n", retval);

	return retval;
}
