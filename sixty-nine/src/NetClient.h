#pragma once

#include <cstdint>

#define OLC_PGEX_NETWORK
#include "olcPGEX_Network.h"

#include "ApplicationState.h"
#include "UIState.h"

#include "MessageType.h"
#include "DirectChat.h"

#include "TransmissionTypes.h"

class NetClient : olc::net::client_interface<AppMsg>
{
public:
	NetClient(ApplicationState& state, UIState& uistate, const std::string& host, int port);
public:
	void Update();
public:
	void SendNetMessage(olc::net::message<AppMsg> msg);
	void Disconnect();
private:
	ApplicationState& m_State;
	UIState& m_UIState;
};