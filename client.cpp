/*
 * clang++ % -o simpleclient00 echo.capnp.c++ -std=c++20 -I/usr/local/include -L/usr/local/lib -lcapnp-rpc -lkj -lcapnp
 * clang++ echo.capnp.c++ simpleclient00.cpp -o clien00 -std=c++20 -I/usr/local/include -L/usr/local/lib `pkg-config --cflags --libs kj capnp kj-async capnp-rpc`
 */
#include "echo.capnp.h"
#include <kj/async-io.h>
#include <capnp/rpc-twoparty.h>
#include <iostream>

int main(int argc, const char* argv[])
{
  auto io = kj::setupAsyncIo();

  auto &waitScope = io.waitScope;
  kj::Network &network = io.provider->getNetwork();
  kj::Own<kj::NetworkAddress> addr =
  (argc != 2) ?
    network.parseAddress("localhost:12345").wait(io.waitScope) :
    network.parseAddress(argv[1]).wait(io.waitScope);

  kj::Own<kj::AsyncIoStream> conn = addr->connect().wait(waitScope);

  capnp::TwoPartyClient client(*conn);
  Echo::Client echo = client.bootstrap().castAs<Echo>();

  auto request = echo.echoRequest();
  request.setRequest("Hello!");

  std::cout << "sending request" << std::endl;
  auto promise = request.send();
  std::cout << "request sent" << std::endl;

  auto readPromise = promise.then([](auto response) { 
    std::cout << "received response : " << response.getResponse().cStr() << std::endl; 
    return response.getResponse(); 
  }); 

  auto response = readPromise.wait(waitScope);

  return 0;
}
