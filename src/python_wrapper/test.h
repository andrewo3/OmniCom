#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include "/Library/Frameworks/Python.framework/Versions/3.12/lib/python3.12/site-packages/pybind11/include/pybind11/pybind11.h"

class Test {
    public:
        int testing_var;
        Test();
        void hello();
    private:

};

class Obj2Test {
    public:
        Obj2Test(Test* write_obj);
        Test * referenced_test;
        void bye(char * name);
};
#endif