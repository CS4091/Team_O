"""Main for module.

To run the module execute: `python -m tests`, in the project root dir.
"""

from PIL import Image

import backend_binding

if __name__ == "__main__":
    example_map_path: str = "./backend/unit_tests/test_csv/mediumTestGrid.csv"

    grid_map = backend_binding.GridMap(width=50, filePath=example_map_path)

    aircraft = backend````````````````````_binding.Aircraft(
            startRow=9,
            startDir=backend_binding.Direction.SOUTH,
            startCol=4,
            map=grid_map,
        )
    
    router = backend_binding.RoutePlanner(aircraft)

    route = router.findRoute()

    rows: int = grid_map.getRowCount()
    cols: int = grid_map.getColCount()

    img = Image.new("RGB", (cols, rows), "black")
    pixels = img.load()
    for row in range(img.size[0]):
        for col in range(img.size[1]):
            if not grid_map.isTraversable(row, col):
                continue
            elif grid_map.isScanned(row, col):
                pixels[row, col] = (255, 0, 255)
                print("FFFF")
            else:
                pixels[row, col] = (255, 255, 255)

    aircraft_row: int = aircraft.getCurRow()
    aircraft_col: int = aircraft.getCurCol()

    pixels[aircraft_row, aircraft_col] = (0, 255, 255)
    direction: str = 'S'
    for move in route[1:]:
        if move == backend_binding.Moves.FORWARD:
            if direction == 'N':
                aircraft_row -= 1
            elif direction == 'E':
                aircraft_col += 1
            elif direction == 'S':
                aircraft_row += 1
            elif direction == 'W':
                aircraft_col -= 1
        elif move == backend_binding.Moves.TURNLEFT:
            if direction == 'N':
                direction = 'W'
            elif direction == 'E':
                direction = 'N'
            elif direction == 'S':
                direction = 'E'
            elif direction == 'W':
                direction = 'S'
        elif move == backend_binding.Moves.TURNRIGHT:
            if direction == 'N':
                direction = 'E'
            elif direction == 'E':
                direction = 'S'
            elif direction == 'S':
                direction = 'W'
            elif direction == 'W':
                direction = 'N'
        pixels[aircraft_row, aircraft_col] = (255, 0, 0)

    img = img.resize((img.width * 10, img.height * 10), resample=Image.NEAREST)
    img.show()
