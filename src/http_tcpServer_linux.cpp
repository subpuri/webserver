#include <http_tcpServer_linux.h>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <thread> // for multi threading


namespace
{
    const int BUFFER_SIZE = 30720;

    void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string &errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }
}

namespace http
{

    TcpServer::TcpServer(std::string ip_address, int port) : m_ip_address(ip_address), m_port(port), m_socket(),
                                                             m_incomingMessage(),
                                                             m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress)),
                                                             m_serverMessage(buildResponse())
    {
        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

        if (startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
            log(ss.str());
        }
    }

    TcpServer::~TcpServer()
    {
        closeServer();
    }

    int TcpServer::startServer()
    {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0)
        {
            exitWithError("Cannot create socket");
            return 1;
        }
        int opt = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            exitWithError("setsockopt(SO_REUSEADDR) failed");
        }


        if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }

        return 0;
    }

    void TcpServer::closeServer()
    {
        close(m_socket);
        exit(0);
    }

    // Multi thread server
    void TcpServer::handleClient(int client_socket)
    {
        char buffer[BUFFER_SIZE] = {0};
        int bytesReceived = read(client_socket, buffer, BUFFER_SIZE);

        if (bytesReceived < 0)
        {
            exitWithError("Failed to read from client");
        }

        log("------ Received Request from client ----\n\n");

        std::string response = buildResponse();
        long bytesSent = write(client_socket, response.c_str(), response.size());

        if (bytesSent == response.size())
        {
            log("----- Server Response sent to client ----\n\n");
        }
        else if (bytesSent <= 0)
        {
            log("Error: Client closed connection or write failed");
        }
        else
        {
            std::ostringstream ss;
            ss << "Warning: Partial response sent: " << bytesSent << "/" << response.size();
            log(ss.str());
        }

        close(client_socket);
    }


    void TcpServer::startListen()
    {
        if (listen(m_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
        log(ss.str());

        int bytesReceived;

        while (true)
        {
            log("====== Waiting for a new connection ======\n\n\n");
            // read, respond, close
            //acceptConnection(m_new_socket);
            //For multi thread server 
            // acceptConnection(m_new_socket);
            // std::thread clientThread(&TcpServer::handleClient, this, m_new_socket);
            // clientThread.detach(); // allow the thread to run independently


            // char buffer[BUFFER_SIZE] = {0};
            // bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
            // if (bytesReceived < 0)
            // {
            //     exitWithError("Failed to read bytes from client socket connection");
            // }

            // std::ostringstream ss;
            // ss << "------ Received Request from client ------\n\n";
            // log(ss.str());

            // sendResponse();

            // close(m_new_socket);
            int client_socket;
            acceptConnection(client_socket);

            std::thread clientThread(&TcpServer::handleClient, this, client_socket);
            clientThread.detach();

        }
    }

    void TcpServer::acceptConnection(int &new_socket)
    {
        new_socket = accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);
        if (new_socket < 0)
        {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " << ntohs(m_socketAddress.sin_port);
            exitWithError(ss.str());
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    // void TcpServer::sendResponse()
    // {
    //     long bytesSent;

    //     bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());

    //     if (bytesSent == m_serverMessage.size())
    //     {
    //         log("------ Server Response sent to client ------\n\n");
    //     }
    //     else
    //     {
    //         log("Error sending response to client");
    //     }
    // }

} // namespace http