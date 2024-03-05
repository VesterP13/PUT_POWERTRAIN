import tkinter

#USTAWIENIA MOTYWU

app_text_font = ("Helvetica", 13)   #DOMYŚLNIE ("Helvetica", 12)

app_text_color = "white"            #DOMYŚLNIE "white"

background_color = "gray14"         #DOMYŚLNIE "gray14"

#USTAWIENIA POŁĄCZENIA

TCP_CONTROL_IP=""                   #IP ARDUINO STERUJĄCEGO

TCP_CONTROL_PORT=12345              #PORT ARDUINOS STERUJĄCEGO

TCP_MEASURE_IP="192.168.1.3"        #IP ARDUINO POMIAROWEGO

TCP_MEASURE_PORT=12345              #PORT ARDUINO POMIAROWEGO

#USTAWIENIA ZEGARÓW

max_rpm_on_display = 40000          #MAKSYMALNA WARTOŚĆ PRĘDKOŚĆI OBROTOWEJ

#USTAWIENIA INNE

log_box_measuring_timeout = 0.2

#USTAWIENIA OKNA

is_settings_window_open = False

def create_settings_window():
    global is_settings_window_open
    if not is_settings_window_open:

        is_settings_window_open = True

        global settings_window
        settings_window = tkinter.Tk()
        settings_window.geometry("600x400")
        settings_window.title("Ustawienia")
        settings_window.resizable(False, False)
        settings_window.configure(bg = background_color)
        #settings_window.overrideredirect(True)

        settings_window.bind("<Escape>", lambda event: settings_window.destroy())
        settings_window.focus_force()

        def on_closing():
            global is_settings_window_open
            is_settings_window_open = False
            settings_window.destroy()
       
        settings_window.protocol("WM_DELETE_WINDOW", on_closing)
    else:
        settings_window.focus_force()

