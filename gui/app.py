# import backend_binding
import tkinter as tk

from .controller import Controller
from .model import Model
from .view import View


class App(tk.Tk):
    def __init__(self):
        super().__init__()

        # set title for window
        self.title("Air-to-Ground Search Demo")

        # change the icon
        # self.iconbitmap('./assets/pythontutorial.ico')

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
        model = Model()

        # example_map_path: str = "./backend/unit_tests/test_csv/mediumTestGrid.csv"
        # test_model = Model()
        # test_model.grid_map = backend_binding.GridMap(
        #     width=50, filePath=example_map_path
        # )

        # test_model.router = backend_binding.RoutePlanner(
        #     aircraft=backend_binding.Aircraft(
        #         startRow=9,
        #         startDir=backend_binding.Direction.SOUTH,
        #         startCol=4,
        #         map=test_model.grid_map,
        #     )
        # )

        # test_model.router.findRoute()

        # create the view and place it on the root window
        view = View(self)
        view.grid(row=0, column=0)

        # create the controller
        controller = Controller(model, view)

        # set the controller to view
        view.set_controller(controller)
        # view.display_map(data)
