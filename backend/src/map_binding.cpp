#include <pybind11/pybind11.h>
#include "map.h"

namespace py = pybind11;

PYBIND11_MODULE(map_binding, m) {
    m.doc() = "";

    py::class_<Cell>(m, "Cell")
        .def(py::init<>())
        .def_readwrite("traversable", &Cell::traversable)
        .def_readwrite("scanned", &Cell::scanned);

    py::class_<GridMap>(m, "GridMap")
        .def(py::init<int, std::string>(), py::arg("width"), py::arg("filePath"))
        .def("isTraversable", &GridMap::isTraversable, py::arg("row"), py::arg("column"))
        .def("isScanned", &GridMap::isScanned, py::arg("row"), py::arg("column"))
        .def("markScanned", &GridMap::markScanned, py::arg("row"), py::arg("column"))
        .def("printer", &GridMap::printer)
        .def("getRowCount", &GridMap::getRowCount)
        .def("getColCount", &GridMap::getColCount)
        .def("getCell", &GridMap::getCell, py::arg("row"), py::arg("column"));
}