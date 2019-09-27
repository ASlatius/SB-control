#include "WString.h"

#ifndef _GAMESCORE
#define _GAMESCORE

enum gamescoreScore {
    SCORE_HOME,
    SCORE_GUEST,
};

class GameScore {
    
private:
    int _homeScore = 0;
    int _guestScore = 0;

 public:
    void reset()
    {
        _homeScore = _guestScore = 0;
    }

    int incScore(gamescoreScore which=SCORE_HOME)
    {
        if (SCORE_HOME == which) {
            if (_homeScore < 99)
                _homeScore++;
            return _homeScore;
        }
        else {
            if (_guestScore < 99)
                _guestScore++;
            return _guestScore;
        }
    }

    int decScore(gamescoreScore which=SCORE_HOME)
    {
        if (SCORE_HOME == which) {
            if (_homeScore > 0)
                _homeScore--;
            return _homeScore;
        }
        else {
            if (_guestScore > 0)
                _guestScore--;
            return _guestScore;
        }
    }

    String getScore(gamescoreScore which=SCORE_HOME)
    {
        if (SCORE_HOME == which)
            return String(_homeScore);
        else
            return String(_guestScore);
    }
};

#endif