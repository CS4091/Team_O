"""
Controller module for the application.

This module defines the Controller class, which manages interactions between
the Model and View components. It handles user input, file operations,
and communication with the backend logic.
"""

from __future__ import annotations

import csv
import traceback
from tkinter import filedialog as fd
from typing import TYPE_CHECKING

from .model import Model

# from tkinter.messagebox import showerror, showwarning, showinfo


if TYPE_CHECKING:
    from view import View

import backend_binding


class Controller:
    """Coordinates updates between the Model and View."""

    def __init__(self, model: Model, view: View) -> None:
        """Construct for the Controller component."""
        self.model = model
        self.view = view
        self.test_model = Model()

    def save(self) -> None:
        """
        Save the current state.
        :param map:
        :return:
        """
        try:
            if self.model.grid_map is None:
                return

            # save the model
            self.model.save()

            # show a success message
            self.view.show_success("The route has been saved!")

        except ValueError as error:
            # show an error message
            self.view.show_error(error)

    def previous_route(self) -> None:
        """Set the current state to the state before."""
        if self.model.current_state - 1 < 0:
            return
        self.model.current_state = max(0, self.model.current_state - 1)
        self.view.show_success(f"Showing State #{self.model.current_state}")
        self.model.update_from_previous_state(self.model.current_state)
        self.redraw_everything()

    def next_route(self) -> None:
        """Set the current state to the state after."""
        if self.model.current_state + 1 > len(self.model.states) - 1:
            return
        self.model.current_state = min(
            len(self.model.states) - 1, self.model.current_state + 1
        )
        self.view.show_success(f"Showing State #{self.model.current_state}")
        self.model.update_from_previous_state(self.model.current_state)
        self.redraw_everything()

    def redraw_everything(self) -> None:
        """Redraws everything for the state."""
        self.view.display_map(self.get_grid_map(), self.get_aircraft())
        if self.get_route():
            self.view.draw_route(
                self.get_grid_map(),
                self.get_aircraft(),
                self.get_route(),
                animate=False,
            )

    def get_grid_map(self) -> backend_binding.GridMap:
        """Get the Grid Map from the model."""
        return self.model.grid_map

    def get_aircraft(self) -> backend_binding.Aircraft:
        """Get the Aircraft from the model."""
        return self.model.aircraft

    def get_route(self) -> list[backend_binding.Moves]:
        return self.model.route

    def select_file(self) -> None:
        """Select a file to display."""
        file_path = fd.askopenfilename(filetypes=[("CSV files", "*.csv")])
        if not file_path:
            return

        try:
            map_width: int = 0
            with open(file_path, mode="r", encoding="utf-8") as csv_file:
                reader = csv.reader(csv_file)
                map_width = len(next(reader))
                for row in reader:
                    if len(row) != map_width:
                        raise csv.Error("Rows in map must be the same width!")
            self.model.grid_map = backend_binding.GridMap(
                width=map_width, filePath=file_path
            )
            self.view.display_map(grid_map=self.model.grid_map)
            self.view.show_success("File loaded successfully!")
        except Exception as e:
            self.view.show_error(f"Failed to load file: {e}")
            traceback.print_exc()

    def init_aircraft(self, row: int, column: int) -> None:
        """Create an aircraft according to the user input.

        Args:
            row (int): Starting row value.
            column (int): Starting column value.
        """
        if not self.model.grid_map:
            self.view.show_error("No map yet!")
            return
        if (
            row >= self.model.grid_map.getRowCount()
            or column >= self.model.grid_map.getColCount()
        ):
            self.view.show_error("Invalid Aircraft Position! (Out of bounds)")
            return
        if not self.model.grid_map.isTraversable(row, column):
            self.view.show_error("Invalid Aircraft Position!")
            return

        self.model.aircraft = backend_binding.Aircraft(
            startRow=row,
            startCol=column,
            startDir=backend_binding.Direction.SOUTH,
            map=self.model.grid_map,
        )
        self.view.display_map(
            grid_map=self.model.grid_map, aircraft=self.model.aircraft
        )
        self.view.show_success("Aircraft created successfully!")

    def scan(
        self, search_percentage: float = 0.80, move_limit: int = 100_000
    ) -> None:
        """Create a route planner and scan the map.

        Args:
            search_percentage (float, optional): The goal search coverage
                percentage for traversable cells. Defaults to 0.80.
            move_limit (int, optional): The maximum steps the algorithm can
                take. Defaults to 100_000.
        """
        if not self.model.grid_map:
            self.view.show_error("No map yet!")
            return
        if not self.model.aircraft:
            self.view.show_error("No aircraft yet!")
            return
        self.model.router = backend_binding.RoutePlanner(
            self.model.aircraft,
            searchPercentage=search_percentage,
            moveLimit=move_limit,
        )

        self.view.hide_message()
        self.view.show_success("Scan in progress...")
        self.view.canvas.delete("all")
        self.model.route = self.model.router.findRoute()

        self.view.draw_route(
            grid_map=self.model.grid_map,
            aircraft=self.model.aircraft,
            route=self.model.route,
        )

        # final_search_percentage: float = (
        #     self.model.router.getSearchPercentage()
        # )
        self.view.show_success(
            "Map scanned successfully!",
            # + f"{final_search_percentage*100:.3f}% out of "
            # + f"{search_percentage*100:.3f}% scanned.",
            timeout=100_000,
        )
