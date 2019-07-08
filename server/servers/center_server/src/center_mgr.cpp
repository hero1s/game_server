

#include "center_mgr.h"
#include "msg_define.pb.h"
#include "game_define.h"
#include "utility/profile_manager.h"

using namespace svrlib;
using namespace Network;

namespace {

}

//--------------------------------------------------------------------------------------------
CCenterMgr::CCenterMgr() {


}

CCenterMgr::~CCenterMgr() {
}

bool CCenterMgr::Init() {
    CServerClientMgr::Init();

    m_tcpServer = std::make_shared<TCPServer>(CApplication::Instance().GetAsioContext(), "0.0.0.0", 7788, "EchoServer");
    m_tcpServer->SetConnectionCallback([](const TCPConnPtr &conn) {
        if (conn->IsConnected()) {
            LOG_DEBUG("{}, connection accepted.", conn->GetName());
        } else if (conn->IsDisconnecting()) {
            LOG_DEBUG("{}, connection disconnecting.", conn->GetName());
        }
    });
    m_tcpServer->SetMessageCallback([](const TCPConnPtr &conn, ByteBuffer &buffer) {
        LOG_DEBUG("recv msg {}", std::string(buffer.Data(), buffer.Size()));
        conn->Send("server say hello!");
    });
    m_tcpServer->Start();

    return true;
}

void CCenterMgr::ShutDown() {
    m_tcpServer->Stop();
    CServerClientMgr::ShutDown();
}


























