import tkinter as tk
from tkinter import ttk
from tkinter import filedialog as fd
from tkinter.messagebox import showerror, showwarning, showinfo


class Model:
    def __init__(self, map):
        self.map = map

    def save_map(self):
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
        map_frame.columnconfigure(0, weight=1)
        map_frame.columnconfigure(1, weight=1)
        map_frame.columnconfigure(1, weight=3)
        map_frame.rowconfigure(1, weight=3)
        
        # map_frame['borderwidth'] = 5
        # map_frame['relief'] = 'groove'
        # map_frame['padding'] = (5, 10)

        # buttons for map frame
        map_frame_button1 = ttk.Button(map_frame, text='Previous Route')
        map_frame_button1.grid(column=0, row=0, padx=10, sticky=tk.W+tk.E)

        map_frame_button2 = ttk.Button(map_frame, text='Save Route')
        map_frame_button2.grid(column=1, row=0, padx=10, sticky=tk.E+tk.W)

        map_frame_button3 = ttk.Button(map_frame, text='Next Route')
        map_frame_button3.grid(column=2, row=0, padx=10, sticky=tk.E+tk.W)

        map_frame_map = ttk.Label(map_frame, text='Map Here')
        map_frame_map.grid(column=1, row=1, padx=10, pady=10, sticky=tk.E+tk.W)

        map_frame.grid(column=5, row=5, padx=10, pady=10)

        
        # labels
        # self.label_temp = ttk.Label(self, text="temp")
        # self.label_temp.grid(column=0, row=0, padx=10)

        # TODO: Show csv file

        # separator
        # self.separator = ttk.Separator(self, orient="vertical")
        # self.separator.pack(fill='y')

        # buttons
        # open file button
        self.open_file_button = ttk.Button(self, text='Open a File', command=self.file_open_button_clicked)
        self.open_file_button.grid(column=10, row=10, padx=10, pady=10)

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
            self.controller.save_map()

    def show_success(self, message):
        """
        Show a success message
        :param message:
        :return:
        """
        self.message_label["text"] = message
        self.message_label["foreground"] = "green"
        self.message_label.after(3000, self.hide_message)


    def show_error(self, message):
        """
        Show an error message
        :param message:
        :return:
        """
        self.message_label["text"] = message
        self.message_label["foreground"] = "red"
        self.message_label.after(3000, self.hide_message)

    def hide_message(self):
        """
        Hide the message
        :return:
        """
        self.message_label["text"] = ""

    def file_open_button_clicked(self):
        """
        Handle open file button click event
        :return:
        """
        # open file button
        if self.controller:
            self.controller.select_files()


class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view

    def save(self, map):
        """
        Save the map
        :param map:
        :return:
        """
        try:

            # save the model
            self.model.map = map
            self.model.save()

            # show a success message
            self.view.show_success(f"The map {map} saved!")

        except ValueError as error:
            # show an error message
            self.view.show_error(error)

    def select_files(self):
        """
        Select multiple files
        :param files:
        :return:
        """
        try:
            # accepted file types
            filetypes = (
                ('Text files', '*.txt'),
                ('CSV files', '*.csv'),
                ('All files', '*.*')
            )

            # gather the files
            filenames = fd.askopenfilenames(
                title='Open files',
                initialdir='/',
                filetypes=filetypes
            )

            # only show info if there is a file selected
            if filenames != '':
                showinfo(
                    title='Selected Files',
                    message=filenames
                )

            
        except ValueError as error:
            self.view.show_error(error)


class App(tk.Tk):
    def __init__(self):
        super().__init__()

        # set title for window
        self.title("Air-to-Ground Search Demo")

        # change the icon
        # self.iconbitmap('./assets/pythontutorial.ico')

        # set resolution
        

        # set window sizes
        window_width = 1000
        window_height = 600

        # get the screen dimension
        screen_width = self.winfo_screenwidth()
        screen_height = self.winfo_screenheight()

        # configure grid to place widgets
        self.columnconfigure(index=10, weight=0)
        self.rowconfigure(index=10, weight=0)

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
        view.grid(row=0, column=0)

        # create the controller
        controller = Controller(model, view)

        # set the controller to view
        view.set_controller(controller)


if __name__ == "__main__":
    app = App()
    app.mainloop()
