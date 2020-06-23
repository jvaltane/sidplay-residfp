#include <proto/exec.h>
#include <constructor.h>

struct Library *PlaysidFpBase;

static CONSTRUCTOR_P(init_PlaysidFpBase, 101)
{
	PlaysidFpBase = OpenLibrary("playsidfp.library", 0);

	return (PlaysidFpBase == NULL);
}

static DESTRUCTOR_P(cleanup_PlaysidFpBase, 101)
{
	CloseLibrary(PlaysidFpBase);
}

