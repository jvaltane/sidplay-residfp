#include <proto/exec.h>
#include <constructor.h>

struct Library *SidplayFpBase;

static CONSTRUCTOR_P(init_SidplayFpBase, 101)
{
	SidplayFpBase = OpenLibrary("sidplayfp.library", 0);

	return (SidplayFpBase == NULL);
}

static DESTRUCTOR_P(cleanup_SidplayFpBase, 101)
{
	CloseLibrary(SidplayFpBase);
}

