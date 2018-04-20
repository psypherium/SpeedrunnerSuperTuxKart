//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013-2015 Glenn De Jonghe
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

#ifndef HEADER_SERVER_HPP
#define HEADER_SERVER_HPP

/**
  * \defgroup onlinegroup Online
  * Represents a server that is joinable
  */

#include "network/transport_address.hpp"
#include "race/race_manager.hpp"
#include "utils/types.hpp"

#include <irrString.h>

#include <string>

class XMLNode;

/**
 * \ingroup online
 */
class Server
{
public:

protected:
    /** The server name to be displayed. */
    irr::core::stringw m_name;

    /** Name in lower case for comparisons. */
    std::string m_lower_case_name;

    uint32_t m_server_id;
    uint32_t m_server_owner;

    /** The maximum number of players that the server supports */
    int m_max_players;

    /** The number of players currently on the server */
    int m_current_players;

    /** The score/rating given */
    float m_satisfaction_score;

    /** The public ip address and port of this server. */
    TransportAddress m_address;

    /** This is the private port of the server. This is used if a WAN game
     *  is started, but one client is discovered on the same LAN, so a direct
     *  connection using the private port with a broadcast is possible. */
    uint16_t m_private_port;

    unsigned m_server_mode;

    RaceManager::Difficulty m_difficulty;

    bool m_password_protected;

    /* WAN server only, show the owner name of server, can only be seen
     * for localhost or if you are friend with the server owner. */
    std::string m_server_owner_name;

    /* WAN server only, distance based on IP latitude and longitude. */
    float m_distance;
public:

         /** Initialises the object from an XML node. */
         Server(const XMLNode &xml);
         Server(unsigned server_id, const irr::core::stringw &name,
                int max_players, int current_players, unsigned difficulty,
                unsigned server_mode, const TransportAddress &address,
                bool password_protected);
    bool filterByWords(const irr::core::stringw words) const;
    // ------------------------------------------------------------------------
    /** Returns ip address and port of this server. */
    const TransportAddress& getAddress() const { return m_address; }
    // ------------------------------------------------------------------------
    /** Returns the lower case name of the server. */
    const std::string& getLowerCaseName() const { return m_lower_case_name; }
    // ------------------------------------------------------------------------
    /** Returns the name of the server. */
    const irr::core::stringw& getName() const { return m_name; }
    // ------------------------------------------------------------------------
    /** Returns the ID of this server. */
    const uint32_t getServerId() const { return m_server_id; }
    // ------------------------------------------------------------------------
    /** Returns the user id in STK addon server of the server owner (WAN). */
    const uint32_t getServerOwner() const { return m_server_owner; }
    // ------------------------------------------------------------------------
    uint16_t getPrivatePort() const { return m_private_port; }
    // ------------------------------------------------------------------------
    /** Returns the maximum number of players allowed on this server. */
    const int getMaxPlayers() const { return m_max_players; }
    // ------------------------------------------------------------------------
    /** Returns the number of currently connected players. */
    const int getCurrentPlayers() const { return m_current_players; }
    // ------------------------------------------------------------------------
    unsigned getServerMode() const                    { return m_server_mode; }
    // ------------------------------------------------------------------------
    RaceManager::Difficulty getDifficulty() const      { return m_difficulty; }
    // ------------------------------------------------------------------------
    bool isPasswordProtected() const           { return m_password_protected; }
    // ------------------------------------------------------------------------
    const std::string& getServerOwnerName() const
                                                { return m_server_owner_name; }
    // ------------------------------------------------------------------------
    float getDistance() const                            { return m_distance; }

};   // Server
#endif // HEADER_SERVER_HPP
