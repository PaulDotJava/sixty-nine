#include "NetClient.h"

#include "ShockBaseLib/ShockBaseLib.h"

NetClient::NetClient(ApplicationState& state, UIState& uistate, const std::string& host, int port)
	: m_State(state), m_UIState(uistate)
{
	if (!Connect(host, port))
	{
		std::cout << "Unable to establish connection!" << std::endl;
	}
}

void NetClient::Update()
{
	if (!IsConnected())
	{
		std::cout << "Connection failed!" << std::endl;
		return;
	}

	while (!Incoming().empty())
	{
		auto msg = Incoming().pop_front().msg;
		switch (msg.header.id)
		{
		case AppMsg::Client_Accepted:
		{
			std::cout << "Server accepted the client!" << std::endl;
			m_State.Connected = true;
			break;
		}
		case AppMsg::Client_RegisterResp:
		{
			bool s = false;
			msg >> s;
			if (s)
			{
				m_UIState.RegistrationSuccess = Success::SUCCESS;
			}
			else {
				m_UIState.RegistrationSuccess = Success::FAIL;
			}
			std::cout << "Registration success: " << s << std::endl;
			break;
		}
		case AppMsg::Client_LoginResp:
		{
			bool s = false;
			msg >> s;
			std::cout << s << std::endl;
			m_State.LoggedIn = s;

			if (s)
			{
				DirectChat::RequestAllChats(*this, m_State.Username);
			}
			break;
		}
		case AppMsg::Client_CreateDMResp:
		{
			bool s = false;
			msg >> s;
			if (s)
			{
				m_UIState.CreateDMSuccess = Success::SUCCESS;
			}
			else {
				m_UIState.CreateDMSuccess = Success::FAIL;
			}
			break;
		}
		case AppMsg::Client_SendDMs:
		{
			std::cout << "All direct chats arrived!" << std::endl;
			char name[64];
			uint32_t id;

			int size = msg.size();

			m_State.DirectChats.clear();

			for (int i = 0; i < size / 68; i++)	// 70 = size of one dm (name = 64 bytes, id = 4 bytes)
			{
				msg >> id >> name;

				m_State.DirectChats.push_back(std::make_unique<DirectChat>(name, id));
				//memset(name, '\0', 64);

			}

			std::cout << "lol" << std::endl;

			break;
		}
		case AppMsg::Client_SendDMChunk:
		{
			std::cout << msg << std::endl;
			int32_t chunkID;
			uint32_t chatID;
			msg >> chunkID >> chatID;
			std::cout << "Recieved dm chunk" << chunkID << std::endl;
			std::cout << msg << std::endl;

			std::vector<trans::Message> msgs;
			trans::Message transmissionMsg{};
			while (true)
			{
				if (msg.size() < sizeof(trans::Message))
				{
					std::cout << msg.size() << " couldn't be read!" << std::endl;
					break;
				}
				std::cout << "reading message..." << std::endl;
				msg >> transmissionMsg;

				//std::cout << transmissionMsg.sender.name << std::endl;
				//std::cout << transmissionMsg.date.date << std::endl;
				//std::cout << transmissionMsg.body.body << std::endl;

				msgs.push_back(transmissionMsg);
			}

			if (msgs.size() > 10)
			{
				std::cout << "Error: recieved chunk contains too many messages!" << std::endl;
			}

			DirectChat* chat = DirectChat::GetChatByID(chatID);
			if (!chat)
			{
				std::cout << "Error: recieved chunk from unknowen chat!" << std::endl;
			}
			else {
				ChatChunk* chunk = chat->GetChunkByID(chunkID);
				if (!chunk)
				{
					chunk = new ChatChunk();
					chunk->ChunkID = chunkID;
					chat->LoadChunk(chunk);
				}
				else {
					std::cout << "chunk found" << std::endl;
				}

				chunk->MessageCount = 0;
				for (int i = 0; i < msgs.size(); i++)
				{
					memcpy(chunk->Messages[i].Sender, msgs.at(i).sender.name, sizeof(Message));
					chunk->MessageCount++;
				}
			}

			if (chat->PopChunkRequestQueue())
				chat->RequestNewChunk(this);

			break;
		}
		case AppMsg::Client_MessageRecieved:
		{
			INFO("Message recieved!");
			msg >> m_UIState.NewMessage;
		}
		default:
		{

		}

		}
	}
}

void NetClient::SendNetMessage(olc::net::message<AppMsg> msg)
{
	Send(msg);
}

void NetClient::Disconnect()
{
	olc::net::message<AppMsg> msg;
	msg.header.id = AppMsg::Server_Disconnect;
	Send(msg);
	std::cout << "Disconnecting" << std::endl;
}