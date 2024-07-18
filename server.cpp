/*
 *clang++ % echo.capnp.c++ -o simpleserver00 -I/usr/local/include -L/usr/local/lib -std=c++20 -lcapnp-rpc -lkj -lcapnp
 */
#include <capnp/rpc-twoparty.h>
#include <kj/async-io.h>
#include <iostream>

#include "echo.capnp.h"

class EchoImpl final : public Echo::Server
{
public:
  kj::Promise<void> echo(EchoContext context) override
  {
    auto message = context.getParams().getRequest();
    std::cout << "Received message: " << message.cStr() << std::endl;
    context.getResults().setResponse("Sending back");//message.cStr());
    return kj::READY_NOW;
  }
};

int main(int argc,const char* argv[])
{
  auto io = kj::setupAsyncIo();
  kj::Network &network = io.provider->getNetwork();
  kj::Own<kj::NetworkAddress> addr = (argc != 2) ?
    network.parseAddress("localhost:12345").wait(io.waitScope) :
    network.parseAddress(argv[1]).wait(io.waitScope);

  kj::Own<kj::ConnectionReceiver> listener = addr->listen();

  capnp::TwoPartyServer server(kj::heap<EchoImpl>());

  uint port = listener->getPort();
  std::cout << "Listening on port " << port << std::endl;

  server.listen(*listener).wait(io.waitScope);

  return 0;
}
