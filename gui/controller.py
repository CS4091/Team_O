

from tkinter import filedialog as fd
import csv
from tkinter.messagebox import showerror, showwarning, showinfo
from .model import Model

class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.test_model = Model()

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

    def select_file(self):
        """
        Select a file to display
        :param files:
        :return:
        """
        file_path = fd.askopenfilename(filetypes=[("CSV files", "*.csv")])
        if not file_path:
            return
        
        try:
            with open(file_path, newline='') as f:
                reader = csv.reader(f)
                data = [row for row in reader if row]  # skips the empty rows
            self.view.display_map(data)
            self.view.show_success("File loaded successfully!")
        except Exception as e:
            self.view.show_error(f"Failed to load file: {e}")
            
