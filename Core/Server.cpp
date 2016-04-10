#include "stdafx.h"
#include "Server.h"
#include "Screen.h"
#include "Mouse.h"
#include "Image.h"
#include "ServerNetworkDriver.h"
#include "IServerDriver.h"
#include <thread>
#include <mutex>

namespace SL {
	namespace Remote_Access_Library {
		namespace Network {
			struct Server_Config;
		}
		class ServerImpl : public Network::IServerDriver {
		public:
			std::shared_ptr<SL::Remote_Access_Library::Utilities::Image> LastScreen;
			std::shared_ptr<SL::Remote_Access_Library::Utilities::Image> LastMouse;
			Utilities::Point LastMousePos;
			Network::ServerNetworkDriver _ServerNetworkDriver;
			Network::IBaseNetworkDriver* _IUserNetworkDriver;
			
			bool _Keepgoing;
			std::mutex _NewClientLock;
			std::vector<std::shared_ptr<Network::ISocket>> _NewClients;


			ServerImpl(Network::Server_Config& config, Network::IBaseNetworkDriver* parent) : _ServerNetworkDriver(this, config), _IUserNetworkDriver(parent)
			{
				LastMousePos = Utilities::Point(0xffffffff, 0xffffffff);
				_Keepgoing = true;
			}

			virtual ~ServerImpl() {
				_Keepgoing = false;
			}
			virtual void OnConnect(const std::shared_ptr<Network::ISocket>& socket) override {
				{
					std::lock_guard<std::mutex> lock(_NewClientLock);
					_NewClients.push_back(socket);
				}
				if (_IUserNetworkDriver != nullptr) _IUserNetworkDriver->OnConnect(socket);
			}

			virtual void OnClose(const std::shared_ptr<Network::ISocket>& socket)override {

				if (_IUserNetworkDriver != nullptr) _IUserNetworkDriver->OnClose(socket);
			}

			virtual void OnReceive(const std::shared_ptr<Network::ISocket>& socket, std::shared_ptr<Network::Packet>& packet)override {
				UNUSED(socket);
				UNUSED(packet);
			}

			void OnScreen(std::shared_ptr<Utilities::Image> img)
			{
				if (!LastScreen) {//first screen send all!
					_ServerNetworkDriver.SendScreenFull(nullptr, *img);
					std::lock_guard<std::mutex> lock(_NewClientLock);
					_NewClients.clear();
				}
				else {//compare and send all difs along
					{
						//make sure to send the full screens to any new connects
						std::lock_guard<std::mutex> lock(_NewClientLock);
						for (auto& a : _NewClients) {
							_ServerNetworkDriver.SendScreenFull(a.get(), *img);
						}
						_NewClients.clear();
					}
					if (img->data() != LastScreen->data()) {
						for (auto r : SL::Remote_Access_Library::Utilities::Image::GetDifs(*LastScreen, *img)) {
							_ServerNetworkDriver.SendScreenDif(nullptr, r, *img);
						}
					}
				}	
				LastScreen = img;//swap
			}
			void OnMouseImg(std::shared_ptr<Utilities::Image> img)
			{
				if (!LastMouse) {//first screen send all!
					_ServerNetworkDriver.SendMouse(nullptr, *img);
					std::lock_guard<std::mutex> lock(_NewClientLock);
					_NewClients.clear();
				}
				else {//compare and send all difs along
					{
						//make sure to send the full screens to any new connects
						std::lock_guard<std::mutex> lock(_NewClientLock);
						for (auto& a : _NewClients) {
							_ServerNetworkDriver.SendMouse(a.get(), *img);
						}
						_NewClients.clear();
					}
					if (memcmp(img->data(), LastScreen->data(), std::min(LastScreen->size(), img->size()))!=0) {
						_ServerNetworkDriver.SendMouse(nullptr, *img);
					}
				}
				LastMouse = img;
			}
			void OnMousePos(Utilities::Point p)
			{
				_ServerNetworkDriver.SendMouse(nullptr, p);
			}
	
			int Run() {
				_ServerNetworkDriver.Start();
				auto sc = std::make_unique<Capturing::Screen>(std::bind(&SL::Remote_Access_Library::ServerImpl::OnScreen, this, std::placeholders::_1));
				auto ms = std::make_unique<Capturing::Mouse>(std::bind(&SL::Remote_Access_Library::ServerImpl::OnMouseImg, this, std::placeholders::_1),
					std::bind(&SL::Remote_Access_Library::ServerImpl::OnMousePos, this, std::placeholders::_1), 1000, 50);
				while (_Keepgoing) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
				_ServerNetworkDriver.Stop();
				return 0;
			}

		};

	}
}

SL::Remote_Access_Library::Server::Server(Network::Server_Config& config, Network::IBaseNetworkDriver* parent)
{
	_ServerImpl = std::make_shared<ServerImpl>(config, parent);
}

SL::Remote_Access_Library::Server::~Server()
{

}

int SL::Remote_Access_Library::Server::Run()
{
	return _ServerImpl->Run();

}

