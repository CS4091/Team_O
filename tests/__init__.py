"""Initialization Script.

Sets up system path for using the backend bindings.
"""

import os
import sys

gui_dir: str = os.path.dirname(os.path.abspath(__file__))
root_dir: str = os.path.dirname(gui_dir)
backend_dir: str = os.path.join(root_dir, "backend")
backend_build_dir: str = os.path.join(backend_dir, "build")
sys.path.append(backend_build_dir)
