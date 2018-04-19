//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006-2015 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_SPEEDRUN_TIMER_HPP
#define HEADER_SPEEDRUN_TIMER_HPP

#include <chrono>
#include <sstream>
#include <iomanip>

class SpeedrunTimer
{
private:
    bool m_valid_speedrun_started, m_valid_speedrun_ended;
    bool m_pause_active;
    bool m_loading;
    bool m_player_tested;
    bool m_player_can_run;

    //This stores the number of milliseconds to display with the counter
    int m_milliseconds;

	std::chrono::time_point<std::chrono::system_clock> m_speedrun_start, m_speedrun_end, m_pause_start;
    std::chrono::duration<double> m_total_pause_time;
public:

    SpeedrunTimer();

    // ------------------------------------------------------------------------
    /** Speedrun timer functions. */
    void startSpeedrunTimer();
    void stopSpeedrunTimer();
    void pauseSpeedrunTimer(bool loading);
    void unpauseSpeedrunTimer();
    void updateTimer();
    void testPlayerRun();
    void playerHasChanged();
    std::string getSpeedrunTimerString();
    bool playerCanRun() const {return m_player_can_run;}
    bool isSpeedrunning() const {return m_valid_speedrun_started;}
    bool speedrunIsFinished() const {return m_valid_speedrun_ended;}
};   // SpeedrunTimer

extern SpeedrunTimer* speedrun_timer;

#endif

/* EOF */
