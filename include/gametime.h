#include "WString.h"

#ifndef _GAMETIME
#define _GAMETIME

enum gametimeTime {
    GAME_TIME,
    MAX_TIME
};

class GameTime {
    
private:
    int _totalTimeSecs = 0;
    int _GameTimeSecs = 30*60;
    bool _run = false;

 public:
    void count()
    {
        //Serial.printf("%i %i %i %i %i", _totalTimeSecs, _GameTimeSecs, _pauzeTimeSecs, _run, _pauzed);
        if (true == _run) {
            if (_totalTimeSecs < _GameTimeSecs) {
                _totalTimeSecs++;
            }
        }
    }

    void reset()
    {
        _totalTimeSecs = 0;
        _run = false;
    }

    void run()
    {
        _run = true;
    }

    void stop()
    {
        _run = false;
    }

    bool isRunning()
    {
        return _run;
    }

    int upGameTime()
    {
        _GameTimeSecs += 5*60;
        if (_GameTimeSecs > 90*60) {
            _GameTimeSecs = 5*60;
        }
        return _GameTimeSecs;
    }

    int getMinute(gametimeTime whichTime=GAME_TIME)
    {
        if (GAME_TIME == whichTime)
            return (int)_totalTimeSecs/60;
        else if (MAX_TIME == whichTime)
            return (int)_GameTimeSecs/60;
        
        return -1;
    }

    int getSecond(gametimeTime whichTime=GAME_TIME)
    {
        if (GAME_TIME == whichTime)
            return (int)_totalTimeSecs%60;
        else if (MAX_TIME == whichTime)
            return (int)_GameTimeSecs%60;
        
        return -1;
    }

    String getTimeString(gametimeTime whichTime=GAME_TIME)
    {
        String seconds = String(getSecond(whichTime));
        if (getSecond(whichTime) < 10) {
            seconds = "0" + seconds;
        }
        return String(getMinute(whichTime)) + ":" + seconds;
    }

    int getGameTimeRemaining()
    {
        return _GameTimeSecs - _totalTimeSecs;
    }
};

#endif