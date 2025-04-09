"""Main for module.

To run the module execute: `python -m tests`, in the project root dir.
"""

import backend_binding

if __name__ == "__main__":
    example_map_path: str = "./backend/unit_tests/test_csv/mediumTestGrid.csv"

    grid_map = backend_binding.GridMap(width=50, filePath=example_map_path)

    router = backend_binding.RoutePlanner(
        aircraft=backend_binding.Aircraft(
            startRow=9,
            startDir=backend_binding.Direction.SOUTH,
            startCol=4,
            map=grid_map,
        )
    )

    router.findRoute()
