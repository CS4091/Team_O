"""Model for the MVC Tkinter GUI."""

#import backend_binding
import csv


class Model:
    """The Model for the Graphical User Interface.

    The model is for managing the states of things that the user will modify
    when interacting with the GUI.

    Attributes:
        _grid_map: private attribute of the map being traversed.
        _router: private attribute of the router used to solve the map problem.
    """

    def __init__(self) -> None:
        """Initialize the instance of the Model."""
        self._grid_map = None
        self._router = None
        self.data = []

    # temp solution to show csv file until backend_binding works
    def load_csv(self, filepath):
        with open(filepath, newline='', encoding='utf-8') as csvfile:
            reader = csv.reader(csvfile)
            return list(reader)
        
    def get_data(self):
        return self.data

    # @property
    # def grid_map(self) -> backend_binding.GridMap:
    #     """Get the Grid-Map of the environment being traversed.

    #     Returns:
    #         backend_binding.GridMap: Grid-map implementation from the C++
    #             backend. Done via PyBind11.
    #     """
    #     return self._grid_map

    # @grid_map.setter
    # def grid_map(self, new_grid_map: backend_binding.GridMap) -> None:
    #     """Set the Grid-Map.

    #     Args:
    #         new_grid_map (backend_binding.GridMap): The new Grid-Map for the
    #         model.
    #     """
    #     self._grid_map = new_grid_map

    # @grid_map.deleter
    # def grid_map(self) -> None:
    #     """Delete the Grid-Map.

    #     TODO: Fully implement if needed.
    #     """
    #     # TODO - add more detail as needed
    #     del self._grid_map

    # @property
    # def router(self) -> backend_binding.RoutePlanner:
    #     """Get the Route Planner for the Grid-Map.

    #     Returns:
    #         backend_binding.RoutePlanner: Route Planner implementation from
    #         the C++ backend. Done via PyBind11.
    #     """
    #     return self._router

    # @router.setter
    # def router(self, new_router: backend_binding.RoutePlanner) -> None:
    #     """Set the Route Planner.

    #     Args:
    #         new_router (backend_binding.RoutePlanner): The new Route Planner
    #         for the model.
    #     """
    #     self._router = new_router

    # @router.deleter
    # def router(self) -> None:
    #     """Delete the Route Planner.

    #     TODO: Fully implement if needed.
    #     """
    #     # TODO - add more detail as needed
    #     del self._router

    # def save_map(self):
    #     """
    #     Save the map to a file
    #     :return:
    #     """
    #     with open("map_file.txt", "a") as f:
    #         f.write(self.map)
