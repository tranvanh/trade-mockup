# Client Application
- Sends orders to server in json format
- Option to simulate order creations for stress testing

# Server Application
- Processes orders and delegades them to the market
- Serves as a communication layer between the market and the client

## TradeLib - Shared
- Trading specific components
- Order book with order matching algorithm. Matching highest buyer with lowest seller
- Stock market manager registering orders and trades

## UtilsLib - Static
- Observer pattern components
- Server and Client using unix sockets. Client sends message size first and the content later, unsuring content consistency on receiving end.
- Thread safe queue
- Logger

Client and Server applications share common utility libraries providing various useful functionality. Thus the goal is to have UtilsLabs as generic as possible, which could be reused for any other future projects

## How to

In order to run Client or Server, you must have UtilsLib and TradeLib installed.

### Installing

From the root directory call `cmake -S ./Libs -B ./Libs/build`, `cmake --build ./Libs/build` and lastly `cmake --install ./Libs/build`. You can specify your own installation prefix. Note that in Client application and Server application, both have my path which is a working state and will be further improve. If you're installing to the system include. Simply remove CMAKE_PREFIX_PATH or specify yours.

To build and run Client application call `cmake -S ./ClientApplication -B ./ClientApplication/build`, `cmake --build ./ClientApplication/build`. The path to executable is `./ClientApplication/build/src/clientApp <userId> [simulate]`.
 `<userId>` is just an identification, could be any positive number, `simulate` is optional argument to simulate the order creation, id's are randomly generated.
 
 Same build process applies to Server application, except it does not have arguments.

### Note:
Purpose of this project is to practice various technical topics as thread safety, networking, design patterns, heavy load handling, optimization.

### TODO
- [x] Model generating buy/sell THREAD - give certain delay of buy/sell generation
- [x] Mock database storing the information THREAD SAFE
- [x] Matching trade/sell
- [ ] Yield list of latest 100 trades happened around the given timestamp
- [ ] Iterator to navigate the trades chronologicaly
- [x] Create server-client connection
- [x] Add commands and the input handling for client side
- [ ] Error handling of connection/communication/parsing issues
- [x] Heavy load handling, used clientApp with simulate currently crashes as the buffer communication buffer gets overwhelmed
- [x] Handle multiple clients and handle reconnection
- [ ] Handle failed connection with try to reconnect
- [ ] Handle arguments properly, handle user inputs properly too
- [x] Logger
- [ ] Add thread scheduler to application to limit worker resources
- [ ] Restructure UtilsLib folders
