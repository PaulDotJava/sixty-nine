#pragma once

#include "Message.h"

struct ChatChunk
{
	uint32_t ChunkID = -1;
	uint32_t MessageCount = 0;
	Message Messages[10];

	ChatChunk() = default;

	ChatChunk(uint32_t id, uint32_t msgCount)
	{
		ChunkID = id;
		MessageCount = msgCount;
	}
};