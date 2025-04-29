"""
View module for the MVC Tkinter GUI.

This module defines the View class, responsible for constructing and
managing the graphical user interface (GUI) components.
"""

import time
import tkinter as tk
from tkinter import Menu, ttk

import backend_binding

from .controller import Controller


class View(ttk.Frame):
    """The View for the Graphical User Interface.

    The view is for managing all the buttons and frames that is showing on the
    window.

    Attributes:
        map_frame: private attribute that holds where the map will be showing.

    """

    def __init__(self, parent: tk.Widget) -> None:
        """Construct for the View component."""
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

        self.map_frame["borderwidth"] = 5
        self.map_frame["relief"] = "groove"
        self.map_frame["padding"] = (5, 10)

        # buttons for map frame
        self.map_frame_button1 = ttk.Button(
            self.map_frame, text="Previous Route"
        )
        self.map_frame_button1.grid(column=0, row=0, padx=10)

        # buttons for map frame
        self.map_frame_button1 = ttk.Button(
            self.map_frame, text="Previous Route"
        )
        self.map_frame_button1.grid(
            column=0, row=0, padx=10, sticky=tk.W + tk.E
        )

        self.map_frame_button2 = ttk.Button(self.map_frame, text="Save Route")
        self.map_frame_button2.grid(column=1, row=0, padx=10)

        self.map_frame_button3 = ttk.Button(self.map_frame, text="Next Route")
        self.map_frame_button3.grid(column=2, row=0, padx=10)

        # canvas for CSV file
        self.canvas = tk.Canvas(self.map_frame)
        self.canvas.grid(column=0, row=1, columnspan=3, sticky=tk.NSEW)

        # menu bar
        self.menu_bar = Menu(self)

        file_menu = Menu(self.menu_bar, tearoff=0)
        file_menu.add_command(
            label="Open...", command=self.file_open_button_clicked
        )
        file_menu.add_separator()
        file_menu.add_command(label="Close")
        self.menu_bar.add_cascade(label="File", menu=file_menu)

        # message label
        self.message_label = ttk.Label(self, text="", foreground="green")
        self.message_label.grid(column=0, row=0, sticky="nw")

        # set the controller
        self.controller = None

        # set the airplane start position
        self.destination_frame = ttk.Frame(self.map_frame)
        self.destination_frame.grid(column=0, row=2, columnspan=3, pady=10)

        self.row_label = ttk.Label(self.destination_frame, text="Row:")
        self.row_label.grid(column=0, row=0, padx=5, sticky=tk.E)

        self.row_entry = ttk.Entry(self.destination_frame, width=10)
        self.row_entry.grid(column=1, row=0, padx=5, sticky=tk.W)

        self.column_label = ttk.Label(self.destination_frame, text="Column:")
        self.column_label.grid(column=2, row=0, padx=5, sticky=tk.E)

        self.column_entry = ttk.Entry(self.destination_frame, width=10)
        self.column_entry.grid(column=3, row=0, padx=5, sticky=tk.W)

        self.set_destination_button = ttk.Button(
            self.destination_frame,
            text="Place Aircraft",
            command=self.place_aircraft_clicked,
        )
        self.set_destination_button.grid(column=4, row=0, padx=10)

        # start the scan
        self.scan_search_percentage_entry = ttk.Entry(self.map_frame, width=5)
        self.scan_search_percentage_entry.grid(column=4, row=0, padx=5)
        self.scan_search_percentage_entry.insert(0, "0.80")

        self.scan_move_limit_entry = ttk.Entry(self.map_frame, width=5)
        self.scan_move_limit_entry.grid(column=5, row=0, padx=5)
        self.scan_move_limit_entry.insert(0, "1000000")

        self.map_frame_button4 = ttk.Button(
            self.map_frame, text="Scan", command=self.scan_button_clicked
        )
        self.map_frame_button4.grid(column=3, row=0, padx=10)

    def set_controller(self, controller: Controller) -> None:
        """Set the controller
        :param controller:
        :return:
        """
        self.controller = controller

    def save_button_clicked(self) -> None:
        """
        Handle button click event
        :return:
        """
        if self.controller:
            self.controller.save_map()

    def show_success(self, message: str, timeout: int = 3000) -> None:
        """
        Show a success message
        :param message:
        :return:
        """
        self.message_label["text"] = message
        self.message_label["foreground"] = "green"
        self.message_label.after(timeout, self.hide_message)

    def show_error(self, message: str, timeout: int = 3000) -> None:
        """
        Show an error message
        :param message:
        :return:
        """
        self.message_label["text"] = message
        self.message_label["foreground"] = "red"
        self.message_label.after(timeout, self.hide_message)

    def hide_message(self) -> None:
        """
        Hide the message
        :return:
        """
        self.message_label["text"] = ""

    def file_open_button_clicked(self) -> None:
        """
        Handle open file button click event
        :return:
        """
        # open file button
        if self.controller:
            self.controller.select_file()

    def display_map(
        self,
        grid_map: backend_binding.GridMap,
        aircraft: backend_binding.Aircraft | None = None,
    ) -> None:
        # clear previous drawings
        self.canvas.delete("all")

        if not grid_map:
            return

        num_rows: int = grid_map.getRowCount()
        num_cols: int = grid_map.getColCount()

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

        for row in range(num_rows):
            for col in range(num_cols):
                x1 = col * cell_size
                y1 = row * cell_size
                x2 = x1 + cell_size
                y2 = y1 + cell_size

                color: str = ""
                if (
                    aircraft is not None
                    and row == aircraft.getCurRow()
                    and col == aircraft.getCurCol()
                ):
                    color = "blue"
                elif not grid_map.isTraversable(row, col):
                    color = "black"
                else:
                    color = "white"

                self.canvas.create_rectangle(
                    x1, y1, x2, y2, fill=color, outline="gray"
                )  # Draw the grid cell

        # resize scroll region
        self.canvas.config(scrollregion=self.canvas.bbox("all"))

    def place_aircraft_clicked(self) -> None:
        """Handle user plane specification input."""
        row = int(self.row_entry.get())
        column = int(self.column_entry.get())
        if self.controller:
            self.controller.init_aircraft(row, column)

    def scan_button_clicked(self) -> None:
        """Start the scan when the user says so."""
        search_percentage = float(self.scan_search_percentage_entry.get())
        move_limit = int(self.scan_move_limit_entry.get())
        if self.controller:
            self.controller.scan(search_percentage, move_limit)

    def draw_route(
        self,
        grid_map: backend_binding.GridMap,
        aircraft: backend_binding.Aircraft,
        route: list[backend_binding.Moves],
        animate: bool = True,
    ) -> None:
        """Draw to router route on the canvas.

        Args:
            grid_map (backend_binding.GridMap): GridMap object being scanned.
            aircraft (backend_binding.Aircraft): Aircraft object scanning the
                GridMap.
            route (list[backend_binding.Moves]): Router object scheduling the
                scanning route.
            animate (bool, optional): Will delay drawing each step of the
                route in order to create an animation. Defaults to True.
        """
        if not grid_map:
            return
        if not aircraft:
            return
        if not route:
            return

        self.canvas.delete("all")
        self.display_map(grid_map=grid_map, aircraft=aircraft)

        num_rows: int = grid_map.getRowCount()
        num_cols: int = grid_map.getColCount()

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

        # aircraft initial position
        aircraft_row: int = aircraft.getCurRow()
        aircraft_col: int = aircraft.getCurCol()

        start_row: int = aircraft_row
        start_col: int = aircraft_col

        direction: str = aircraft.getDir()

        moves: list[tuple[int, int]] = []
        for move in route:
            moves.append((aircraft_row, aircraft_col))

            x1 = aircraft_col * cell_size
            y1 = aircraft_row * cell_size
            x2 = x1 + cell_size
            y2 = y1 + cell_size

            if move == backend_binding.Moves.FORWARD:
                if direction == backend_binding.Direction.NORTH:
                    aircraft_row -= 1
                elif direction == backend_binding.Direction.EAST:
                    aircraft_col += 1
                elif direction == backend_binding.Direction.SOUTH:
                    aircraft_row += 1
                elif direction == backend_binding.Direction.WEST:
                    aircraft_col -= 1
            elif move == backend_binding.Moves.TURNLEFT:
                if direction == backend_binding.Direction.NORTH:
                    direction = backend_binding.Direction.WEST
                elif direction == backend_binding.Direction.EAST:
                    direction = backend_binding.Direction.NORTH
                elif direction == backend_binding.Direction.SOUTH:
                    direction = backend_binding.Direction.EAST
                elif direction == backend_binding.Direction.WEST:
                    direction = backend_binding.Direction.SOUTH
            elif move == backend_binding.Moves.TURNRIGHT:
                if direction == backend_binding.Direction.NORTH:
                    direction = backend_binding.Direction.EAST
                elif direction == backend_binding.Direction.EAST:
                    direction = backend_binding.Direction.SOUTH
                elif direction == backend_binding.Direction.SOUTH:
                    direction = backend_binding.Direction.WEST
                elif direction == backend_binding.Direction.WEST:
                    direction = backend_binding.Direction.NORTH

            color: str = "red"

            if aircraft_row == start_row and aircraft_col == start_col:
                print(aircraft_row, aircraft_col, direction)
                color = "blue"
                # continue  # do not draw over starting position.

            self.canvas.create_rectangle(
                x1, y1, x2, y2, fill=color, outline="gray"
            )  # Draw the grid cell

            row_scanned_range: list[int] | range = []
            col_scanned_range: list[int] | range = []

            if direction == backend_binding.Direction.NORTH:
                row_scanned_range = range(aircraft_row - 2, aircraft_row)
                col_scanned_range = range(aircraft_col - 1, aircraft_col + 2)
            elif direction == backend_binding.Direction.SOUTH:
                row_scanned_range = range(aircraft_row + 1, aircraft_row + 3)
                col_scanned_range = range(aircraft_col - 1, aircraft_col + 2)
            elif direction == backend_binding.Direction.EAST:
                row_scanned_range = range(aircraft_row - 1, aircraft_row + 2)
                col_scanned_range = range(aircraft_col + 1, aircraft_col + 3)
            elif direction == backend_binding.Direction.WEST:
                row_scanned_range = range(aircraft_row - 1, aircraft_row + 2)
                col_scanned_range = range(aircraft_col - 2, aircraft_col)

            for row in row_scanned_range:
                for col in col_scanned_range:
                    if (
                        grid_map.isWithinBounds(row, col)
                        and grid_map.isTraversable(row, col)
                        and ((row, col) not in moves)
                    ):
                        x1 = col * cell_size
                        y1 = row * cell_size
                        x2 = x1 + cell_size
                        y2 = y1 + cell_size

                        self.canvas.create_rectangle(
                            x1, y1, x2, y2, fill="yellow", outline="grey"
                        )

            if animate:
                self.canvas.update()
                time.sleep(0.005)

        # resize scroll region
        self.canvas.config(scrollregion=self.canvas.bbox("all"))
