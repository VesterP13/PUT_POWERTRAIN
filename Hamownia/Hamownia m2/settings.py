import tkinter
import customtkinter

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
        global app_text_font, app_text_color, background_color
        settings_window = tkinter.Tk()
        settings_window.geometry("600x400")
        settings_window.title("Ustawienia")
        settings_window.resizable(False, False)
        settings_window.configure(bg = background_color)
        #settings_window.overrideredirect(True)

        settings_window.bind("<Escape>", lambda event: on_closing())
        settings_window.focus_force()

        def on_closing():
            global is_settings_window_open
            is_settings_window_open = False
            settings_window.destroy()
            
        def command_save():
            pass
        
        button_save=customtkinter.CTkButton(master = settings_window, width=200, height=50, command=command_save, text = "Zapisz", text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, bg_color=background_color, fg_color='gray10')
        button_save.place(relx=0.30, rely=0.9, anchor="center")
        
        button_quit=customtkinter.CTkButton(master = settings_window, width=200, height=50, command=on_closing, text = "Wyjdź", text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, bg_color=background_color, fg_color='gray10')
        button_quit.place(relx=0.70, rely=0.9, anchor="center")

        label_control_ip = customtkinter.CTkLabel(master = settings_window, text = "IP", text_color=app_text_color, font=app_text_font)
        label_control_ip.place(relx=0.1, rely=0.1, anchor="center")
        box_control_ip = customtkinter.CTkEntry(master = settings_window, width=150, height=50, text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, bg_color=background_color, fg_color='gray10')
        box_control_ip.place(relx=0.3, rely=0.1, anchor="center")
        
        label_control_port = customtkinter.CTkLabel(master = settings_window, text = "PORT", text_color=app_text_color, font=app_text_font)
        label_control_port.place(relx=0.1, rely=0.3, anchor="center")
        box_control_port = customtkinter.CTkEntry(master = settings_window, width=150, height=50, text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, bg_color=background_color, fg_color='gray10')
        box_control_port.place(relx=0.3, rely=0.3, anchor="center")
        
        label_measure_ip = customtkinter.CTkLabel(master = settings_window, text = "IP", text_color=app_text_color, font=app_text_font)
        label_measure_ip.place(relx=0.5, rely=0.1, anchor="center")
        box_measure_ip = customtkinter.CTkEntry(master = settings_window, width=150, height=50, text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, bg_color=background_color, fg_color='gray10')
        box_measure_ip.place(relx=0.7, rely=0.1, anchor="center")
        
        label_measure_port = customtkinter.CTkLabel(master = settings_window, text = "PORT", text_color=app_text_color, font=app_text_font)
        label_measure_port.place(relx=0.5, rely=0.3, anchor="center")
        box_measure_port = customtkinter.CTkEntry(master = settings_window, width=150, height=50, text_color=app_text_color, font=app_text_font, border_color='gray30', border_width=1, bg_color=background_color, fg_color='gray10')
        box_measure_port.place(relx=0.7, rely=0.3, anchor="center")


       
        settings_window.protocol("WM_DELETE_WINDOW", on_closing)
    else:
        settings_window.focus_force()
