#include "test.h"
#include "/Library/Frameworks/Python.framework/Versions/3.12/lib/python3.12/site-packages/pybind11/include/pybind11/pybind11.h"
#include <cstdio>

namespace py = pybind11;

Test::Test() {
    testing_var = 5;
}



void Test::hello() {
    printf("Hello test!\n");
}

Obj2Test::Obj2Test(Test* write_obj) {
    referenced_test = write_obj;
}

void Obj2Test::bye(char * name) {
    printf("Bye %s!\n",name);
}

PYBIND11_MODULE(test,m) {
    py::class_<Test>(m,"Test").def(py::init<>())
    .def("hello",&Test::hello)
    .def_readwrite("testing_var",&Test::testing_var);
    py::class_<Obj2Test>(m,"Obj2Test")
    .def(py::init<Test*>())
    .def("bye",&Obj2Test::bye)
    .def_readwrite("referenced_test",&Obj2Test::referenced_test);
}
