from PyQt5 import QtWidgets
import time, threading, sys
import serial_test

sys.path.append("../")

from py_ui.Ui_demo import Ui_Control

class DemoWidget(QtWidgets.QWidget):
    point_list = [
        # [0,   0], 
        # [30,  0],
        # [60,  0],
        # [90,  0],
        # [120, 0], 
        # [150, 0], 
        # [180, 0], 
        # [210, 0], 
        # [240, 0], 
        # [270, 0], 
        # [300, 0], 
        # [330, 0],
        # [360, 0],
    ]
    def __init__(self, ui : Ui_Control):
        super().__init__()
        self.ui = ui
        # 
        self.is_reading = False
        self.dev_read : threading.Thread = None
        self.dev_read_stop = True
        self.connect_dev : serial_test.SerialDevice = None
        # 
        self.ui.setupUi(self)
        self.setUiInit()
    
    def setUiInit(self):
        self.ui.btn_reflash.clicked.connect(self.dev_refresh_evt)
        self.ui.btn_connect.clicked.connect(self.dev_connect_evt)
        self.ui.btn_reset_val.clicked.connect(self.dev_reset_val_evt)
        self.ui.btn_set_val.clicked.connect(self.dev_set_val_evt)
    
    def dev_set_val_evt(self): 
        print("dev_set_val_evt...")
        sdb = bytearray(3)
        sdb[0] = 0x00
        sdb[1] = 0x00
        sdb[2] = 0x3a
        # send_dat = self.textEdit.toPlainText()
        # print(send_dat)
        start_ang = self.ui.startAngleSlider.value()
        end_ang = self.ui.endAngleSlider.value()
        step = self.ui.stepSlider.value()
        if step > end_ang - start_ang:
            print("step > end_ang - start_ang")
            return
        if start_ang > end_ang: 
            start_ang, end_ang = end_ang, start_ang
            # return
        mod = (end_ang - start_ang) % step
        if mod != 0: 
            end_ang += (step - mod)
        # 
        sd_lst = [
            start_ang, 
            end_ang, 
            step
        ]
        dt = bytearray(sd_lst)
        for v in dt: 
            sdb.append(v)
        print(sdb)
        # 
        if self.connect_dev.is_open(): 
            self.connect_dev.write(sdb)  # 发送数据

    def dev_reset_val_evt(self): 
        print("dev_reset_val_evt...")
        self.ui.startAngleSpinBox.setValue(0)
        self.ui.endAngleSlider.setValue(180)
        self.ui.stepSlider.setValue(5)
    
    def dev_close_evt(self): 
        if self.dev_read is not None :
            self.is_reading = False
            self.dev_read = None
            print("read close")
        if self.connect_dev is not None :
            self.connect_dev.close()
    
    def set_point_dat(self, dat : str): 
        try:
            lst = dat.split(" ")
            print("set_point_dat...", len(lst))
            if len(lst) != 3:
                return
            print("lst ", lst)
            # 判断是否是数字
            if not lst[1].isdigit() or not lst[2].isdigit():
                return
            # 
            for point in DemoWidget.point_list: 
                if point[0] == int(lst[1]): 
                    print(int(lst[1]))
                    point[1] = int(lst[2])
                    # point[1] = random.randint(2, 400)
                    return
            # 
            DemoWidget.point_list.append([int(lst[1]), int(lst[2])])
                                         #random.randint(2, 400)]) 
        except Exception as e: 
            print(e)      
    
    def rec_dat_exe(self, dat : str): 
        if "Data" not in dat:
            return 
        #
        s = dat.find("Data")
        print("s = ", s)
        if s == -1: 
            return
        e = dat.find(";", s + 4)
        print("e = ", e)
        if e == -1: 
            return
        print(dat[s:e])
        self.set_point_dat(dat[s:e])
        self.rec_dat_exe(dat[e+1:])
    
    def serial_dev_read(self, connect_dev : serial_test.SerialDevice) :
        while self.is_reading:
            try:
                if connect_dev.serial.in_waiting > 0:
                    # print(",,,")
                    dat = connect_dev.read(connect_dev.serial.in_waiting)
                    print(dat)
                    dat_str = dat.decode("gbk", errors='ignore')
                    # 
                    if "set_seet_init_val" in dat_str: 
                        DemoWidget.point_list = []
                        # time.sleep(0.1)
                        continue
                    # 
                    self.rec_dat_exe(dat_str)
                    # 
                    print()
                    print(DemoWidget.point_list)
            except Exception as e:
                print(e)
                print("cls...")
                self.dev_close_evt()
                self.dev_read_stop = True
                break
            time.sleep(0.02)
        print("cls...")
        self.dev_read_stop = True
    
    def dev_connect_evt(self): 
        if self.ui.btn_connect.text() == "断开连接":
            self.dev_close_evt()
            self.ui.btn_connect.setText("连接")
            return
        print("dev_connect_evt...")
        current_serial_port = self.ui.devComboBox.currentData()
        if current_serial_port == "" or current_serial_port is None:
            QtWidgets.QMessageBox.information(self, '提示', '请选择串口设备')
            return
        
        current_serial_baud_rate = self.ui.rateComboBox.currentText()
        if current_serial_baud_rate == "" or current_serial_baud_rate is None:
            QtWidgets.QMessageBox.information(self, '提示', '请选择波特率')
            return
        
        print(current_serial_port, current_serial_baud_rate)
        try:
            self.connect_dev = serial_test.SerialDevice(current_serial_port, current_serial_baud_rate)
        except Exception as e: 
            print(e)
            self.connect_dev = None
            return
        
        ret = self.connect_dev.open()
        print(ret)
        print(self.connect_dev.is_open())
        # 
        if self.connect_dev.is_open():
            if self.dev_read == None and self.dev_read_stop:
                try:
                    self.is_reading = True
                    self.dev_read = threading.Thread(target=self.serial_dev_read, args=(self.connect_dev,))
                    self.dev_read.daemon = True
                    self.dev_read.start()
                    self.dev_read_stop = False
                    self.ui.btn_connect.setText("断开连接")
                    # 
                    self.dev_set_val_evt()
                except Exception as e:
                    print(e)
                    self.is_reading = False
    
    def dev_refresh_evt(self): 
        print("dev_refresh_evt...")
        self.ui.devComboBox.clear()
        # self.comboBox.addItem("")
        serial_ports = serial_test.scan_serial_ports()
        if len(serial_ports) > 0:
            print("Available serial serial_ports:")
            for device, description in serial_ports:
                print(device, "---", description)
                self.ui.devComboBox.addItem(description, device)
        else:
            print("No serial serial_ports found.")
            # 
            QtWidgets.QMessageBox.warning(self, "提示", "没有可用串口")
    