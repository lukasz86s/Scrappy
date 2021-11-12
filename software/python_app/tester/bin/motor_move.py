from Packages.mainwindow import Ui_MainWindow as MainWin
from PyQt6.QtWidgets import QApplication
from PyQt6.QtWidgets import QMainWindow
from PyQt6.QtGui import QStandardItemModel, QStandardItem
from serial import Serial, SerialException
from serial.tools.list_ports import comports
import keyboard
from threading import Thread
import time
import sys

BAUD_RATE_TABLE = ("2400",
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

    def closeEvent(self, a0) -> None:
        super().closeEvent(a0)
        self.isclosed = True


class MainWindow:
    def __init__(self):
        self.main_win = MyMainWindow()
        self.ui = MainWin()
        self.ui.setupUi(self.main_win)
        self.model = QStandardItemModel()
        self.serialPort = None
        self.read_port_thread = Thread(target=self.read_port)
        self.keyboard_thread = Thread(target=self.keyboard_remote)
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
        port = self.ui.comboBox.currentText().strip()
        baud_rate = int(self.ui.comboBox_2.currentText())
        # check if port is not open or not exist , then create and open
        if self.serialPort is None or not self.serialPort.is_open:
            try:
                self.serialPort = Serial(port, baud_rate)
                #view info on view list screen
                self.model.appendRow(QStandardItem("connected:"))
                #change text on button
                self.ui.pushButton_2.setText("Disconnect")
            except SerialException:
                # view  exception in widget list screen
                self.model.appendRow(QStandardItem("error: connected problems"))
        else:
            try:
                self.serialPort.close()
                # view  info in widget list screen
                self.model.appendRow(QStandardItem("disconnected:"))
                # change text on button
                self.ui.pushButton_2.setText("Connect")
            except SerialException:
                # view  exception in widget list screen
                self.model.appendRow(QStandardItem("error: disconnect problems"))

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
                    self.model.appendRow(QStandardItem(serial_string))
            if self.main_win.isclosed:
                if self.serialPort:
                    self.serialPort.close()
                break

    def keyboard_remote(self):
        while True:
            time.sleep(0.06)
            if self.serialPort and self.serialPort.is_open:
                #todo:maybe ping connection every 1s or cyclical sending information about engine movement
                # keys are connected in pairs, to the opposite side
                # forward and backward keys ar in  control_keys['engine1'][0] and controls_keys['engine1'][1]
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
                        self.serialPort.write(raw_frame)
            # ending thread
            if self.main_win.isclosed:
                break

    # return events if key was pressed or release
    def key_status(self, key):
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
        self.ui.comboBox.clear()
        self.ui.comboBox.addItems([str(i)[0:5].strip() for i in comports()])

    def move_button(self):
        engine = self.ui.comboBox_3.currentText()
        direction = self.ui.comboBox_4.currentText()
        direction = '1' if direction == 'left' else '0'
        steps = self.ui.lineEdit.text()
        # parse string , if not hex convert and remove starting '0x'
        if steps[0] != 'x':
            steps = str(hex(int(steps)))[2:]
        else:
            steps = steps[1:]
        # if steps is to short for frame requirements
        if 4 - len(steps):
            count = 4 - len(steps)
            zeros = ''
            for i in range(count):
                zeros += '0'
            steps = zeros + steps
        # get len of useful data
        data_len = len(engine+direction+steps)
        #todo: add comboBox or text_line for 'function' byte
        frame = ':'+str(data_len)+'1'+engine+direction+steps
        raw_frame = frame.encode()
        # if serialPort exist and is open
        if self.serialPort and self.serialPort.is_open:
            self.serialPort.write(raw_frame)

    def ui_init(self):
        #add com list
        self.refresh_com_list()
        # add baud rate
        self.ui.comboBox_2.addItems(BAUD_RATE_TABLE)
        # engines numbers
        [self.ui.comboBox_3.addItem(str(i)) for i in range(5)]
        # engine direction
        self.ui.comboBox_4.addItems(["left", "right"])
        # connect refresh function and button 1
        self.ui.pushButton.clicked.connect(self.refresh_com_list)
        # connect button2 and serialConnect  function
        self.ui.pushButton_2.clicked.connect(self.serial_connect)
        # connect button_3 to function
        self.ui.pushButton_3.clicked.connect(self.move_button)
        #prepere listView to work with standards models
        self.ui.listView.setModel(self.model)

    def show(self):
        self.main_win.show()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_win = MainWindow()
    main_win.ui_init()
    main_win.read_port_thread.start()
    main_win.keyboard_thread.start()
    main_win.show()
    sys.exit(app.exec())
