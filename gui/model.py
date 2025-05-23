"""
Model module for the MVC Tkinter GUI.

This module defines the Model class, which manages the state and data
for the application, including the grid map and the routing logic.
"""

import backend_binding


from dataclasses import dataclass


@dataclass
class State:
    grid_map: backend_binding.GridMap
    aircraft: backend_binding.Aircraft
    router: backend_binding.RoutePlanner
    route: list[backend_binding.Moves]


class Model:
    """The Model for the Graphical User Interface.

    The model is for managing the states of things that the user will modify
    when interacting with the GUI.

    Attributes:
        _grid_map: private attribute of the map being traversed.
        _router: private attribute of the router used to solve the map problem.
    """

    def __init__(self) -> None:
        """Construct of the Model component."""
        self._grid_map = None
        self._aircraft = None
        self._router = None

        self.route: list[backend_binding.Moves] = []

        self.states: list[State] = []
        self.current_state: int = 0

    @property
    def grid_map(self) -> backend_binding.GridMap:
        """Get the Grid-Map of the environment being traversed.

        Returns:
            backend_binding.GridMap: Grid-map implementation from the C++
                backend. Done via PyBind11.
        """
        return self._grid_map

    @grid_map.setter
    def grid_map(self, new_grid_map: backend_binding.GridMap) -> None:
        """Set the Grid-Map.

        Args:
            new_grid_map (backend_binding.GridMap): The new Grid-Map for the
            model.
        """
        self.route = []
        del self.aircraft
        self._grid_map = new_grid_map

    @grid_map.deleter
    def grid_map(self) -> None:
        """Delete the Grid-Map.

        TODO: Fully implement if needed.
        """
        # TODO - add more detail as needed
        self.route = []
        self._grid_map = None
        del self.aircraft
        del self.router

    @property
    def aircraft(self) -> backend_binding.Aircraft:
        """Get the Aircraft that will be traversing the environment.

        Returns:
            backend_binding.Aircraft: Aircraft implementation from the C++
                backend. Done via PyBind11.
        """
        return self._aircraft

    @aircraft.setter
    def aircraft(self, new_aircraft: backend_binding.Aircraft) -> None:
        """Set the Aircraft.

        Args:
            new_aircraft (backend_binding.Aircraft): The new Aircraft for the
            model.
        """
        self.route = []
        del self.router
        self._aircraft = new_aircraft

    @aircraft.deleter
    def aircraft(self) -> None:
        """Delete the Aircraft.

        TODO: Fully implement if needed.
        """
        # TODO - add more detail as needed
        self.route = []
        self._aircraft = None
        del self.router

    @property
    def router(self) -> backend_binding.RoutePlanner:
        """Get the Route Planner for the Grid-Map.

        Returns:
            backend_binding.RoutePlanner: Route Planner implementation from
            the C++ backend. Done via PyBind11.
        """
        return self._router

    @router.setter
    def router(self, new_router: backend_binding.RoutePlanner) -> None:
        """Set the Route Planner.

        Args:
            new_router (backend_binding.RoutePlanner): The new Route Planner
            for the model.
        """
        self.route = []
        self._router = new_router

    @router.deleter
    def router(self) -> None:
        """Delete the Route Planner.

        TODO: Fully implement if needed.
        """
        # TODO - add more detail as needed
        self.route = []
        self._router = None

    def save(self) -> None:
        """Saves the current state."""
        new_state: State = State(
            self.grid_map, self.aircraft, self.router, self.route
        )
        if new_state not in self.states:
            self.states.append(new_state)
            self.current_state += 1

    def update_from_previous_state(self, state_index: int) -> None:
        if not self.states:
            return

        if state_index >= len(self.states) or state_index < 0:
            raise ValueError(f"Invalid State Index: {state_index}")

        self.grid_map = self.states[state_index].grid_map
        self.aircraft = self.states[state_index].aircraft
        self.router = self.states[state_index].router
        self.route = self.states[state_index].route
