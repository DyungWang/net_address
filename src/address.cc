#include "address.h"

#include <arpa/inet.h>
#include "address_exception.h"

#define MAX_ADDRESS_LEN 256

Address::Address(): port_(0), type_(UNASSIGNED) {}

Address::Address(const std::string& addrString) {
  std::string hostString;
  std::string portString;
  Parse(addrString, &hostString, &portString);
  Parse(hostString, host_, &type_);
  port_ = std::stoi(portString.data());
}

Address::Address(const std::string& hostString, uint32_t port): port_(port) {
  Parse(hostString, host_, &type_);
}

Address::Address(const uint32_t* host, uint32_t port): port_(port), type_(IPV6) {
  memcpy(host_, host, sizeof(host_));
}

Address::Address(uint32_t host, uint32_t port): port_(port), type_(IPV4) {
  host_[0] = host;
}

Address::Address(const Address& addr): port_(addr.port_), type_(addr.type_) {
  memcpy(host_, addr.host_, sizeof(host_));
}

Address::Address(Address&& addr): port_(addr.port_), type_(addr.type_) {
  memcpy(host_, addr.host_, sizeof(host_));
}

Address& Address::operator=(const Address& addr) {
  memcpy(host_, addr.host_, sizeof(host_));
  port_ = addr.port_;
  type_ = addr.type_;
  return *this;
}

Address& Address::operator=(Address&& addr) {
  memcpy(host_, addr.host_, sizeof(host_));
  port_ = addr.port_;
  type_ = addr.type_;
  return *this;
}

void Address::Parse(const std::string& addrString, std::string* hostString, std::string* portString) {
  auto pos = addrString.rfind(':');
  if (pos == std::string::npos)
    throw AddressException("invalid address, " + addrString);
  *hostString = addrString.substr(0, pos);
  *portString = addrString.substr(pos+1);
}

void Address::Parse(const std::string& hostString, uint32_t* host, IpType* type) {
  std::string nHostString = hostString;
  if (hostString[0] == '[') 
    nHostString = hostString.substr(1, hostString.size()-2);
  
  int32_t res = 0;
  if (nHostString.find(':') != std::string::npos) {
    *type = IPV6;
    res = inet_pton(AF_INET6, nHostString.data(), host);
  } else {
    *type = IPV4;
    res = inet_pton(AF_INET, nHostString.data(), host);
  }
  if (res <= 0) throw AddressException("invalid address, " + hostString);
} 

std::string Address::Host() const {
  auto afNet = type_ == IPV4 ? AF_INET : AF_INET6;
  char hostString[MAX_ADDRESS_LEN];
  if (nullptr == inet_ntop(afNet, host_, hostString, MAX_ADDRESS_LEN))
    throw AddressException("conver host failed");
  return hostString;
}

uint32_t Address::Port() const {
  return port_;
}

Address::IpType Address::Type() const {
  return type_;
}

// type -> host -> port
bool operator< (const Address& lhs, const Address& rhs) {
  if (lhs.type_ != rhs.type_)
    return lhs.type_ < rhs.type_;
  int32_t ret = memcmp(lhs.host_, rhs.host_, sizeof(lhs.host_));
  if (ret != 0)
    return ret < 0;
  return lhs.port_ < rhs.port_;
}

bool operator==(const Address& lhs, const Address& rhs) {
  if (lhs.type_ != rhs.type_)
    return false;
  if (lhs.port_ != rhs.port_)
    return false;
  return memcmp(lhs.host_, rhs.host_, sizeof(lhs.port_)) == 0;
}
