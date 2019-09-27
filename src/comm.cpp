#include <CRC32.h>
#include "comm.h"
#include "gametime.h"
#include "gamescore.h"

static CRC32 crc;
static long int displaydata[4] = {0,0,0,-1};

void commSendDisplayData(HC12 hc12, GameScore gamescore, GameTime gametime)
{
    displaydata[0] = gamescore.getScore(SCORE_HOME).toInt();
    displaydata[1] = gamescore.getScore(SCORE_GUEST).toInt();
    displaydata[2] = gametime.getGameTimeRemaining();
    
    crc.reset();
    for (size_t i = 0; i < 3; i++) {
        crc.update(displaydata[i]);
    }
    displaydata[3] = crc.finalize();

    hc12.send(displaydata, 4);
    hc12.loop();                //Send input/output to Serial/HC-12 and run AT commands
}