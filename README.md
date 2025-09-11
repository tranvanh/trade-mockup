# Client Application
- Sends orders to server

# Server Application
- Processes orders and delegades them to the market

## TradeLib - Shared
- Trading specific components
- Order book with order matching algorithm
- Stock market manager registering orders and trades

## UtilsLib - Static
- Observer pattern components
- Server and Client using unix sockets
- Thread safe queue
- Logger

### Note:
Purpose of this project is to practice various technical topics

### Depricated functionality

Will move these to client application
- Generates the stock market for simulation purposes
- Dumb investor reacting to trades
