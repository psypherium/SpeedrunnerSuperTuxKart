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

#include "challenges/speedrun_timer.hpp"
#include "config/player_manager.hpp"

SpeedrunTimer *speedrun_timer = 0;

SpeedrunTimer::SpeedrunTimer()
{
    m_valid_speedrun_started = false;
    m_valid_speedrun_ended = false;
    m_pause_active = false;
    m_loading = false;
    m_player_tested = false;
    m_player_can_run = false;
    m_total_pause_time.zero();
    m_milliseconds = 0;
}  // SpeedrunTimer

void SpeedrunTimer::startSpeedrunTimer()
{
	if (!m_valid_speedrun_started)
		m_speedrun_start = std::chrono::system_clock::now();
	m_valid_speedrun_started = true;
}

void SpeedrunTimer::stopSpeedrunTimer()
{
	if (m_valid_speedrun_started)
    {
		m_speedrun_end = std::chrono::system_clock::now();
		m_valid_speedrun_ended = true;
	}
}

void SpeedrunTimer::pauseSpeedrunTimer(bool loading)
{
    //Don't change the pause time if there is no run,
    //if it is finished, or if it is already set.
	if ( !m_valid_speedrun_started || m_pause_active
       || m_valid_speedrun_ended)
		return;
    m_pause_start = std::chrono::system_clock::now();
    m_pause_active = true;    
    m_loading = loading;
}

void SpeedrunTimer::unpauseSpeedrunTimer()
{
    //Don't unpause if there is no run or no previous pause
	if (!m_valid_speedrun_started || !m_pause_active || m_valid_speedrun_ended)
		return;
    std::chrono::time_point<std::chrono::system_clock> now(std::chrono::system_clock::now());
    m_total_pause_time += now - m_pause_start;
    int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(m_total_pause_time).count();
    Log::verbose("SpeedrunTimer", "Total pause time %ims.",milliseconds);
    m_pause_active = false;
}

void SpeedrunTimer::updateTimer()
{
    //The game loop call this only once before loading is finished
    if (!m_loading)
    {
        unpauseSpeedrunTimer();
    }
    else
    {
        m_loading = false;
    }

	std::chrono::duration<double> elapsedTime;

	if (m_valid_speedrun_ended)
    {
		elapsedTime = m_speedrun_end - m_speedrun_start - m_total_pause_time;
    }
	else
    {
		std::chrono::time_point<std::chrono::system_clock> now(std::chrono::system_clock::now());
		elapsedTime = now - m_speedrun_start - m_total_pause_time;
	}
	
	m_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();   
}

//Check if the current player has already entered story mode or not
void SpeedrunTimer::testPlayerRun()
{
    PlayerProfile *player = PlayerManager::getCurrentPlayer();

    if (player->isFirstTime())
    {
        m_player_can_run = true;
    }

    m_player_tested = true;
}

void SpeedrunTimer::playerHasChanged()
{
    m_player_can_run = false;
    m_player_tested = false;
    m_valid_speedrun_started = false;
    std::chrono::time_point<std::chrono::system_clock> now(std::chrono::system_clock::now());
    m_total_pause_time = now - now;
}

std::string SpeedrunTimer::getSpeedrunTimerString()
{
    if (!m_player_tested)
        testPlayerRun();

    //TODO use string utils to have translatable strings
	if (!m_valid_speedrun_started)
    {
        if(m_player_can_run)
            return "Run not started.";
        else
            return "Can only run if story mode\nhas not been entered before.\nPlease use a new profile,\nor disable the story mode timer\nin the user interface options.";
    }

    updateTimer();

	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << m_milliseconds/3600000 << ":"
	    << std::setw(2) << std::setfill('0') << m_milliseconds/60000 % 60 << ":"
	    << std::setw(2) << std::setfill('0') << m_milliseconds/1000 % 60 << "."
	    << std::setw(3) << std::setfill('0') << m_milliseconds % 1000;
	return oss.str();
}

/* EOF */
