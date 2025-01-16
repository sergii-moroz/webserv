# webserv
webserv 42 Core

## What Is Done
 - TCP Server
	- create a server on localhost with pre-defined port `12345` and backlog `10`
	- wait on select() and then process ready to read/write sockets
	- non-blocking
	- for any client's request send simple responce back

## To Do
- TCP Server
	- error handling
	- chancked message
- config file
	- design (nginx inspired)
	- parse / validate / design data structure to save
- http server
	- take request as string / char
	- parse / validate request
	- process reques
	- return responce as string
- logging
	- ugly looking std::cout (current state)
	- design a class Logger
	- logType: DEBUG < INFO <  WARNING < ERROR
	- static void log(int logType, std::string/std::ostream)
	- should output messages that logType >= globally defined status


## How To Use

```bash
make
./webserv
```

**_NOTE:_** config file currently is not required

if you will get following output, it means that default server on localhost was created and listen to port `12345`.

```
Configuration file isn't provided.
Loading default settings ...
Webserv: Default constructor called
INFO: creating a socket...
INFO: Server :::12345
INFO: Ready for client connections ...
Waiting on select()...
```

## How To Test

### using terminal
- open another terminal
- type `nc localhost 12345` + `Enter`
- type `your message` + `Enter`

```bash
HTTP/1.1 200 OK
Content-Length: 14
Content-Type: text/html

Hello World!
```
**_NOTE:_** you could use alternative command `telnet localhost 12345` too.

### using browser

In addres line type `localhost:12345`

## Time Line

| Date | Task | intra name |
| --- | --- | --- |
| 2025-01-16 | TCP Server | smoroz |
| xxxx-xx-xx | Some other task | olanokhi |
