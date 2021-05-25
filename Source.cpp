#include "Util/Log.h"
#include "instPGE.h"

int main()
{
	debug("Entered Main.");

	if (instPGE::getInstance()->Construct(1024, 512, 1, 1))
		instPGE::getInstance()->Start();
	debug("Leaving Main.");
	return 0;
}