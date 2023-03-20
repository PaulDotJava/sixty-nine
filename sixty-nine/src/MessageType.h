#pragma once

#include <stdint.h>

enum class AppMsg : uint32_t
{
	Client_Accepted,
	Client_Acknowledged,
	Client_RegisterResp,
	Client_LoginResp,
	Client_CreateDMResp,
	Client_SendDMChunk,
	Client_SendDMs,
	Client_MessageRecieved,

	Server_Login,
	Server_Register,
	Server_CreateDM,
	Server_RequestDMChunk,
	Server_RequestDMs,
	Server_SendDirectMessage,
	Server_Disconnect
};