"""View for the MVC Tkinter GUI"""

import tkinter as tk
from tkinter import ttk, Menu
from .controller import Controller

class View(ttk.Frame):
    """The View for the Graphical User Interface.

    The view is for managing all the buttons and frames that is showing on the
    window.

    Attributes:
        map_frame: private attribute that holds where the map will be showing.
    
    """
    def __init__(self, parent):
        super().__init__(parent)

        self.grid(row=0, column=0, sticky="nsew")

        # configure this frame's grid to expand
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)

        # create frame for map visual
        self.map_frame = ttk.Frame(self)
        self.map_frame.grid(row=0, column=0, sticky="nsew", padx=20, pady=20)

        # grid layout for the map frame
        self.map_frame.columnconfigure(0, weight=1)
        self.map_frame.columnconfigure(1, weight=1)
        self.map_frame.columnconfigure(2, weight=1)
        self.map_frame.rowconfigure(1, weight=1)
        
        self.map_frame['borderwidth'] = 5
        self.map_frame['relief'] = 'groove'
        self.map_frame['padding'] = (5, 10)

        # buttons for map frame
        self.map_frame_button1 = ttk.Button(self.map_frame, text='Previous Route')
        self.map_frame_button1.grid(column=0, row=0, padx=10)

        self.map_frame_button2 = ttk.Button(self.map_frame, text='Save Route')
        self.map_frame_button2.grid(column=1, row=0, padx=10)

        self.map_frame_button3 = ttk.Button(self.map_frame, text='Next Route')
        self.map_frame_button3.grid(column=2, row=0, padx=10)

        # canvas for CSV file
        self.canvas = tk.Canvas(self.map_frame)
        self.canvas.grid(column=0, row=1, columnspan=3, sticky=tk.NSEW)
        
        # menu bar
        self.menu_bar = Menu(self)

        file_menu = Menu(self.menu_bar, tearoff=0)
        file_menu.add_command(label='Open...', command=self.file_open_button_clicked)
        file_menu.add_separator()
        file_menu.add_command(label='Close')
        self.menu_bar.add_cascade(label='File', menu=file_menu)
        
        # message label
        self.message_label = ttk.Label(self, text="", foreground="green")
        self.message_label.grid(column=0, row=0, sticky="nw")


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
            self.controller.select_file()

    def display_map(self, data):
        # clear previous drawings
        self.canvas.delete("all")

        if not data:
            return

        num_rows = len(data)
        num_cols = max(len(row) for row in data)

        # get current canvas display size in pixels
        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()

        # if the canvas hasn't been drawn yet, force update
        if canvas_width <= 1 or canvas_height <= 1:
            self.canvas.update_idletasks()
            canvas_width = self.canvas.winfo_width()
            canvas_height = self.canvas.winfo_height()

        # compute the cell size so that the whole grid fits
        cell_width = canvas_width // num_cols if num_cols else 20
        cell_height = canvas_height // num_rows if num_rows else 20
        cell_size = min(cell_width, cell_height)

        for row_idx, row in enumerate(data):
            for col_idx, val in enumerate(row):
                x1 = col_idx * cell_size
                y1 = row_idx * cell_size
                x2 = x1 + cell_size
                y2 = y1 + cell_size

                color = "white" if str(val).strip() == '1' else "black"
                self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline='gray')

        # resize scroll region
        self.canvas.config(scrollregion=self.canvas.bbox("all"))