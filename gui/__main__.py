"""Main for module.

To run the module execute: `python -m gui`, in the project root dir.
"""

from .app import App

if __name__ == "__main__":
    app = App()
    app.mainloop()
