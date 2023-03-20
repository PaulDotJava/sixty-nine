#include "ShockBaseLib/ShockBaseLib.h"

#include "ApplicationState.h"
#include "NetClient.h"
#include "UIState.h"

#include "MessageType.h"
#include "TransmissionTypes.h"

#include "SmokeObject.h"
#include "Background.h"

#define CHUNKS_PER_LOAD 2

class SNWin : public sbl::Window
{
private:
	void Start() override
	{
		INFO("App started!");

		std::vector<sbl::Vertex> vertices;
		vertices.push_back({ {-0.5f,  0.5f, 0.0}, {0.0f, 0.0} });     // top left
		vertices.push_back({ { 0.5f,  0.5f, 0.0}, {1.0f, 0.0} });     // top right
		vertices.push_back({ {-0.5f, -0.5f, 0.0}, {0.0f, 1.0} });     // bottom left
		vertices.push_back({ { 0.5f, -0.5f, 0.0}, {1.0f, 1.0} });     // bottom right

		sbl::VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_Vao.Init();
		m_Vao.AddVertexBuffer(vertices, vertices.size() * sizeof(sbl::Vertex), layout);

		GLuint indices[6] = {
			0, 1, 2,
			1, 2, 3
		};

		m_Vao.AddIndexBuffer(indices, 6 * sizeof(GLuint));

		m_Shader.Init("shader.vs", "shader.fs", true);
		glm::mat4 projMat = glm::ortho(0.0f, (float)m_Width, 0.0f, (float)m_Height, -1.0f, 1.0f);
		glm::mat4 viewMat = glm::mat4(1.0f);

		m_Shader.SetUniformMat4f("u_ProjectionMatrix", projMat);
		m_Shader.SetUniformMat4f("u_ViewMatrix", viewMat);
		m_Shader.SetUniform1i("u_Sampler", 0);

		m_SmokeTexture.Init("./res/textures/SmokeTexture.PNG");

		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.1, 0.1f);
		ImGui::GetStyle().Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.001, 0.001, 0.001, 0.0001f);

		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.1, 0.1f);
		ImGui::GetStyle().Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.001, 0.001, 0.001, 0.0001f);

		m_Background.Init();
	}

	void Update(double dt) override
	{
		if (m_Client)
			m_Client->Update();

		for (int i = 0; i < m_Objects.size(); i++)
		{
			m_Objects.at(i)->Update(dt);

			if (!m_Objects.at(i)->IsAlive())
			{
				delete m_Objects.at(i);
				m_Objects.erase(m_Objects.begin() + i);
			}
		}

		if (m_UIState.NewMessage != 0)
		{
			for (int i = 0; i < m_UIState.NewMessage; i++)
			{
				m_Objects.push_back(new SmokeObject(m_Color, m_MinScale, m_MaxScale, m_MinVel, m_MaxVel, m_MinRotVel, m_MaxRotVel, m_MinLifeTime, m_MaxLifeTime, m_MinGrothFactor, m_MaxGrothFactor));
				INFO("Spawning smoke!");
			}
			m_UIState.NewMessage = 0;
		}
	}

	void Render() override
	{
		m_Background.RenderLower(*m_Renderer.get(), m_Shader, m_Vao);

		for (auto obj : m_Objects)
		{
			obj->Render(*m_Renderer.get(), m_Shader, m_Vao, m_SmokeTexture);
		}

		m_Background.RenderUpper(*m_Renderer.get(), m_Shader, m_Vao);
	}

	void ImGuiRender() override
	{
		ImGui::Begin("ChatApp", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::DockSpaceOverViewport();
		ImGui::End();

		if (!m_State.Connected)
		{
			ShowConnect();
		}
		if (!m_State.LoggedIn && m_State.Connected)
		{
			ShowLogin();
		}

		ImGui::Begin("Editor");
		ImGui::Text("%i Object on screen", m_Objects.size());
		ImGui::SliderInt("n", &m_N, 1, 100);
		ImGui::SliderFloat4("Color", &m_Color.x, 0.0f, 1.0f);

		ImGui::SliderFloat("Min velocity", &m_MinVel, -100.0f, 100.0f);
		ImGui::SliderFloat("Max velocity", &m_MaxVel, -100.0f, 100.0f);

		ImGui::SliderFloat("Min rotation velocity", &m_MinRotVel, -100, 100);
		ImGui::SliderFloat("Max rotation velocity", &m_MaxRotVel, -100, 100);

		ImGui::SliderFloat("Min life time", &m_MinLifeTime, 1.0f, 10.0f);
		ImGui::SliderFloat("Max life time", &m_MaxLifeTime, 1.0f, 10.0f);

		ImGui::SliderFloat("Min scale", &m_MinScale, 50.0f, 100.0f);
		ImGui::SliderFloat("Max scale", &m_MaxScale, 50.0f, 100.0f);

		ImGui::SliderFloat("Min groth factor", &m_MinGrothFactor, 1.0f, 10.0f);
		ImGui::SliderFloat("Max groth factor", &m_MaxGrothFactor, 1.0f, 10.0f);

		if (ImGui::Button("Spawn"))
		{
			for (int i = 0; i < m_N; i++)
			{
				m_Objects.push_back(new SmokeObject(m_Color, m_MinScale, m_MaxScale, m_MinVel, m_MaxVel, m_MinRotVel, m_MaxRotVel, m_MinLifeTime, m_MaxLifeTime, m_MinGrothFactor, m_MaxGrothFactor));
				//INFO("+ Object spawned!");
			}

		}
		ImGui::End();

		ImGui::Begin("Chats", NULL, ImGuiWindowFlags_NoBackground || ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Create direct message"))
		{
			ImGui::OpenPopup("Create DM");
		}
		//ImGui::SameLine();
		/*if (ImGui::Button("Create group chat"))
		{

		}*/

		ShowCreateDM();		// ...only if it was opended

		ImGui::Separator();

		static std::string selected = "";
		static DirectChat* dirChat = nullptr;

		for (int i = 0; i < m_State.DirectChats.size(); i++)
		{
			if (ImGui::Selectable(m_State.DirectChats.at(i)->GetName(), selected == m_State.DirectChats.at(i)->GetName()))
			{
				selected = m_State.DirectChats.at(i)->GetName();
				dirChat = DirectChat::GetChatByName(selected);
				dirChat->RequestNewChunk(m_Client.get(), CHUNKS_PER_LOAD);
			}
		}
		ImGui::End();

		ImGui::Begin("Chat");

		//static GroupChat* gChat = DirectChat::GetChatByName(selected);

		if (dirChat)
		{
			static std::vector<ChatChunk*> chunks;
			chunks = dirChat->GetChunks();
			//ImGui::Beg
			for (auto chunk : chunks)
			{
				//ImGui::Text(std::to_string(chunk->ChunkID).c_str());
				for (int i = 0; i < chunk->MessageCount; i++)
				{
					ImGui::TextColored(ImVec4(0.8f, 1.0f, 0.8f, 1.0f), chunk->Messages[i].Sender);
					ImGui::SameLine();
					ImGui::Text(chunk->Messages[i].Body);
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.8f, 8.0f, 1.0f, 1.0f), chunk->Messages[i].Date);
				}
			}

			static std::string messageInput;
			if (ImGui::Button("Send", ImVec2(120, 0)))
			{
				if (messageInput != "")
				{
					olc::net::message<AppMsg> msg;
					msg.header.id = AppMsg::Server_SendDirectMessage;
					if (!dirChat->GetLatestChunk())
					{
						msg << 0;
					}
					else if (dirChat->GetLatestChunk()->MessageCount < 10)
					{
						msg << dirChat->GetLatestChunk()->ChunkID;
					}
					else {
						msg << (dirChat->GetLatestChunk()->ChunkID + 1);
					}
					msg << dirChat->GetID();

					trans::Name n{};
					memcpy(n.name, m_State.Username.data(), m_State.Username.size());
					msg << n;

					trans::MessageBody b{};
					memcpy(b.body, messageInput.data(), messageInput.size());
					msg << b;

					m_Client->SendNetMessage(msg);

					messageInput = "";
				}

			}
			ImGui::SameLine();
			ImGui::InputText(" ", &messageInput);
			ImGui::SameLine();
			if (ImGui::Button("Load more"))
			{
				dirChat->RequestNewChunk(m_Client.get(), CHUNKS_PER_LOAD);
			}
		}
		ImGui::End();
	}

	void Delete() override
	{
		if(m_State.Connected)
			m_Client->Disconnect();
	}

private:
	void ShowCreateDM()
	{
		if (ImGui::BeginPopupModal("Create DM", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Who do you want to message?");
			static char username[64];
			static std::string error = "";
			ImGui::InputText(" ", username, 64);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), error.c_str());
			ImGui::Spacing();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				olc::net::message<AppMsg> msg;
				msg.header.id = AppMsg::Server_CreateDM;

				char u[64];
				memcpy(u, m_State.Username.c_str(), 64);

				msg << u;
				msg << username;

				m_Client->SendNetMessage(msg);
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			if (m_UIState.CreateDMSuccess == Success::FAIL)
			{
				error = "You to already share a direct message!";
				m_UIState.CreateDMSuccess = Success::ZERO;
			}
			else if (m_UIState.CreateDMSuccess == Success::SUCCESS)
			{
				m_UIState.CreateDMSuccess = Success::ZERO;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	void ShowConnect()
	{
		ImGui::OpenPopup("Establish connection");
		if (ImGui::BeginPopupModal("Establish connection", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("What server do you want to connect to?");
			ImGui::Separator();

			static std::string ip = "127.0.0.1";
			uint32_t port = 60000;

			ImGui::InputText("IP-Address", &ip);
			ImGui::InputScalar("Port", ImGuiDataType_U32, &port, NULL, NULL, "%u");

			if (ImGui::Button("Connect", ImVec2(120, 0)))
			{
				std::cout << "connectin to " << ip << std::endl;
				m_Client = std::make_unique<NetClient>(m_State, m_UIState, ip, 60000);
			}

			ImGui::EndPopup();
		}
	}
	void ShowLogin()
	{
		ImGui::OpenPopup("Login");
		if (ImGui::BeginPopupModal("Login", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static char username[64] = "";
			ImGui::InputText("Username", username, 64);
			static char password[64] = "";
			ImGui::InputText("Password", password, 64, ImGuiInputTextFlags_Password);

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				olc::net::message<AppMsg> msg;
				msg.header.id = AppMsg::Server_Login;
				msg << username << password;
				m_Client->SendNetMessage(msg);

				m_State.Username = username;
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Sign up", ImVec2(120, 0)))
			{
				ImGui::OpenPopup("Sign up");
			}

			if (ImGui::BeginPopupModal("Sign up", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static trans::UserData ud;
				static char pwConf[64];
				ImGui::InputText("First name", ud.firstName, 64);
				ImGui::InputText("Last name", ud.lastName, 64);
				ImGui::InputText("E-Mail", ud.email, 64);
				ImGui::Separator();
				ImGui::InputText("Username", ud.username, 64);
				ImGui::InputText("Password", ud.password, 64, ImGuiInputTextFlags_Password);
				ImGui::InputText("Password (confirm)", pwConf, 64, ImGuiInputTextFlags_Password);

				static std::string error = "";

				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					if (ud.firstName[0] == 0 || ud.lastName[0] == 0 || ud.email[0] == 0 || ud.username[0] == 0 || ud.password[0] == 0 || pwConf[0] == 0)
					{
						error = "All fields must be filled out!";
					}
					else {
						if (strcmp(ud.password, pwConf) != 0)
						{
							error = "Passwords do not match!";
						}
						else {
							olc::net::message<AppMsg> msg;
							msg.header.id = AppMsg::Server_Register;
							msg << ud;
							m_Client->SendNetMessage(msg);
						}
					}
				}

				if (m_UIState.RegistrationSuccess == Success::SUCCESS)
				{
					std::cout << "Closing popup!" << std::endl;
					error = "";
					m_UIState.RegistrationSuccess = Success::ZERO;
					ImGui::CloseCurrentPopup();
				}

				if (m_UIState.RegistrationSuccess == Success::FAIL)
				{
					error = "Username already in use!";
				}

				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), error.c_str());

				ImGui::EndPopup();
			}
			ImGui::EndPopup();
		}
	}
private:
	ApplicationState m_State;
	UIState m_UIState;
	std::unique_ptr<NetClient> m_Client;

	sbl::VertexArray m_Vao;
	sbl::ShaderProgram m_Shader;
	sbl::Texture m_SmokeTexture;

	int m_N = 1;

	std::vector<SmokeObject*> m_Objects;
	glm::vec4 m_Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float m_MinVel = -47;
	float m_MaxVel = 52;

	float m_MinRotVel = -40;
	float m_MaxRotVel = 40;

	float m_MinLifeTime = 1;
	float m_MaxLifeTime = 5;

	float m_MinGrothFactor = 3.6;
	float m_MaxGrothFactor = 6;

	float m_MinScale = 50;
	float m_MaxScale = 80;

	Background m_Background;
};

int main()
{
	SNWin w;
	w.Init(1600, 800, "sixty-nine");
	w.Run();
}