import tkinter as tk
from tkinter import ttk
from tkinter import filedialog as fd


class Model:
    def __init__(self, map):
        self.map = map

    def save(self):
        """
        Save the map to a file
        :return:
        """
        with open("map_file.txt", "a") as f:
            f.write(self.map)


class View(ttk.Frame):
    def __init__(self, parent):
        super().__init__(parent)

        # create widgets
        # create frame for map visual
        map_frame = ttk.Frame(self)

        # grid layout for the map frame
        map_frame.columnconfigure(5, weight=1)
        map_frame.columnconfigure(5, weight=3)

        ttk.Label(map_frame, text='place map').grid(column=5, row=0, sticky=tk.SE)
        
        # map_frame['borderwidth'] = 5
        # map_frame['relief'] = 'groove'
        # map_frame['padding'] = (5, 10)

        # label
        self.label_map = ttk.Label(self, text="Map", background="green")
        self.label_map.grid(column=12, row=4, columnspan=4, sticky=tk.NE, padx=5, pady=5)

        # separator
        # self.separator = ttk.Separator(self, orient="vertical")
        # self.separator.pack(fill='y')

        # save button
        self.save_button = ttk.Button(
            self, text="Save", command=self.save_button_clicked
        )
        self.save_button.grid(row=1, column=3, padx=10)

        # message
        self.message_label = ttk.Label(self, text="", foreground="red")
        self.message_label.grid(row=2, column=1, sticky=tk.W)

        # set the controller
        self.controller = None

    def set_controller(self, controller):
        """
        Set the controller
        :param controller:
        :return:
        """
        self.controller = controller

    def save_button_clicked(self):
        """
        Handle button click event
        :return:
        """
        if self.controller:
            self.controller.save()

    def show_success(self, message):
        """
        Show a success message
        :param message:
        :return:
        """
        self.message_label["text"] = message
        self.message_label["foreground"] = "green"
        self.message_label.after(3000, self.hide_message)

        # reset the form
        self.email_entry["foreground"] = "black"
        self.email_var.set("")

    def show_error(self, message):
        """
        Show an error message
        :param message:
        :return:
        """
        self.message_label["text"] = message
        self.message_label["foreground"] = "red"
        self.message_label.after(3000, self.hide_message)
        self.email_entry["foreground"] = "red"

    def hide_message(self):
        """
        Hide the message
        :return:
        """
        self.message_label["text"] = ""


class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view

    def save(self, email):
        """
        Save the email
        :param email:
        :return:
        """
        try:

            # save the model
            self.model.email = email
            self.model.save()

            # show a success message
            self.view.show_success(f"The email {email} saved!")

        except ValueError as error:
            # show an error message
            self.view.show_error(error)


class App(tk.Tk):
    def __init__(self):
        super().__init__()

        # set title for window
        self.title("Air-to-Ground Search Demo")

        # change the icon
        # root.iconbitmap('./assets/pythontutorial.ico')

        # set window sizes
        window_width = 1000
        window_height = 600

        # get the screen dimension
        screen_width = self.winfo_screenwidth()
        screen_height = self.winfo_screenheight()

        # find the center point
        center_x = int(screen_width / 2 - window_width / 2)
        center_y = int(screen_height / 2 - window_height / 2)

        # set the position of the window to the center of the screen
        self.geometry(f"{window_width}x{window_height}+{center_x}+{center_y}")

        # set the window to the topmost layer
        self.attributes("-topmost", 1)

        # set the window so that it cant be resized
        self.resizable(False, False)

        # configure the grid weights
        # self.columnconfigure(0, weight=1)
        # self.columnconfigure(2,weight=4)
        # self.rowconfigure(2, weight=4)

        # create the model
        model = Model("test")

        # create the view and place it on the root window
        view = View(self)
        view.grid(row=0, column=0, padx=10, pady=10)

        # create the controller
        controller = Controller(model, view)

        # set the controller to view
        view.set_controller(controller)


if __name__ == "__main__":
    app = App()
    app.mainloop()
