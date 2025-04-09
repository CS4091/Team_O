from tkinter import filedialog as fd
from tkinter.messagebox import showerror, showinfo, showwarning

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

    def select_files(self):
        """
        Select multiple files
        :param files:
        :return:
        """
        try:
            # accepted file types
            filetypes = (
                ("Text files", "*.txt"),
                ("CSV files", "*.csv"),
                ("All files", "*.*"),
            )

            # gather the files
            filenames = fd.askopenfilenames(
                title="Open files", initialdir="/", filetypes=filetypes
            )

            # only show info if there is a file selected
            if filenames != "":
                showinfo(title="Selected Files", message=filenames)

        except ValueError as error:
            self.view.show_error(error)
