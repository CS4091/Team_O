#include "aircraft.h"
#include "map.h"
#include "router1.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(backend_binding, m) {
  m.doc() = "";

  py::class_<Cell>(m, "Cell")
      .def(py::init<>())
      .def_readwrite("traversable", &Cell::traversable)
      .def_readwrite("scanned", &Cell::scanned)
      .def_readwrite("colored", &Cell::colored);

  py::class_<GridMap>(m, "GridMap")
      .def(py::init<int, std::string>(), py::arg("width"), py::arg("filePath"))
      .def("isTraversable", &GridMap::isTraversable)
      .def("isScanned", &GridMap::isScanned)
      .def("markScanned", &GridMap::markScanned)
      .def("markColored", &GridMap::markColored)
      .def("markUntraversable", &GridMap::markUntraversable)
      .def("isColored", &GridMap::isColored)
      .def("isWithinBounds", &GridMap::isWithinBounds)
      .def("printer", &GridMap::printer)
      .def("mapStats", &GridMap::mapStats)
      .def("getRowCount", &GridMap::getRowCount)
      .def("getColCount", &GridMap::getColCount)
      .def("getCell", &GridMap::getCell)
      .def("getTraversableCount", &GridMap::getTraversableCount);

  py::enum_<Direction>(m, "Direction")
      .value("NORTH", Direction::dir_NORTH)
      .value("SOUTH", Direction::dir_SOUTH)
      .value("EAST", Direction::dir_EAST)
      .value("WEST", Direction::dir_WEST)
      .export_values();

  py::class_<Aircraft>(m, "Aircraft")
      .def(py::init<int, Direction, int, GridMap>(), py::arg("startRow"),
           py::arg("startDir"), py::arg("startCol"), py::arg("map"))
      .def("moveForward", &Aircraft::moveForward)
      .def("turnLeft", &Aircraft::turnLeft)
      .def("turnRight", &Aircraft::turnRight)
      .def("scan", &Aircraft::scan)
      .def("getCurRow", &Aircraft::getCurRow)
      .def("getCurCol", &Aircraft::getCurCol)
      .def("getDir", &Aircraft::getDir)
      .def("getMap", &Aircraft::getMap);

  py::enum_<Moves>(m, "Moves")
      .value("FORWARD", Moves::move_FORWARD)
      .value("TURNLEFT", Moves::move_TURNLEFT)
      .value("TURNRIGHT", Moves::move_TURNRIGHT)
      .export_values();

  py::class_<RoutePlanner>(m, "RoutePlanner")
      .def(py::init<Aircraft, float, int>(), py::arg("aircraft"),
           py::arg("searchPercentage"), py::arg("moveLimit"))
      .def("findRoute", &RoutePlanner::findRoute)
      .def("findNearestUnscannedPosRow",
           &RoutePlanner::findNearestUnscannedPosRow)
      .def("findNearestUnscannedPosCol",
           &RoutePlanner::findNearestUnscannedPosCol)
      .def("getSearchPercentage", &RoutePlanner::getSearchPercentage);
}
