# Missouri S&T - CS 4091 - Capstone II

## Spring 2025 - Team O - Project 3

Jack Wyeth, Ethan Heckart, Matthew Lopez, Ryan Kluesner, Jimmy Hoerschgen

---

## Build / Install

1. Install the Conda environment using the `environment.yml` file in the project root.

2. Activate the Conda environment.

3. Run `build.sh` in the `\backend` directory.
    - This will require that `pybind` is installed in the Conda environment.
    - The build script will use CMake to build the backend and the bindings that will be used by the Python GUI.

4. To run the GUI, with the Conda environment installed execute the command: `python -m gui`  in the project root.

5. Profit
