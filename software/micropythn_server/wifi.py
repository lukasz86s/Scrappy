import network
import socket
from time import sleep

class WiFi:
    def __init__(self, name, password):
        self.name = name
        self.password = password
        self.net = network.WLAN(network.STA_IF)
        self.net.active(True)

    def connect(self):
        """connecting to wifi"""
        if not self.findingNet(self.name):
            print(self.name, " is not in visable network")
            return 0
        if not self.net.isconnected():
            self.net.connect(self.name, self.password)
            while not self.net.isconnected():
                pass 
        print(self.net.ifconfig())

    def scan_(self):
        """return array of tuple with names and address of network"""
        return self.net.scan()

    def findingNet(self, name):
        """ checks if a network with that name exists"""
        for i in self.scan_():
            if name == i[0].decode() :
                return True 
        return False
    
def socket_listener():
    server_address = ('192.168.100.100', 6789)
    max_size = 2048
    print("start servera na esp32")
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(server_address)
    data, client = server.recvfrom(max_size)
    print('dane odebrane od kienta:', client, 'data:', data, '\n')
    server.sendto('witaj zjebie',client)
    server.close()