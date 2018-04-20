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

#ifndef HEADER_NETWORKING_LOBBY_HPP
#define HEADER_NETWORKING_LOBBY_HPP

#include "guiengine/screen.hpp"
#include "guiengine/widgets/text_box_widget.hpp"
#include <memory>
#include <tuple>

class Server;
class STKPeer;

namespace GUIEngine
{ 
    class ButtonWidget;
    class LabelWidget;
    class ListWidget;
    class IconButtonWidget;
    class TextBoxWidget;
}

namespace irr
{
    namespace gui
    {
        class STKModifiedSpriteBank;
    }
}

/**
  * \brief Handles the networking lobby
  * \ingroup states_screens
  */
class NetworkingLobby : public GUIEngine::Screen,
                        public GUIEngine::ScreenSingleton<NetworkingLobby>,
                        public GUIEngine::ITextBoxWidgetListener
{
private:
    enum LobbyState
    {
        LS_ADD_PLAYERS,
        LS_CONNECTING
    } m_state;

    friend class GUIEngine::ScreenSingleton<NetworkingLobby>;

    NetworkingLobby();

    std::shared_ptr<Server> m_joined_server;
    std::weak_ptr<STKPeer> m_server_peer;
    std::vector<core::stringw> m_server_info;
    int m_server_info_height;

    GUIEngine::IconButtonWidget* m_back_widget;
    GUIEngine::LabelWidget* m_header;
    GUIEngine::LabelWidget* m_text_bubble;
    GUIEngine::IconButtonWidget* m_exit_widget;
    GUIEngine::IconButtonWidget* m_start_button;
    GUIEngine::ListWidget* m_player_list;
    GUIEngine::TextBoxWidget* m_chat_box;
    GUIEngine::ButtonWidget* m_send_button;

    irr::gui::STKModifiedSpriteBank* m_icon_bank;

    /** \brief implement optional callback from parent class GUIEngine::Screen */
    virtual void unloaded();

    virtual void onTextUpdated() {}
    virtual bool onEnterPressed(const irr::core::stringw& text) OVERRIDE
    {
        sendChat(text);
        return true;
    }

    void sendChat(irr::core::stringw text);

public:

    virtual void onUpdate(float delta) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget, const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void beforeAddingWidget() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void tearDown() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual bool onEscapePressed() OVERRIDE;

    void finishAddingPlayers();
    void addMoreServerInfo(core::stringw info);
    void setJoinedServer(std::shared_ptr<Server> server)
    {
        m_joined_server = server;
        m_server_info.clear();
    }
    void updatePlayers(const std::vector<std::tuple<uint32_t/*host id*/,
                       uint32_t/*online id*/, core::stringw/*player name*/,
                       int/*icon id*/> >& p);
    void addSplitscreenPlayer(irr::core::stringw name);
    void cleanAddedPlayers();

};   // class NetworkingLobby

#endif
