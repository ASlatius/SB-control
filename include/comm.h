#include <Arduino.h>
#include <HC12.h>
#include "gametime.h"
#include "gamescore.h"

void commSendDisplayData(HC12 hc12, GameScore gamescore, GameTime gametime);