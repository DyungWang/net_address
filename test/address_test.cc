#include <iostream>
#include <assert.h>

#include "address.h"

void TestConstruct1() {
  Address ipv4("127.0.0.1:1");
  assert(ipv4.Host() == "127.0.0.1");
  assert(ipv4.Port() == 1u);
  assert(ipv4.Type() == Address::IPV4);

  Address ipv6("[::]:1");
  assert(ipv6.Host() == "::");
  assert(ipv6.Port() == 1u);
  assert(ipv6.Type() == Address::IPV6);
}

void TestConstruct2() {
  Address ipv4("127.0.0.1", 1u);
  assert(ipv4.Host() == "127.0.0.1");
  assert(ipv4.Port() == 1u);
  assert(ipv4.Type() == Address::IPV4);

  Address ipv6("::", 1u);
  assert(ipv6.Host() == "::");
  assert(ipv6.Port() == 1u);
  assert(ipv6.Type() == Address::IPV6);
}

void TestConstruct3() {
  Address ipv4(1u, 1u);
  assert(ipv4.Host() == "1.0.0.0");
  assert(ipv4.Port() == 1u);
  assert(ipv4.Type() == Address::IPV4);

  uint32_t host[4];
  memset(host, 0, sizeof(uint32_t) * 4);
  Address ipv6(host, 1u);
  assert(ipv6.Host() == "::");
  assert(ipv6.Port() == 1u);
  assert(ipv6.Type() == Address::IPV6);
}

void TestCopyConstruct() {
  Address ipv4("0.0.0.1:1");
  Address copy1 = ipv4;
  Address copy2 = std::move(ipv4);
  
  assert(copy1.Host() == "0.0.0.1");
  assert(copy1.Port() == 1u);
  assert(copy1.Type() == Address::IPV4);

  assert(copy2.Host() == "0.0.0.1");
  assert(copy2.Port() == 1u);
  assert(copy2.Type() == Address::IPV4);
}

void TestCopyAssigment() {
  Address ipv4("0.0.0.1:1");
  Address copy1;
  Address copy2;
  copy1 = ipv4;
  copy2 = std::move(ipv4);

  assert(copy1.Host() == "0.0.0.1");
  assert(copy1.Port() == 1u);
  assert(copy1.Type() == Address::IPV4);

  assert(copy2.Host() == "0.0.0.1");
  assert(copy2.Port() == 1u);
  assert(copy2.Type() == Address::IPV4);
}

void TestOperatorLess() {
  Address addr1("127.0.0.1:1");
  Address addr2("127.0.0.2:2");
  Address addr3("[::]:1");
  
  assert(!(addr1 < addr1));
  assert(addr1 < addr2);
  assert(addr1 < addr3);
}

void TestOperatorEqual() {
  Address addr1("127.0.0.1:1");
  Address addr2("127.0.0.2:2");
  Address addr3("[::]:1");
  Address addr4("127.0.0.1:1");
  Address addr5("127.0.0.2:2");
  Address addr6("[::]:1");

  assert(addr1 == addr1);
  assert(addr2 == addr2);
  assert(addr3 == addr3);

  assert(addr1 == addr4);
  assert(addr2 == addr5);
  assert(addr3 == addr6);
}

int main(int argc, char** argv) {
  TestConstruct1();
  TestConstruct2();
  TestConstruct3();
  TestCopyConstruct();
  TestCopyAssigment();
  TestOperatorLess();
  TestOperatorEqual();
  return 0;
}
