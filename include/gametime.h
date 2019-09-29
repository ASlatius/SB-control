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
    // Count game time in seconds
    void count()
    {
        if (true == _run) {
            if (_totalTimeSecs < _GameTimeSecs) {
                _totalTimeSecs++;
            }
        }
    }

    // Reset game time, stop when running.
    void reset()
    {
        _totalTimeSecs = 0;
        _run = false;
    }

    // Set gametime running
    void run()
    {
        _run = true;
    }

    // Stop gametime running
    void stop()
    {
        _run = false;
    }

    // return bool on running game time
    bool isRunning()
    {
        return _run;
    }

    // Increase gametime by 5 minutes (300sec)
    int upGameTime()
    {
        _GameTimeSecs += 5*60;
        if (_GameTimeSecs > 90*60) {
            _GameTimeSecs = 5*60;
        }
        return _GameTimeSecs;
    }

    // return int minutes part of Total gametime or Running game time 
    int getMinute(gametimeTime whichTime=GAME_TIME)
    {
        if (GAME_TIME == whichTime)
            return (int)_totalTimeSecs/60;
        else if (MAX_TIME == whichTime)
            return (int)_GameTimeSecs/60;
        
        return -1;
    }

    // return int seconds part of Total gametime or Running game time 
    int getSecond(gametimeTime whichTime=GAME_TIME)
    {
        if (GAME_TIME == whichTime)
            return (int)_totalTimeSecs%60;
        else if (MAX_TIME == whichTime)
            return (int)_GameTimeSecs%60;
        
        return -1;
    }

    // return string Total gametime or Running game time with ':' separator
    String getTimeString(gametimeTime whichTime=GAME_TIME)
    {
        String seconds = String(getSecond(whichTime));
        if (getSecond(whichTime) < 10) {
            seconds = "0" + seconds;
        }
        return String(getMinute(whichTime)) + ":" + seconds;
    }

    // return int how much game time remains
    int getGameTimeRemaining()
    {
        return _GameTimeSecs - _totalTimeSecs;
    }
};

#endif