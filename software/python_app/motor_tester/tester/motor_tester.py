try:
    from tester.mainwindow import Ui_MainWindow as MainWin
except ModuleNotFoundError:
    from mainwindow import Ui_MainWindow as MainWin
from PyQt6.QtWidgets import QApplication
from PyQt6.QtWidgets import QMainWindow
from PyQt6.QtGui import QStandardItemModel, QStandardItem
from serial import Serial, SerialException
from serial.tools.list_ports import comports
import keyboard
from threading import Thread, Event
import time
import socket
import sys



BAUD_RATE = ("2400",
                   "4800",
                   "9600",
                   "14400",
                   "19200",
                   "28800",
                   "38400",
                   "57600",
                   "76800",
                   "115200",
                   "230400")

class MyMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.isclosed = False

    def add_threads_to_close(self, *args):
        self.threads = args

    def closeEvent(self, a0) -> None:
        super().closeEvent(a0)
        self.isclosed = True
        for thread in self.threads:
            thread.join()
                    

class MainWindow:

    def __init__(self):
        self.main_win = MyMainWindow()
        self.ui = MainWin()
        self.ui.setupUi(self.main_win)
        self.listView_model = QStandardItemModel()
        self.serialPort = None
        self.read_port_thread = Thread(target=self.read_port)
        self.keyboard_thread = Thread(target=self.keyboard_remote)
        self.sending_event = Event()
        self.sending_thread = Thread(target=self.send_raw_frame_on_event, args=(self.sending_event,))
        self.sending_thread.daemon = True
        self.main_win.add_threads_to_close(self.read_port_thread)#, self.keyboard_thread)
        #mapping engines directions to the pairs of keys
        self.control_keys = {'engine1': ['w', 's'],
                             'engine2': ['a', 'd'],
                             'engine3': ['up', 'down'],
                             'engine4': ['left', 'right']}
        # mapping engines to the number
        self.mapping_keys_engines_nr = {'engine1': '0',
                                        'engine2': '1',
                                        'engine3': '2',
                                        'engine4': '3'}

    def serial_connect(self, byte_size=8, stop_bits=1):
        port = self.ui.port_comboBox.currentText().strip()
        baud_rate = int(self.ui.baud_rate_comboBox.currentText())
        # check if port is not open or not exist , then create and open
        if self.serialPort is None or not self.serialPort.is_open:
            try:
                self.serialPort = Serial(port, baud_rate)
                #view info on view list screen
                self.listView_model.appendRow(QStandardItem("connected:"))
                #change text on button
                self.ui.connect_com_button.setText("Disconnect")
            except SerialException:
                # view  exception in widget list screen
                self.listView_model.appendRow(QStandardItem("error: connected problems"))
        else:
            try:
                self.serialPort.close()
                # view  info in widget list screen
                self.listView_model.appendRow(QStandardItem("disconnected:"))
                # change text on button
                self.ui.connect_com_button.setText("Connect")
            except SerialException:
                # view  exception in widget list screen
                self.listView_model.appendRow(QStandardItem("error: disconnect problems"))

    def tcp_connect(self, message ):
        """" create clien tcp, and connect to the addres """
        #get ip and port
        ip = self.ui.ip_lineEdit.text()
        port = self.ui.port_lineEdit.text()
        MAX_DATA = 512

        #check that it is not empty
        if len(ip) == 0:
            self.listView_model.appendRow(QStandardItem("nothig in ip line !!!"))
            return
        if len(port) == 0:
            self.listView_model.appendRow(QStandardItem("nothig in port line !!!"))
            return
        try:
            port = int(port)
        except ValueError:
            self.listView_model.appendRow(QStandardItem("Port mast be digit !!!"))

        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            client.connect((ip, port))
            client.sendall(message)
            self.listView_model.appendRow(QStandardItem(client.recv(MAX_DATA).decode()))
            client.sendall('END_SERVER'.encode())
            client.close()
        except (socket.gaierror, TimeoutError) as e:
            self.listView_model.appendRow(QStandardItem(str(e)))



    def read_port(self):
        while True:
            #todo: think about change thread to Q_thread
            time.sleep(0.1)
            if self.serialPort and self.serialPort.is_open:
                serial_string = ''
                # if data , read it
                while self.serialPort.in_waiting > 0:
                    serial_string += self.serialPort.read().decode('Ascii')
                # if string len is 0 do nothing
                if len(serial_string):
                    self.listView_model.appendRow(QStandardItem(serial_string))
            if self.main_win.isclosed:
                if self.serialPort:
                    self.serialPort.close()
                break

    #TODO:connect start this function with new button (like keyboard move )or checkbutton
    def keyboard_remote(self):
        """function sends a frame that corresponds to the assigned keys on keyboard.
            keys as assigned to the movement of the motors """
        while True:
            time.sleep(0.06)
            if self.serialPort and self.serialPort.is_open:
                #TODO:maybe ping connection every 1s or cyclical sending information about engine movement
                # keys are connected in pairs, to the opposite side
                # forward and backward keys are in  control_keys['engine1'][0] and controls_keys['engine1'][1]
                for i in self.control_keys.keys():
                    #check if key is pressed or released
                    status_f = self.key_status(self.control_keys[i][0])
                    status_b = self.key_status(self.control_keys[i][1])
                    #depeding of status , create appropriate frame
                    frame = None

                    if status_f == 'pressed':
                        frame = ':42' + self.mapping_keys_engines_nr[i] + '0' + 'T'
                    elif status_b == 'pressed':
                        frame = ':42' + self.mapping_keys_engines_nr[i] + '1' + 'T'
                    elif status_f == 'release' or status_b == 'release':
                        frame = ':42' + self.mapping_keys_engines_nr[i] + '0' + 'F'
                    if frame:
                        print(frame)
                        raw_frame = frame.encode()
                        self.send_raw_frame(raw_frame)
            # ending thread
            if self.main_win.isclosed:
                break

    def key_status(self, key):
        """function retrun keyboard key status  """
        if not hasattr(self, 'active_keys'):
            # todo: move self.active_keys to the __init__
            self.active_keys = []
        if keyboard.is_pressed(key):
            if key not in self.active_keys:
                self.active_keys.append(key)
                return 'pressed'
            return 'not_pressed'
        elif key in self.active_keys:
            self.active_keys.remove(key)
            return 'release'
        return 'not_press'

    def refresh_com_list(self):
        self.ui.port_comboBox.clear()
        self.ui.port_comboBox.addItems([str(i)[0:5].strip() for i in comports()])

    def move_button(self):
        engine = self.ui.engine_comboBox.currentText()
        direction = self.ui.direction_comboBox.currentText()
        direction = '1' if direction == 'left' else '0'
        steps = self.ui.steps_lineEdit.text()
        if len(steps) == 0:
            self.listView_model.appendRow(QStandardItem('step tex box is empty!!!'))
            return 
        # parse string , if not hex convert and remove starting '0x'
        if steps[0] != 'x':
            steps = str(hex(int(steps)))[2:]
        else:
            steps = steps[1:]
        # if steps is to short for frame requirements
        if 4 - len(steps):
            count = 4 - len(steps)
            zeros = count * '0'
            steps = zeros + steps
        # get len of useful data
        data_len = len(engine+direction+steps)
        #todo: add comboBox or text_line for 'function' byte
        frame = ':'+str(data_len)+'1'+engine+direction+steps
        raw_frame = frame.encode()
        self.raw_frame = raw_frame
        self.sending_event.set()
        #self.send_raw_frame(raw_frame)

    def send_raw_frame(self, raw_frame):
        """Send frame by com port or Wlan """
        if self.ui.com_port_radioButton.isChecked():
            # if serialPort exist and is open
            if self.serialPort and self.serialPort.is_open:
                self.serialPort.write(raw_frame)
        else:
            self.thread = Thread(target=self.tcp_connect, args=[raw_frame])
            self.thread.daemon = True
            self.thread.start()
            
    def send_raw_frame_on_event(self, event):
        """Send frame by com port or Wlan """
        while True:
            event.wait()
            if self.ui.com_port_radioButton.isChecked():
                # if serialPort exist and is open
                if self.serialPort and self.serialPort.is_open:
                    self.serialPort.write(self.raw_frame)
            else:
                self.tcp_connect(self.raw_frame)
            event.clear()

            

    def rb_com_ports(self):
        """Enable and disable widgets associated with com radiobuton"""
        self.ui.connect_com_button.setEnabled(True)
        self.ui.refreshe_button.setEnabled(True)
        self.ui.port_comboBox.setEnabled(True)
        self.ui.baud_rate_comboBox.setEnabled(True)

        self.ui.bind_ip_button.setEnabled(False)
        self.ui.ip_lineEdit.setEnabled(False)
        self.ui.port_lineEdit.setEnabled(False)

    def rb_tcp_ip(self):
        """Enable and disable widgets associated with ip radiobuton"""
        self.ui.bind_ip_button.setEnabled(True)
        self.ui.ip_lineEdit.setEnabled(True)
        self.ui.port_lineEdit.setEnabled(True)   

        self.ui.connect_com_button.setEnabled(False)
        self.ui.refreshe_button.setEnabled(False)
        self.ui.port_comboBox.setEnabled(False)
        self.ui.baud_rate_comboBox.setEnabled(False)
        

    def ui_init(self):
        #add com list
        self.refresh_com_list()
        # add baud rate
        self.ui.baud_rate_comboBox.addItems(BAUD_RATE)
        # engines numbers
        [self.ui.engine_comboBox.addItem(str(i)) for i in range(5)]
        # engine direction
        self.ui.direction_comboBox.addItems(["left", "right"])
        # connect refresh function and button 1
        self.ui.refreshe_button.clicked.connect(self.refresh_com_list)
        # connect button2 and serialConnect  function
        self.ui.connect_com_button.clicked.connect(self.serial_connect)
        # connect button_3 to function
        self.ui.move_button.clicked.connect(self.move_button)
        #prepere listView to work with standards models
        self.ui.listView.setModel(self.listView_model)
        #connect radio butons to the functions
        self.ui.com_port_radioButton.toggled.connect(self.rb_com_ports)
        self.ui.tcp_radioButton.toggled.connect(self.rb_tcp_ip)

    def show(self):
        self.main_win.show()


def startApp():
    app = QApplication(sys.argv)
    main_win = MainWindow()
    main_win.ui_init()
    main_win.read_port_thread.start()
    main_win.sending_thread.start()
    #TODO: add sender with ip and com handler to keyboard remot function. activate below line with thread 
    #main_win.keyboard_thread.start()
    main_win.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    startApp()
