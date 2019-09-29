#include <CRC32.h>
#include "comm.h"
#include "gametime.h"
#include "gamescore.h"

static CRC32 crc;
static long int displaydata[4] = {0,0,0,-1};

/******************************************************************************
 * Send scores and game time in secords
 * 
 * All fields are summed in a CRC32 4th field. Values are sent using HC12 and 
 * are separated by ';' char.
 *****************************************************************************/
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

    hc12.send(displaydata, 4);  //Send as ';' separated list
    hc12.loop();                //Softserial, make it happen...
}