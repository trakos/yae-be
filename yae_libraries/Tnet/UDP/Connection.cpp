#include <Tnet/UDP/Connection.h>

Tnet_UDP_Connection::Tnet_UDP_Connection(int _clientSocket, std::string _ip, unsigned int _port, sockaddr_in _peer) : Tnet_Connection(_clientSocket,_ip,_port,_peer)
{
}

Tnet_UDP_Connection::Tnet_UDP_Connection() : Tnet_Connection()
{
}

ssize_t Tnet_UDP_Connection::coreReceive(unsigned int sizeToReceive)
{
	int peerSize = sizeof(this->peer);
	return recvfrom(this->clientSocket, this->buffer+this->bufferPosition, sizeToReceive - this->bufferPosition, 0, (struct sockaddr *)&this->peer, (socklen_t*)&peerSize);
}

ssize_t Tnet_UDP_Connection::coreSend(const char* buffer, size_t len)
{
	return ::sendto(this->clientSocket, buffer, len, 0, (struct sockaddr *)&this->peer, sizeof(this->peer));
}
