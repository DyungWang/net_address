#pragma once

#include <string>
#include <cstdint>

class Address {
 public:
  enum IpType { UNASSIGNED, IPV4, IPV6 };

 public:
  Address();
  Address(const std::string& addrString);
  Address(const std::string& hostString, uint32_t port);
  Address(const uint32_t* host, uint32_t port);
  Address(uint32_t host, uint32_t port);

  Address(const Address& addr);
  Address(Address&& addr);

  Address& operator=(const Address& addr);
  Address& operator=(Address&& addr);

  friend bool operator< (const Address& lhs, const Address& rhs);
  friend bool operator==(const Address& lhs, const Address& rhs);

  std::string Host() const;
  uint32_t Port() const;
  IpType Type() const;

 private:
  void Parse(const std::string& addrString, std::string* hostString, std::string* portString);
  void Parse(const std::string& hostString, uint32_t* host, IpType* type);

 private:
  uint32_t host_[4];
  uint32_t port_;
  IpType type_;
};
