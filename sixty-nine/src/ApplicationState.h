#pragma once

#include <memory>
#include "DirectChat.h"

struct ApplicationState
{
	bool Connected = false;
	bool LoggedIn = false;
	std::string Username;

	std::vector<std::unique_ptr<DirectChat>> DirectChats;
};