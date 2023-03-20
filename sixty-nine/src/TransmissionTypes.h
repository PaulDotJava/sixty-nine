#pragma once

#include <stdint.h>

namespace trans
{
	struct Name
	{
		char name[64];
	};

	struct MessageBody
	{
		char body[128];
	};

	struct Date
	{
		char date[20];
	};

	struct Message
	{
		Name sender;
		Date date;
		MessageBody body;
	};

	struct UserData
	{
		char firstName[64];
		char lastName[64];
		char email[64];
		char username[64];
		char password[64];
	};
}
