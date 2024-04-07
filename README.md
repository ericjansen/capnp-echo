# capnp-echo
Simple Echo Text using Capnproto in C++

## Compilation
Either which compiler you are using `clang++` or `g++` 
then run 
```
capnp compile -oc++ echo.capnp
g++ echo.capnp.c++ client.cpp -o client -std=c++20 `pkg-config --cflags --libs capnp kj kj-async capnp-rpc`
g++ echo.capnp.c++ server.cpp -o server -std=c++20 `pkg-config --cflags --libs capnp kj kj-async capnp-rpc`
```

## Usage
`./server`
`./client`
