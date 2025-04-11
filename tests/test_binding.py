import os
import sys

current_dir: str = os.path.dirname(os.path.abspath(__file__))
root_dir: str = os.path.dirname(current_dir)
backend_dir: str = os.path.join(root_dir, "backend")
backend_build_dir: str = os.path.join(backend_dir, "build")
sys.path.append(backend_build_dir)

import backend_binding  # noqa: E402

example_map_path: str = "../backend/unit_tests/test_csv/mediumTestGrid.csv"

with open(example_map_path, mode="r") as map_csv:
    for i, line in enumerate(map_csv.readlines()):
        print(i, len(line.split(sep=",")))

# example_map = backend_binding.GridMap(width=3, filePath=example_map_path)
print("FFFFFs")
example_cell = backend_binding.GridMap(width=50, filePath=example_map_path)
