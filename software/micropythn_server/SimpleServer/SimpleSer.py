from machine import UART
from SimpleServer.Server import ServerTCP


class EngineDriverConnect():
    def __init__(self, server_addr, uart_nr=1, baudrate=115200, tx=17, rx=16):
        self.uart = UART(2, baudrate=115200,tx=17, rx=16)
        self.server = ServerTCP(server_addr)
    
    def test(self):
        self.server.start(self.send_rec)
        
    def send_rec(self, data_in):
        self.uart.write(data_in)
        return self.uart.read() 