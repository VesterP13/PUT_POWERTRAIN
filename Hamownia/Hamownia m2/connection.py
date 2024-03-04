import settings
import socket
import threading
import pandas as pd
import datetime
import os

#POBIERANE USTAWIENIA
TCP_CONTROL_IP=settings.TCP_CONTROL_IP
TCP_CONTROL_PORT=settings.TCP_CONTROL_PORT
TCP_MEASURE_IP=settings.TCP_MEASURE_IP
TCP_MEASURE_PORT=settings.TCP_MEASURE_PORT

rpm_1_list = []
rpm_2_list = []
rpm_3_list = []
rpm_4_list = []
temp_ambient_list = []
temp_baterry_list = []
current_list = []
voltage_list = []
arduino_data_list = []
time_list = []

measurement_running = False

#ŁĄCZENIE

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
    
    # Check that arduino_data_list has 8 elements
    if len(arduino_data_list) != 8:
        print("Error: arduino_data_list should have 8 elements")
        return

    rpm_1_list.append(arduino_data_list[0]) 
    rpm_2_list.append(arduino_data_list[1])
    rpm_3_list.append(arduino_data_list[2])
    rpm_4_list.append(arduino_data_list[3])
    temp_ambient_list.append(arduino_data_list[4])
    temp_baterry_list.append(arduino_data_list[5])
    current_list.append(arduino_data_list[6])
    voltage_list.append(arduino_data_list[7])
    time_now = datetime.datetime.now().strftime("%H:%M:%S:%f")[:-3]
    time_list.append(time_now)
    arduino_data_list.clear()

#RESET
def reset_data():
    rpm_1_list.clear()
    rpm_2_list.clear()
    rpm_3_list.clear()
    rpm_4_list.clear()
    temp_ambient_list.clear()
    temp_baterry_list.clear()
    current_list.clear()
    voltage_list.clear()
    time_list.clear()

#ZAPIS 
def strings_to_floats(string_list):

  for i in range(len(string_list)):
    try:
      string_list[i] = float(string_list[i])
    except ValueError:
      string_list[i] = 0
      
  return string_list

def save_data_to_excel():
    df = pd.DataFrame({
    'czas': time_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))],
    'rpm1_silnik1': strings_to_floats(rpm_1_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]),
    'rpm_silnik2': strings_to_floats(rpm_2_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]),
    'rpm_silnik3': strings_to_floats(rpm_3_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]), 
    'rpm_silnik4': strings_to_floats(rpm_4_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]),
    'temp. otoczenia': strings_to_floats(temp_ambient_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]),
    'temp. baterii': strings_to_floats(temp_baterry_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]),
    'prąd': strings_to_floats(current_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))]),
    'napięcie': strings_to_floats(voltage_list[:min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list), len(current_list), len(voltage_list))])
    })
    aktualna_data = datetime.datetime.now().strftime("%Y_%m_%d") 
    nazwa_pliku = f"{aktualna_data}.xlsx"
    numer_cyfry = 1
    while os.path.exists(nazwa_pliku):
        numer_cyfry += 1
        nazwa_pliku = f"{aktualna_data}_{numer_cyfry}.xlsx"
    df.to_excel(nazwa_pliku, index=False) 

def save_data_to_txt():
    aktualna_data = datetime.datetime.now().strftime("%Y_%m_%d")
    nazwa_pliku = f"{aktualna_data}.txt"
    numer_cyfry = 1
    while os.path.exists(nazwa_pliku):
        numer_cyfry += 1
        nazwa_pliku = f"{aktualna_data}_{numer_cyfry}.txt"
    with open(nazwa_pliku, "w") as f:
        f.write("czas;rpm1_silnik1;rpm_silnik2;rpm_silnik3;rpm_silnik4;temp. otoczenia;temp. baterii;prąd;napięcie\n")
        for i in range(min(len(time_list), len(rpm_1_list), len(rpm_2_list), len(rpm_3_list), len(rpm_4_list), len(temp_ambient_list), len(temp_baterry_list))):
            f.write(f"{time_list[i]};{rpm_1_list[i]};{rpm_2_list[i]};{rpm_3_list[i]};{rpm_4_list[i]};{temp_ambient_list[i]};{temp_baterry_list[i]};{current_list[i]};{voltage_list[i]}\n")