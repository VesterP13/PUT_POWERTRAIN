import socket
import threading
import queue

lock = threading.Lock()

TCP_CONTROL_IP=""
TCP_CONTROL_PORT=12345

TCP_MEASURE_IP="192.168.1.3"
TCP_MEASURE_PORT=12345

rpm_1_list = []
rpm_2_list = []
rpm_3_list = []
rpm_4_list = []
temp_ambient_list = []
temp_baterry_list = []
current_list = []
voltage_list = []
arduino_data_list = []

measurement_running = False

def connect_control_arduino_by_wifi():
    global TCP_CONTROL_IP
    global TCP_CONTROL_PORT
    global sock_arduino_control
    print("Connecting to arduino by WiFi")
    try:
        sock_arduino_control = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock_arduino_control.settimeout(2)
        sock_arduino_control.connect((TCP_CONTROL_IP, TCP_CONTROL_PORT))
        print("Connecting to arduino by WiFi")
        return sock_arduino_control
    except:
        return None
    
def disconnect_control_arduino_by_wifi():
    global sock_arduino_control
    sock_arduino_control.close()
    print("Disconnecting from arduino by WiFi")

def connect_measure_arduino_by_wifi():
    global TCP_MEASURE_IP
    global TCP_MEASURE_PORT
    global sock_arduino_measure
    try:
        sock_arduino_measure = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock_arduino_measure.settimeout(2)
        sock_arduino_measure.connect((TCP_MEASURE_IP, TCP_MEASURE_PORT))
        return sock_arduino_measure
    except:
        return None
    
def disconnect_measure_arduino_by_wifi():
    global sock_arduino_measure
    sock_arduino_measure.close()
    print("Disconnecting from arduino by WiFi")

#ZDALNE STEROWANIE

def motor1_control(value):
    global sock_arduino_control
    message = "1" + str(value) + "#"
    print(message)
    try:
        sock_arduino_control.sendall(message.encode())
    except:
        print("Utracono połączenie z Arduino")

def motor2_control(value):
    global sock_arduino_control
    message = "2" + str(value) + "#"
    print(message)
    try:
        sock_arduino_control.sendall(message.encode())
    except:
        print("Utracono połączenie z Arduino")

def motor3_control(value):
    global sock_arduino_control
    message = "3" + str(value) + "#"
    print(message)
    try:
        sock_arduino_control.sendall(message.encode())
    except:
        print("Utracono połączenie z Arduino")

def motor4_control(value):
    global sock_arduino_control
    message = "4" + str(value) + "#"
    print(message)
    try:
        sock_arduino_control.sendall(message.encode())
    except:
        print("Utracono połączenie z Arduino")

def motors_control(value):
    global sock_arduino_control
    message = "9" + str(value) + "#"
    print(message)
    try:
        sock_arduino_control.sendall(message.encode())
    except:
        print("Utracono połączenie z Arduino")

#POMIAR
        
def start_measurement():
    global measurement_running
    measurement_running = True
    measurement_thread = threading.Thread(target=measurement, daemon=True)
    measurement_thread.start()

def stop_measurment():
    global measurement_running
    measurement_running = False
    print(measurement_running)
        
def measurement():
    global sock_arduino_measure
    while True:
        received_message = sock_arduino_measure.recv(1024).decode()
        received_message = received_message.replace("\r\n", "")
        global measurement_running
        if not measurement_running:
            break
        elif received_message.startswith("*") and received_message.endswith("#"):
            measuring_data_handling(received_message)
        
def measuring_data_handling(received_message):
    received_message = received_message[1:-1]
    arduino_data_list = received_message.split(";")
    rpm_1_list.append(arduino_data_list[0])
    rpm_2_list.append(arduino_data_list[1])
    rpm_3_list.append(arduino_data_list[2]) 
    rpm_4_list.append(arduino_data_list[3])
    temp_ambient_list.append(arduino_data_list[4])
    temp_baterry_list.append(arduino_data_list[5])
    current_list.append(arduino_data_list[6])
    voltage_list.append(arduino_data_list[7])
    arduino_data_list.clear()