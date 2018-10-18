# zeromq-sql-reactor
A 0MQ/Lua/C server with a SQL-like interface

### Purpose:
  This project existed (discontinued) to satisfy the need for a federated sql-like messaging service with redis-like features such as expiring information. The purose of this project was to route millions of calls per minute in a VOIP company on an edge network with minimal latency and security control.
  
### Dependencies:
  This project depends on several C libraries and a linux-based operating environment (ideally Debian Stable)
  
  - sqlite3
  - sodium
  - czmq
  - json
  - lua5.2
  
  This project also depends on standard build tools:
  - build-essential
  - cmake
  
### Building:
  To compile and run this code run the following commands:

  ```
  cmake
  ./out/nexus_handle
  ```
  
#### Messages:
  This accepts standard 0MQ messages using messages in an envelope which could be secured with czmq sodium integration to authenticate and securely transport messages. Queries for information could be performed by specifying method handler and the parameters in two seperate messages within the same envelope. Responses would be delivered in JSON bia a returned message from the reactor.
  
### State:  
  This project was discontinued for a Swift based project which would be friendlier to newer developers but the fundamental idea exists here and was worth preserving. I like writing C especially with ZeroMQ interfaces and with Lua as a confgiuration/glueware component.
