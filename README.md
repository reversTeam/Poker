# Poker
It's a server for playing poker

## Dependancy
ncursew
mysql




## Run
Command to run Serveur Poker
```bash
./server [port]
```

## Client
```
CONNECT [USER] [PASSWORD]
GET_GAMES
[SERVER RESPOND]
SELECT_GAME [GAME NAME]
GET_MODES
[SERVER RESPOND]
SELECT_MODE [MODE NAME]
GET_BLINDS
[MIN MAX] [MIN MAX]
SELECT_BLIND [MIN] [MAX] || CREATE_BLIND [MIN]
CHANGE_ROLE [PLAYER|SPECTATOR]
```
