import sys
import os

gui_dir: str = os.path.dirname(os.path.abspath(__file__))
root_dir: str = os.path.dirname(gui_dir)
backend_dir: str = os.path.join(root_dir, "backend")
backend_build_dir: str = os.path.join(backend_dir, "build", "Debug")
sys.path.append(backend_build_dir)

import map_binding

cell = map_binding.Cell()
cell.traversable = True
cell.scanned = False
