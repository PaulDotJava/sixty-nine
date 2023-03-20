#pragma once

#include "User.h"
#include "ChatChunk.h"
#include "MessageType.h"

#include "olcPGEX_Network.h"

#include <vector>
#include <set>

class NetClient;

class DirectChat
{
public:
	DirectChat(char partnerName[64], int chatID);
	~DirectChat();
public:
	void RequestNewChunk(NetClient* client, uint32_t off);
	void RequestNewChunk(NetClient* client);
	void LoadChunk(ChatChunk* chunk);
	bool HasChunk(uint32_t id);
	bool PopChunkRequestQueue();
public:
	inline char* GetName() { return m_PartnerName; };
	inline uint32_t GetID() { return m_ChatID; };
	inline std::vector<ChatChunk*> GetChunks() const { return m_Chunks; };
	ChatChunk* GetChunkByID(uint32_t id);
	ChatChunk* GetLatestChunk();
public:
	static void RequestAllChats(olc::net::client_interface<AppMsg>& client, const std::string& username);
	static DirectChat* GetChatByName(const std::string& chatName);
	static DirectChat* GetChatByID(uint32_t id);
private:
	void RequestChunk(int32_t id, NetClient* client);
private:
	static std::set<DirectChat*> s_Instances;
private:
	uint32_t m_ChunkRequestQueue;
	ChatChunk* m_LatestChunk = nullptr;
	char m_PartnerName[64];
	std::vector<ChatChunk*> m_Chunks;
	uint32_t m_ChatID = -1;
};