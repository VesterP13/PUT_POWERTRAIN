import customtkinter
import connection
import threading

#MOTYW
app_text_font = ("Helvetica", 14)
app_text_color = "white"

#GŁÓWNE OKNO APLIKACJI
main_app_window = customtkinter.CTk()
main_app_window.geometry("1024x600")
main_app_window.title("PUT POWERTRAIN DYNAMOMETER")

def display_update():
    while True:
        log_box_show_message()

#FUNKCJE PRZYCISKÓW
def command_start():
    global button_wifi_ardumeasure
    if button_wifi_ardumeasure.cget("fg_color") == "red":
        button_startstop.configure(text="Stop", command=command_stop, fg_color='red')
        connection.start_measurement()
    else:
        log_box_show_message("Napierw połącz się ze systemem pomiarowym.")

def command_stop():
    button_startstop.configure(text="Start", command=command_start, fg_color='green')
    connection.stop_measurment()

def command_reset():
    log_box_show_message("Rpm1:"+connection.rpm_1_list[-1]+" Rpm2:"+connection.rpm_2_list[-1]+" Rpm3:"+connection.rpm_3_list[-1]+" Rpm4"+connection.rpm_4_list[-1])
    pass

def command_save():
    pass

def command_start_remote_control():
    if button_wifi_arducontrol.cget("fg_color") == "red":
        button_remote_control.configure(text="Wyłącz zdalne sterowanie", command=command_stop_remote_control, fg_color='red')
        slider1.configure(command = motor1_control)
        slider2.configure(command = motor2_control)
        slider3.configure(command = motor3_control)
        slider4.configure(command = motor4_control)
        slider5.configure(command = motors_control)
        log_box_show_message("Zdalne sterowanie włączone.")
    else:
        log_box_show_message("Napierw połącz się ze sterowaniem.")

def command_stop_remote_control():
    button_remote_control.configure(text="Włącz zdalne sterowanie", command=command_start_remote_control, fg_color='green')
    slider1.configure(command = None)
    slider2.configure(command = None)
    slider3.configure(command = None)
    slider4.configure(command = None)
    slider5.configure(command = None)
    log_box_show_message("Zdalne sterowanie wyłączone.")

def command_connect_arduino_control():
    global socket_arduino_control
    socket_arduino_control = connection.connect_control_arduino_by_wifi()
    if socket_arduino_control:
        is_arduino_control_connected(True)
        log_box_show_message("Połączono ze sterowaniem.")
    else:
        log_box_show_message("Nie udało się połączyć z sterowaniem.")

def is_arduino_control_connected(status):
    if status == True:
        button_wifi_arducontrol.configure(text="Rozłącz sterowanie", fg_color='red', command = command_disconnect_arduino_control)
    else:
        button_wifi_arducontrol.configure(text="Połącz ze sterowaniem", fg_color='green', command = command_connect_arduino_control)

def command_disconnect_arduino_control():
    global socket_arduino_control
    connection.disconnect_control_arduino_by_wifi()
    is_arduino_control_connected(False)
    command_stop_remote_control()
    log_box_show_message("Rozłączono ze sterowaniem.")

def command_connect_arduino_measure():
    global socket_arduino_measure
    socket_arduino_measure = connection.connect_measure_arduino_by_wifi()
    if socket_arduino_measure:
        is_arduino_measure_connected(True)
        log_box_show_message("Połączono ze systemem pomiarowym.")
    else:
        log_box_show_message("Nie udało się połączyć ze systemem pomiarowym.")

def is_arduino_measure_connected(status):
    if status == True:
        button_wifi_ardumeasure.configure(text="Rozłącz system pom.", fg_color='red', command = command_disconnect_arduino_measure)
    else:
        button_wifi_ardumeasure.configure(text="Połącz ze systemem pomiarowym", fg_color='green', command = command_connect_arduino_measure)

def command_disconnect_arduino_measure():
    global socket_arduino_measure
    connection.disconnect_measure_arduino_by_wifi()
    is_arduino_measure_connected(False)
    log_box_show_message("Rozłączono ze systemem pomiarowym.")


#STEROWANIE
def motor1_control(value):
    threading.Thread(target=connection.motor1_control(value))
    log_box_show_message("Ustawiono motor 1 na: " + motor_percentage(value))

def motor2_control(value):
    threading.Thread(target=connection.motor2_control(motor_percentage(value)))
    log_box_show_message("Ustawiono motor 2 na: " + motor_percentage(value))

def motor3_control(value):
    threading.Thread(target=connection.motor3_control(value))
    log_box_show_message("Ustawiono motor 3 na: " + motor_percentage(value))

def motor4_control(value):
    threading.Thread(target=connection.motor4_control(value))
    log_box_show_message("Ustawiono motor 4 na: " + motor_percentage(value))

def motors_control(value):
    threading.Thread(target=connection.motors_control(value))
    log_box_show_message("Ustawiono wszystkie motory na: " + motor_percentage(value))

def motor_percentage(value):
    value = (value - 1000)/10
    return str(value) + "%"

#SLIDERY
def all_sliders_change(value):
    slider1.set(value)
    slider2.set(value)
    slider3.set(value)
    slider4.set(value)

slider1=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000)
slider1.place(relx=0.12, rely=0.25, anchor="center")
slider1_label=customtkinter.CTkLabel(master = main_app_window, text = "m1",font=app_text_font, text_color=app_text_color)
slider1_label.place(relx=0.02, rely=0.25, anchor="center")
slider1.set(1000)

slider2=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000)
slider2.place(relx=0.12, rely=0.35, anchor="center")
slider2_label=customtkinter.CTkLabel(master = main_app_window, text = "m2", font=app_text_font, text_color=app_text_color)
slider2_label.place(relx=0.02, rely=0.35, anchor="center")
slider2.set(1000)

slider3=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000)
slider3.place(relx=0.12, rely=0.45, anchor="center")
slider3_label=customtkinter.CTkLabel(master = main_app_window, text = "m3", font=app_text_font, text_color=app_text_color)
slider3_label.place(relx=0.02, rely=0.45, anchor="center")
slider3.set(1000)

slider4=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000)
slider4.place(relx=0.12, rely=0.55, anchor="center")
slider4_label=customtkinter.CTkLabel(master = main_app_window, text = "m4", font=app_text_font, text_color=app_text_color)
slider4_label.place(relx=0.02, rely=0.55, anchor="center")
slider4.set(1000)

slider5=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000, command=all_sliders_change)
slider5.place(relx=0.12, rely=0.65, anchor="center")
slider5_label=customtkinter.CTkLabel(master = main_app_window, text = "ms", font=app_text_font, text_color=app_text_color)
slider5_label.place(relx=0.02, rely=0.65, anchor="center")
slider5.set(1000)

#PRZYCISKI
button_startstop=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_start, text = "Start", fg_color='green', text_color=app_text_color, font=app_text_font)
button_startstop.place(relx=0.10, rely=0.75, anchor="center")

button_reset=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_reset, text = "Reset", text_color=app_text_color, font=app_text_font)
button_reset.place(relx=0.10, rely=0.85, anchor="center")

button_save=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_save, text = "Zapisz", text_color=app_text_color, font=app_text_font)
button_save.place(relx=0.10, rely=0.95, anchor="center")

button_wifi_arducontrol=customtkinter.CTkButton(master = main_app_window, width=200, height=25, command=command_connect_arduino_control, text = "Połącz ze sterowaniem", fg_color='green', text_color=app_text_color, font=app_text_font)
button_wifi_arducontrol.place(relx=0.1, rely=0.03, anchor="center")
button_wifi_ardumeasure=customtkinter.CTkButton(master = main_app_window, width=200, height=25, command=command_connect_arduino_measure, text = "Połącz z systemem pom.", fg_color='green', text_color=app_text_color, font=app_text_font)
button_wifi_ardumeasure.place(relx=0.1, rely=0.08, anchor="center")

button_remote_control=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_start_remote_control, text = "Włącz zdalne sterowanie", fg_color='green', text_color=app_text_color, font=app_text_font)
button_remote_control.place(relx=0.1, rely=0.15, anchor="center")

#POLE TEKSTOWE
def log_box_show_message(message):
    log_box.insert("end", message + "\n")
    log_box.yview(customtkinter.END)

log_box = customtkinter.CTkTextbox(master = main_app_window, width=800, height=100, text_color=app_text_color, font=app_text_font)
log_box.place(relx=0.6, rely=0.9, anchor="center")

main_app_window.mainloop()
