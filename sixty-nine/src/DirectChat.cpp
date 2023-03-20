#include "DirectChat.h"

#include "NetClient.h"

#include <algorithm>

std::set<DirectChat*> DirectChat::s_Instances;

DirectChat::DirectChat(char partnerName[64], int chatID)
	: m_ChatID(chatID)
{
	s_Instances.emplace(this);
	memcpy(m_PartnerName, partnerName, 64);
}

DirectChat::~DirectChat()
{
	for (int i = 0; i < m_Chunks.size(); i++)
	{
		delete m_Chunks.at(i);
	}

	s_Instances.erase(this);
}

void DirectChat::LoadChunk(ChatChunk* chunk)
{
	m_Chunks.push_back(chunk);

	std::sort(m_Chunks.begin(), m_Chunks.end(), 
		[](const ChatChunk* a, const ChatChunk* b) {
			return (a->ChunkID < b->ChunkID);
		});

	m_LatestChunk = m_Chunks.back();
}

void DirectChat::RequestNewChunk(NetClient* client, uint32_t off)
{
	m_ChunkRequestQueue = off;

	RequestNewChunk(client);
}

void DirectChat::RequestNewChunk(NetClient* client)
{
	int id = -1;

	std::cout << "number of chunks so far " << m_Chunks.size() << std::endl;

	if (m_Chunks.size() != 0)
	{
		id = m_Chunks.front()->ChunkID - 1;
		if (id < 0)
		{
			m_ChunkRequestQueue = 0;
			return;
		}
	}

	RequestChunk(id, client);
}

bool DirectChat::PopChunkRequestQueue()
{
	m_ChunkRequestQueue--;
	return m_ChunkRequestQueue;
}

bool DirectChat::HasChunk(uint32_t id)
{
	for (int i = 0; i < m_Chunks.size(); i++)
	{
		if (m_Chunks.at(i)->ChunkID == id)
			return true;
	}

	return false;
}

void DirectChat::RequestAllChats(olc::net::client_interface<AppMsg>& client, const std::string& username)
{
	char u[64] = "";
	memcpy(u, username.data(), username.size());

	olc::net::message<AppMsg> msg;
	msg.header.id = AppMsg::Server_RequestDMs;
	msg << u;
	client.Send(msg);
}

DirectChat* DirectChat::GetChatByName(const std::string& chatName)
{
	char cname[64] = "";
	memcpy(cname, chatName.data(), chatName.size());

	for (auto i : s_Instances)
	{
		if (strcmp(i->GetName(), cname) == 0)
		{
			return i;
		}
	}

	return nullptr;
}

DirectChat* DirectChat::GetChatByID(uint32_t id)
{
	for (auto i : s_Instances)
	{
		if (i->m_ChatID == id)
		{
			return i;
		}
	}

	return nullptr;
}

ChatChunk* DirectChat::GetLatestChunk()
{
	return m_LatestChunk;
}

ChatChunk* DirectChat::GetChunkByID(uint32_t id)
{
	for (const auto& i : m_Chunks)
	{
		if (i->ChunkID == id)
			return i;
	}

	return nullptr;
}

void DirectChat::RequestChunk(int32_t id, NetClient* client)
{
	olc::net::message<AppMsg> msg;
	msg.header.id = AppMsg::Server_RequestDMChunk;

	std::cout << "Requesting chatID: " << m_ChatID << " chunk: " << id << std::endl;

	msg << m_ChatID;
	msg << id;
	client->SendNetMessage(msg);
}