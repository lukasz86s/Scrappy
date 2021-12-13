import socket
from SimpleServer.SimpleSer import EngineDriverConnect

wifi.connect()


import time
if __name__ == "__main__":
    print('hello')
    if wifi.net.isconnected():
        eng = EngineDriverConnect((wifi.net.ifconfig()[0], 6789))
        eng.test()