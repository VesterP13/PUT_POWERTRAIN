import customtkinter
import threading
import time
from tkinter import messagebox
import connection
import settings

windows = []
button_list = []
save_window_status = False


#POBRANE USTAWIENIA
app_text_font = settings.app_text_font
app_text_color = settings.app_text_color
log_box_measuring_timeout = settings.log_box_measuring_timeout

#GŁÓWNE OKNO APLIKACJI
main_app_window = customtkinter.CTk()
main_app_window.geometry("1024x600")
main_app_window.title("PUT POWERTRAIN DYNAMOMETER")
main_app_window.resizable(False, False)
#main_app_window.overrideredirect(True)
#main_app_window.focus_force()

#WYŚWIETLANIE
def display_measurment():
    while True:
        try:
            log_box_show_message("Rpm1:"+connection.rpm_1_list[-1]+" Rpm2:"+connection.rpm_2_list[-1]+" Rpm3:"+connection.rpm_3_list[-1]+" Rpm4:"+connection.rpm_4_list[-1]+" Temp. Amb:"+connection.temp_ambient_list[-1]+" Temp. Bat"+connection.temp_baterry_list[-1]+" Curr:"+connection.current_list[-1]+" Volt:"+connection.voltage_list[-1]+" Time:"+connection.time_list[-1])
            global log_box_measuring_timeout
            time.sleep(log_box_measuring_timeout)
        except:
            pass
        global is_measuring_running
        if not is_measuring_running:
            break

def time_counting():
    global start
    while True:
        # Wykonaj jakieś zadanie tutaj
        time.sleep(0.01)  # Poczekaj przez 1 sekundę
        # Oblicz upływający czas
        elapsed_time = time.perf_counter() - start
        # Aktualizuj time_counter
        time_counter.delete("0", "end")
        time_counter.insert("1", f"{elapsed_time:.2f} s")
        time_counter.update()  # Zaktualizuj widok
        if not is_measuring_running:
            break

def time_counting_reset():
    time_counter.delete("0", customtkinter.END)
    time_counter.insert("1", "00.00 s")
    global start
    start = time.perf_counter()

#FUNKCJE PRZYCISKÓW
def command_start():
    global button_wifi_ardumeasure
    if button_wifi_ardumeasure.cget("border_color") == "red":
        time_counting_thread = threading.Thread(target=time_counting, daemon=True)
        if not connection.rpm_1_list:
            global start
            start = time.perf_counter()  # Rozpocznij pomiar czasu
        time_counting_thread.start()
        button_startstop.configure(text="Stop", command=command_stop, border_color='red')
        connection.start_measurement()
        global is_measuring_running
        is_measuring_running = True
        display_measurment_thread = threading.Thread(target=display_measurment, daemon=True)
        display_measurment_thread.start()
    else:
        log_box_show_message("Napierw połącz się ze systemem pomiarowym.")

def command_stop():
    button_startstop.configure(text="Start", command=command_start, border_color='green')
    connection.stop_measurment()
    global is_measuring_running
    is_measuring_running = False

def command_reset():
    if not connection.rpm_1_list:
        log_box_show_message("Nie ma żadnych danych do zresetowania.")
    else:
        connection.reset_data()
        time_counting_reset()
        log_box_show_message("Dane zresetowane.")

def command_save():
    command_stop()
    if connection.rpm_1_list:
        create_question_save_format()
    else:
        log_box_show_message("Nie ma żadnych danych do zapisania.")

def command_start_remote_control():
    if button_wifi_arducontrol.cget("border_color") == "red":
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
    button_remote_control.configure(text="Włącz zdalne sterowanie", command=command_start_remote_control, border_color='green')
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
        log_box_show_message("Nie udało się połączyć ze sterowaniem.")

def is_arduino_control_connected(status):
    if status == True:
        button_wifi_arducontrol.configure(text="Rozłącz sterowanie", border_color="red", command = command_disconnect_arduino_control)
    else:
        button_wifi_arducontrol.configure(text="Połącz ze sterowaniem", border_color="green", command = command_connect_arduino_control)

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
        button_wifi_ardumeasure.configure(text="Rozłącz system pom.", border_color="red", command = command_disconnect_arduino_measure)
    else:
        button_wifi_ardumeasure.configure(text="Połącz z systemem pom.", border_color="green", command = command_connect_arduino_measure)

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

slider1=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000, fg_color="gray30", button_color="gray1", progress_color="green")
slider1.place(relx=0.12, rely=0.25, anchor="center")
slider1_label=customtkinter.CTkLabel(master = main_app_window, text = "m1",font=app_text_font, text_color=app_text_color)
slider1_label.place(relx=0.02, rely=0.25, anchor="center")
slider1.set(1000)

slider2=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000, fg_color="gray30", button_color="gray1", progress_color="green")
slider2.place(relx=0.12, rely=0.35, anchor="center")
slider2_label=customtkinter.CTkLabel(master = main_app_window, text = "m2", font=app_text_font, text_color=app_text_color)
slider2_label.place(relx=0.02, rely=0.35, anchor="center")
slider2.set(1000)

slider3=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000, fg_color="gray30", button_color="gray1", progress_color="green")
slider3.place(relx=0.12, rely=0.45, anchor="center")
slider3_label=customtkinter.CTkLabel(master = main_app_window, text = "m3", font=app_text_font, text_color=app_text_color)
slider3_label.place(relx=0.02, rely=0.45, anchor="center")
slider3.set(1000)

slider4=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000, fg_color="gray30", button_color="gray1", progress_color="green")
slider4.place(relx=0.12, rely=0.55, anchor="center")
slider4_label=customtkinter.CTkLabel(master = main_app_window, text = "m4", font=app_text_font, text_color=app_text_color)
slider4_label.place(relx=0.02, rely=0.55, anchor="center")
slider4.set(1000)

slider5=customtkinter.CTkSlider(master = main_app_window, width=160, height=20, from_=1000, to=2000, command=all_sliders_change, fg_color="gray30", button_color="gray1", progress_color="green")
slider5.place(relx=0.12, rely=0.65, anchor="center")
slider5_label=customtkinter.CTkLabel(master = main_app_window, text = "ms", font=app_text_font, text_color=app_text_color)
slider5_label.place(relx=0.02, rely=0.65, anchor="center")
slider5.set(1000)

#PRZYCISKI
button_startstop=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_start, text = "Start", fg_color='gray10', text_color=app_text_color, font=app_text_font, border_color='green', border_width=1)
button_startstop.place(relx=0.3, rely=0.75, anchor="center")

button_analisys=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command="", text = "Badanie", fg_color='gray10', text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1)
button_analisys.place(relx=0.7, rely=0.75, anchor="center")

button_options=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command="", text = "Opcje", fg_color='gray10', text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1)
button_options.place(relx=0.9, rely=0.75, anchor="center")


button_reset=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_reset, text = "Reset", text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1)
button_reset.place(relx=0.10, rely=0.85, anchor="center")

button_save=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_save, text = "Zapisz", text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1)
button_save.place(relx=0.10, rely=0.75, anchor="center")

button_wifi_arducontrol=customtkinter.CTkButton(master = main_app_window, width=200, height=25, command=command_connect_arduino_control, text = "Połącz ze sterowaniem", fg_color='green', text_color=app_text_color, font=app_text_font,border_color='green', border_width=1)
button_wifi_arducontrol.place(relx=0.1, rely=0.03, anchor="center")
button_wifi_ardumeasure=customtkinter.CTkButton(master = main_app_window, width=200, height=25, command=command_connect_arduino_measure, text = "Połącz z systemem pom.", fg_color='green', text_color=app_text_color, font=app_text_font,border_color='green', border_width=1)
button_wifi_ardumeasure.place(relx=0.1, rely=0.08, anchor="center")

button_remote_control=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_start_remote_control, text = "Włącz zdalne sterowanie", fg_color='green', text_color=app_text_color, font=app_text_font,border_color='green', border_width=1)
button_remote_control.place(relx=0.1, rely=0.15, anchor="center")

def command_clear_log_box():
    log_box.delete("1.0", customtkinter.END)

button_clear_log_box=customtkinter.CTkButton(master = main_app_window, width=200, height=50, command=command_clear_log_box, text = "Czyść logi", text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1)
button_clear_log_box.place(relx=0.1, rely=0.95, anchor="center")

time_counter = customtkinter.CTkEntry(master=main_app_window, width=200, height=50, text_color=app_text_color, font=("Helvetica", 18), border_color='gray30', border_width=1, fg_color="gray10", justify=customtkinter.CENTER)
time_counter.insert("1", "00.00 s")
time_counter.place(relx=0.5, rely=0.75, anchor=customtkinter.CENTER)


#0.9 0.75

button_list.append(button_clear_log_box)
button_list.append(button_startstop)
button_list.append(button_reset)
button_list.append(button_save)
button_list.append(button_wifi_arducontrol)
button_list.append(button_wifi_ardumeasure)
button_list.append(button_remote_control)


for button in button_list:
    button.configure(corner_radius=5, fg_color='gray10')

#POLE TEKSTOWE
def log_box_show_message(message):
    log_box.insert("end", message + "\n")
    log_box.yview(customtkinter.END)

log_box = customtkinter.CTkTextbox(master = main_app_window, width=815, height=110, text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, fg_color="gray10")
log_box.place(relx=0.6, rely=0.9, anchor="center")

#OKNO WYBORU ZAPISU
def create_question_save_format():
    global save_window
    global save_window_status
    if save_window_status == True:
        save_window.focus_force()
        return
    save_window_status = True
    def save_data_to_excel():
        connection.save_data_to_excel()
        on_closing_save_window()

    def save_data_to_txt():
        connection.save_data_to_txt()
        on_closing_save_window()


    save_window =  customtkinter.CTk()
    save_window.geometry("300x75")
    save_window.title("Zapisywanie")
    save_window.resizable(False, False)
    global windows
    windows.append(save_window)

    main_question = customtkinter.CTkLabel(master=save_window, text="Wybierz format zapisu:")
    main_question.place(relx=0.5, rely=0.2, anchor=customtkinter.CENTER)
    main_question.configure(font=app_text_font, text_color=app_text_color)

    button_excel = customtkinter.CTkButton(master=save_window, text="Excel", command=save_data_to_excel, corner_radius=5, fg_color='gray10', text_color='white', border_color='green', border_width=1)
    button_excel.place(relx=0.25, rely=0.8, anchor=customtkinter.CENTER)

    button_txt = customtkinter.CTkButton(master=save_window, text="Txt", command=save_data_to_txt, corner_radius=5, fg_color='gray10', text_color='white', border_color='blue', border_width=1)
    button_txt.place(relx=0.75, rely=0.8, anchor=customtkinter.CENTER)

    save_window.bind("<Escape>", lambda event: save_window.destroy())

    def on_closing_save_window():
        global save_window_status
        save_window_status = False
        save_window.destroy()

    save_window.protocol("WM_DELETE_WINDOW", on_closing_save_window)
    save_window.mainloop()

#CANVAS

from tkinter import PhotoImage
my_canvas = customtkinter.CTkCanvas(master=main_app_window, width=604, height=400, highlightthickness=0)
my_canvas.place(relx=0.5, rely=0.35, anchor=customtkinter.CENTER)
photo = PhotoImage(file="zegary2.png")
my_canvas.create_image(0, 0, image=photo, anchor=customtkinter.NW)



#ZAMYKANIE
def on_closing():
    if connection.rpm_1_list:
        if messagebox.askokcancel("Wyjście", "Czy na pewno chcesz wyjść?\n Wszystkie niezapisane dane zostaną utracone."):
            global windows
            for window in windows:
                try:
                    window.destroy()
                except:
                    pass

            main_app_window.destroy()
    main_app_window.destroy()

print(button_reset.cget('bg_color'))
    
main_app_window.protocol("WM_DELETE_WINDOW", on_closing)
main_app_window.mainloop()

