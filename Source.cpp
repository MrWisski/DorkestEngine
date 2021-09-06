#include "Util/Log.h"
#include <SFML/Graphics.hpp>
#include <sheetEditor.h>

int main()
{
	debug("Entered Main.");
	debug("Creating engine");
	sheetEditor e;
	//Engine e;
	debug("Initializing engine.");
	if (!e.initSheetEditor()) {
		return -1;
	}
	
	
	sf::Clock clock;
	sf::Time elapsed = clock.restart();
	while (e.update(elapsed.asMicroseconds())){
		elapsed = clock.restart();
	}

	debug("Leaving Main.");
	return 0;
}