#pragma once

/*
 * 封装socket相关操作
 */

#include <arpa/inet.h>
#include <cstdint>
#include <sys/socket.h>
#include "define/define_variable.h"

namespace muduo
{
namespace net
{
namespace sockets 
{

// Creates a non-blocking socket file descriptor, abort if any error. 
SDWORD createNonblockingOrDie(sa_family_t family);

SDWORD connect(SDWORD sockfd, const struct sockaddr* addr);
void bindOrDie(SDWORD sockfd, const struct sockaddr* addr);
void listenOrDie(SDWORD sockfd);
SDWORD accept(SDWORD sockfd, struct sockaddr_in6* addr);
ssize_t read(SDWORD sockfd, void *buf, size_t count);
ssize_t readv(SDWORD sockfd, const struct iovec *iov, SDWORD iovcnt);
ssize_t write(SDWORD sockfd, const void * buf, size_t count);
void close(SDWORD sockfd);
void shutdownWrite(SDWORD sockfd);

void toIpPort(CHAR* buf, size_t size, const struct sockaddr* addr);
void toIp(CHAR* buf, size_t size, const struct sockaddr* addr);

void fromIpPort(const CHAR* ip, uint16_t port, struct sockaddr_in* addr);
void fromIpPort(const CHAR* ip, uint16_t port, struct sockaddr_in6* addr);

SDWORD getSocketError(SDWORD sockfd);

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

struct sockaddr_in6 getLocalAddr(SDWORD sockfd);
struct sockaddr_in6 getPeerAddr(SDWORD sockfd);
bool isSelfConnect(SDWORD sockfd);

} // namespace sockets
} // namespace net
} // namespace muduo

