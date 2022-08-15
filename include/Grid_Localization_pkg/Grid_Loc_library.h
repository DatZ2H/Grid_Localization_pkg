#ifndef GRID_LOC_LIBRARY_H
#define GRID_LOC_LIBRARY_H
#include <iostream>
#include <future>
#include <functional>

#include <errno.h>
#include <string.h>

#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/signal.h>

#include <ros/ros.h>
#include <boost/thread.hpp>
#include <sstream>
#include "std_msgs/Bool.h"


using namespace std;
	
///Function Code
#define     READ_COILS        0x01
#define     READ_INPUT_BITS   0x02
#define     READ_REGS         0x03
#define     READ_INPUT_REGS   0x04
#define     WRITE_COIL        0x05
#define     WRITE_REG         0x06
#define     WRITE_COILS       0x0F
#define     WRITE_REGS        0x10

///Exception Codes

#define    EX_ILLEGAL_FUNCTION  0x01 // Function Code not Supported
#define    EX_ILLEGAL_ADDRESS   0x02 // Output Address not exists
#define    EX_ILLEGAL_VALUE     0x03 // Output Value not in Range
#define    EX_SERVER_FAILURE    0x04 // Slave Deive Fails to process request
#define    EX_ACKNOWLEDGE       0x05 // Service Need Long Time to Execute
#define    EX_SERVER_BUSY       0x06 // Server Was Unable to Accept MB Request PDU
#define    EX_NEGATIVE_ACK      0x07
#define    EX_MEM_PARITY_PROB   0x08
#define    EX_GATEWAY_PROBLEMP  0x0A // Gateway Path not Available
#define    EX_GATEWYA_PROBLEMF  0x0B // Target Device Failed to Response
#define    EX_BAD_DATA          0XFF // Bad Data lenght or Address

#define    BAD_CON              -1


#define    PARA_READ_INPUT_DATA_SET_01 0x0001
#define    PARA_READ_INPUT_DATA_SET_02 0x0001
#define    PARA_READ_INPUT_DATA_SET_03 0x0001
#define    PARA_READ_INPUT_DATA_SET_04 0x0001

#define    PARA_WRITE_OUTPUT_DATA_SET_01 0x000A
#define    PARA_WRITE_OUTPUT_DATA_SET_02 0x000A
#define    PARA_WRITE_OUTPUT_DATA_SET_03 0x000A
#define    PARA_WRITE_OUTPUT_DATA_SET_04 0x000A
#define    PARA_WRITE_OUTPUT_DATA_SET_05 0x000A

#define    PARA_CONFIGURE_INPUT_DATA_SET_01 0x0001
#define    PARA_CONFIGURE_INPUT_DATA_SET_02 0x0001
#define    PARA_CONFIGURE_INPUT_DATA_SET_03 0x0001
#define    PARA_CONFIGURE_INPUT_DATA_SET_04 0x0001

#define    PARA_CONFIGURE_INPUT_ENABLE_COS 0x0000

#define     CMD_MODE_POLL_REQUEST 0x00F1
#define     CMD_MODE_POLL_RESONSE 0x001F

#define     CMD_MODE_AUTO_REQUEST 0x00E1
#define     CMD_MODE_AUTO_RESONSE 0x001E
#define     CMD_MODE_AUTO_MSG_REQUEST 0x002E

#define     CMD_MODE_READ_WRITE_SET 0x00F2
#define     CMD_MODE_READ_WRITE_RESONSE 0x002F



#define    PARA_READ_INPUT_LENGTH 10
#define    PARA_WRITE_OUTUT_LENGTH 12
#define    PARA_CONFIGURE_INPUT_LENGTH 12


typedef enum FUNCTION
{
    FUNCTION_READ_INPUT = 0,
    FUNCTION_WRITE_OUTPUT = 1,
    FUNCTION_CONFIGURE = 2,
    FUNCTION_READ_AUTO = 3,

}FC;
typedef enum ERROR_CONNECT
{
  ERROR_CONNECT_DISCONNECT = 1,
  ERROR_CONNECT_SEND_DATA = 2,
  ERROR_CONNECT_RECV_DATA = 3,
  ERROR_CONNECT_TIME_OUT_SEND = 4,
  ERROR_CONNECT_TIME_OUT_RECV = 5,
  ERROR_CONNECT_FUNC_ERROR = 5,
  ERROR_CONNECT_DATA_ERROR = 2,

}EC;
class clientSock {
  public:
    clientSock(string host, unsigned int port);
    clientSock(string host, unsigned int port,bool automsg);
    clientSock();
    clientSock(int sock);
    ~clientSock();

    bool hasError();
    int connect();
    int disconnect();


    string host;
    unsigned int port;
    bool connected;


  protected:

  private:
  
    string HOST_IP;
    unsigned int PORT_IP;

    int enable_keepalive(int sock);
    void set_bad_con();
    void set_bad_input();

   // void print_data(uint16_t buffer[],int lengh);

    static const unsigned int buffSize = 1024;
    int sockfd;//establish connection to ID distribution server

    struct sockaddr_in servaddr;
    uint16_t recv[buffSize];
    struct hostent* server;



    


};

#endif // CLIENTSOCK_H