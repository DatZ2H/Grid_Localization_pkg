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

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose2D.h>

#include <ros/ros.h>
#include <boost/thread.hpp>
#include <sstream>
#include "std_msgs/Bool.h"


using namespace std;

///Function Code
#define READ_COILS        0x01
#define READ_INPUT_BITS   0x02
#define READ_REGS         0x03
#define READ_INPUT_REGS   0x04
#define WRITE_COIL        0x05
#define WRITE_REG         0x06
#define WRITE_COILS       0x0F
#define WRITE_REGS        0x10

///Exception Codes

#define EX_ILLEGAL_FUNCTION  0x01 // Function Code not Supported
#define EX_ILLEGAL_ADDRESS   0x02 // Output Address not exists
#define EX_ILLEGAL_VALUE     0x03 // Output Value not in Range
#define EX_SERVER_FAILURE    0x04 // Slave Deive Fails to process request
#define EX_ACKNOWLEDGE       0x05 // Service Need Long Time to Execute
#define EX_SERVER_BUSY       0x06 // Server Was Unable to Accept MB Request PDU
#define EX_NEGATIVE_ACK      0x07
#define EX_MEM_PARITY_PROB   0x08
#define EX_GATEWAY_PROBLEMP  0x0A // Gateway Path not Available
#define EX_GATEWYA_PROBLEMF  0x0B // Target Device Failed to Response
#define EX_BAD_DATA          0XFF // Bad Data lenght or Address

#define BAD_CON              -1

#define PARA_START_BIT 0x02 // <STX>
#define PARA_END_BIT   0x03 // <ETX>
#define PARA_HEARTHEART_BIT 0x486561727442656174

#define PARA_CODE_LENGTH 12
#define PARA_POS_X_LENGTH 5
#define PARA_POS_Y_LENGTH 5
#define PARA_XMCL_LENGTH 4
#define PARA_YMCL_LENGTH 4
#define PARA_AN2_LENGTH 5
#define PARA_FAMS_LENGTH 8

#define PARA_HEARTHEART_LENGTH 11

#define PARA_RESOLUTION_LENGTH 1000 // Resolution mm than 200mm

#define POSE_POS_CODE 1
#define POSE_POS_X  3
#define POSE_POS_Y  8
#define POSE_XMCL 14
#define POSE_YMCL 19
#define POSE_AN2  24
#define POSE_FAMS 30



#define PARA_HEARTHEART_LENGTH 11

#define PARA_READ_INPUT_LENGTH 10
#define PARA_WRITE_OUTUT_LENGTH 12
#define PARA_CONFIGURE_INPUT_LENGTH 12
#define PARA_READ_MCL_LENGTH 39



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
typedef enum STATUS
{
  STATUS_GRID_RECV_PING = 1,
  STATUS_GRID_RECV_DATA = 2,

}ST;
struct code_pose {
  char rowcode;
  char colcode;
};
struct label_pose {
  int64_t XMCL;
  int64_t YMCL;
  int32_t ANS;
};
struct label_code {
  code_pose code;
  geometry_msgs::Pose2D content;
  label_pose pose;
  int FAMS;
};
struct LLSResult{
  uint64_t telegram_count;
  uint64_t timestamp;
  int64_t x;
  int64_t y;
  int32_t heading;
  uint8_t loc_status;
  uint8_t map_match_status;

  uint32_t sync_timestamp_sec;
  uint32_t sync_timestamp_nsec;
  uint32_t sync_timestamp_valid;
};


class clientSock {
public:
  clientSock(string host, unsigned int port);
  clientSock(string host, unsigned int port, bool automsg);
  clientSock();
  clientSock(int sock);
  ~clientSock();

  bool hasError();
  int connect();
  int disconnect();

  int tcp_read();
  int tcp_write();
  string convertToString(char* chararray);
  int convertToInt(char* chararray, int pos, int len);
  label_pose getLocResult();
  


  string host;
  unsigned int port;
  bool connected;
  label_code Qrcode;
  label_pose LocResult;
  LLSResult LlsResult;
  int rate_;
  int call_state = 5;


protected:

private:

  string HOST_IP;
  unsigned int PORT_IP;

  int enable_keepalive(int sock);
  size_t getContent(char* buffer);
  size_t getPose(char* buffer);
  size_t getCode(char* buffer);
  size_t getFAMS(char* buffer);


  size_t tcp_send(char* to_send, int length);
  size_t tcp_receive(char* buffer, int length);
  void set_bad_con();
  void set_bad_input();

  // void print_data(uint16_t buffer[],int lengh);

  static const unsigned int buffSize = 1024;
  char Buffer[buffSize] = { 0 };
  int sockfd;//establish connection to ID distribution server

  struct sockaddr_in servaddr;
  struct hostent* server;
};

#endif // CLIENTSOCK_H