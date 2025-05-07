"""Main application module.

This module initializes the Tkinter root window and combines all components of
the Model-View-Controller application.
"""

# import tkinter as tk
import ttkthemes

from .controller import Controller
from .model import Model
from .view import View

# THEME ="scidgreen"
THEME = "radiance"


class App(ttkthemes.ThemedTk):
    """Tkinter MVC application."""

    def __init__(self) -> None:
        """Application constructor."""
        super().__init__(theme=THEME)

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

        # find the center point
        center_x = int(screen_width / 2 - window_width / 2)
        center_y = int(screen_height / 2 - window_height / 2)

        # set the position of the window to the center of the screen
        self.geometry(f"{window_width}x{window_height}+{center_x}+{center_y}")

        # set the window so that it cant be resized, JK LOL
        self.resizable(True, True)

        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        # create the model
        model = Model()

        # create the view and place it on the root window
        view = View(self)
        self.config(menu=view.menu_bar)

        # make sure view expands to fill window
        view.grid(row=0, column=0, sticky="nsew")

        # create the controller
        controller = Controller(model, view)

        # set the controller to view
        view.set_controller(controller)
