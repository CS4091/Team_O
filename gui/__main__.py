"""Main for module.

To run the module execute: `python -m gui`, in the project root dir.
"""

#import backend_binding

#from .model import Model
from .app import App


if __name__ == "__main__":
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

    app = App()
    app.mainloop()
