"""View for the MVC Tkinter GUI"""

import tkinter as tk
from tkinter import ttk

class View(ttk.Frame):
    """The View for the Graphical User Interface.

    The view is for managing all the buttons and frames that is showing on the
    window.

    Attributes:
        map_frame: private attribute that holds where the map will be showing.
    
    """
    def __init__(self, parent):
        super().__init__(parent)

        # create widgets
        # create frame for map visual
        self.map_frame = ttk.Frame(self)

        # grid layout for the map frame
        self.map_frame.columnconfigure(0, weight=1)
        #map_frame.columnconfigure(1, weight=1)
        #map_frame.columnconfigure(1, weight=3)
        self.map_frame.rowconfigure(1, weight=3)
        
        self.map_frame['borderwidth'] = 5
        self.map_frame['relief'] = 'groove'
        self.map_frame['padding'] = (5, 10)

        # buttons for map frame
        self.map_frame_button1 = ttk.Button(self.map_frame, text='Previous Route')
        self.map_frame_button1.grid(column=0, row=0, padx=10, sticky=tk.W+tk.E)

        self.map_frame_button2 = ttk.Button(self.map_frame, text='Save Route')
        self.map_frame_button2.grid(column=1, row=0, padx=10)

        self.map_frame_button3 = ttk.Button(self.map_frame, text='Next Route')
        self.map_frame_button3.grid(column=2, row=0, padx=10, sticky=tk.E+tk.W)

        # tree for csv file
        self.tree = ttk.Treeview(self.map_frame)
        self.tree.grid(column=0, row=1, columnspan=3, sticky=tk.NSEW)

        self.map_frame_map = ttk.Label(self.map_frame, text='Map Here')
        self.map_frame_map.grid(column=1, row=1, padx=250, pady=200, sticky=tk.E+tk.W)

        self.map_frame.grid(column=5, row=5, padx=100, pady=50, sticky=tk.E+tk.W)

        # buttons
        # open file button
        #self.open_file_button = ttk.Button(self, text='Open a File', command=self.file_open_button_clicked)
        #self.open_file_button.grid(column=5, row=0, padx=10, pady=10)

        # message label
        self.message_label = ttk.Label(self, text="", foreground="green")
        self.message_label.grid(column=0, row=1, sticky=tk.W)


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

    def display_map(self, data):
        # clear existing map
        self.tree.delete(*self.get_children())
        
        num_columns = len(data[0])
        columns = [str(i) for i in range(num_columns)]
        self.tree["columns"] = columns
        self.tree["show"] = "headings"

        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=20, anchor="center")
        
        for row in data:
            self.tree.insert("", tk.END, values=row)

    

