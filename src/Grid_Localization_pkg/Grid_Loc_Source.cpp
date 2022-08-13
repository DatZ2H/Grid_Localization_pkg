#include "Grid_Localization_pkg/Grid_Loc_library.h"

clientSock::clientSock(string host, unsigned int port)
{
    int t = 0;
    HOST_IP = host;
    PORT_IP = port;
    connect();
}

clientSock::clientSock(string host, unsigned int port, bool automsg)
{
    int t = 0;
    HOST_IP = host;
    PORT_IP = port;
    connect();
    //  tcp_auto_read(1);
}

clientSock::clientSock()
{
    connected = false;
}

clientSock::clientSock(int sock)
{
    sockfd = sock;
    connected = true;
}

clientSock::~clientSock()
{
    // disconnect();
}

int clientSock::connect()
{
    cout << "connecting!!" << endl;
    clientSock::host = HOST_IP;
    clientSock::port = PORT_IP;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    server = gethostbyname(host.data());
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(port);

    if (connected)
        disconnect();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // struct timeval tv;
    // tv.tv_sec = 10;//5 Secs Timeout
    // tv.tv_usec = 0;//Not init'ing this can cause strange errors
    // setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(struct timeval));
    // setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

    enable_keepalive(sockfd);

    for (size_t i = 0; i < 10; i++)
    { // try to connect 10 times
        if (::connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
            cout << "Error on connecting: " << errno << "  " << strerror(errno) << endl;
        else
        {
            connected = true;
            cout << "connected!!!" << endl;
            return 0;
        }
    }

    connected = false;
    return 1;
}

bool clientSock::hasError()
{
    if (sockfd == -1)
        return true;

    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);

    if (retval != 0 || error != 0)
        return true;
    else
        return false;
}

int clientSock::enable_keepalive(int sock)
{
    int yes = 1;

    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
        return -1;

    int idle = 1;

    if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) == -1)
        return -1;

    int interval = 1;

    if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) == -1)
        return -1;

    int maxpkt = 10;

    if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(int)) == -1)
        return -1;

    return 0;
}

int clientSock::disconnect()
{
    if (!connected)
        return -1;

    close(sockfd);
    connected = false;

    return 0;
}























void clientSock::set_bad_con()
{
    ROS_ERROR("DISCONNECT FLEXISOFT");
    connected = false;
}
void clientSock::set_bad_input()
{
}
