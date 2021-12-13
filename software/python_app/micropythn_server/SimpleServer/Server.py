import _thread
try:
    import usocket as socket
except Exception:
    import socket
    
class ServerTCP():
    #add finding ip from board , first place in get ifconfig (net)
    def __init__(self, address, max_size=5000):
        self.address = address
        self.max_size = max_size
        self.CreateServer()
    
    def eptyfunc(self):
        pass
    
    def client_handler(self, client, addr, func):
        data = b''
        recdata = b''
        
        callback_f = self.eptyfunc  
        if func != None:
            callback_f = lambda :func(data)
            
        while True:
            print("waiting for data")
            try:
                data = client.recv(self.max_size)
                if data.decode('utf-8') == 'END_SERVER':
                    break
            
                recdata = callback_f()
                
                if recdata == None:
                    recdata = b'I don`t recive data'
                client.sendall(recdata)
                
            except OSError:
                print("OS error , maby disconnected")
                break
                
        client.close()
        

    
    def start(self, callback=None):


             
        self.server.listen(1)
        print("Server start")

        while True:
            print("waiting for client")
            client, addr = self.server.accept()
            self.client_handler(client, addr, callback)

                 
    def DestroyServer(self):
        self.server.close()
    
    def CreateServer(self):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind(self.address)
                     
                    
        