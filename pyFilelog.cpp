#include "python2.7/Python.h"
#include "structmember.h"
#include "filelog.hpp"
#include <iostream>

using std::string;

typedef struct{
    PyObject_HEAD
        filelog* fl;
    PyObject* fname;
}PyFileLog;

static void
PyFileLog_dealloc(PyFileLog* self){
    Py_XDECREF(self->fname);
    delete self->fl;
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static int
PyFileLog_init(PyFileLog* self, PyObject* args){
    PyObject* fname=NULL, *tmp;
    const char* fnstr;
    if(!PyArg_ParseTuple(args, "s", &fnstr)){
        return -1;
    }
    if(fnstr){
        fname = Py_BuildValue("s", fnstr);
        tmp = self->fname;
        Py_INCREF(fname);
        self->fname = fname;
        Py_DECREF(tmp);
    }
    self->fl = new filelog(fnstr);
    return 0;
}
static PyObject*
PyFileLog_new(PyTypeObject* type, PyObject* args, PyObject* kwds){
    PyFileLog* self;
    self = (PyFileLog*) type->tp_alloc(type, 0);
    if(self!=NULL){
        self->fname = PyString_FromString("");
    }
    return (PyObject*) self;
}

static PyObject*
PyFileLog_append(PyFileLog* self, PyObject* args){
    const char* content;
    if(!PyArg_ParseTuple(args, "s", &content)){
        return NULL;
    }
    if(content){
        const string s(content);
        size_t c = self->fl->append(s);
        return Py_BuildValue("i", c);
    }
    return Py_BuildValue("i", 0);
}

static PyMemberDef  PyFileLog_members[] = {
    {"fname", T_OBJECT_EX, offsetof(PyFileLog, fname), 0, "logfile name"},
    {NULL}
};

static PyMethodDef PyFileLog_methods[] = {
    {"append", (PyCFunction)PyFileLog_append, METH_VARARGS, "append to logfile"},
    {NULL, NULL, 0, NULL}
};

static PyTypeObject PyFileLogType={
    PyVarObject_HEAD_INIT(NULL, 0)
        "filelog.FileLog",
    sizeof(PyFileLog),
    0,
    (destructor)PyFileLog_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "filelog objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyFileLog_methods,             /* tp_methods */
    PyFileLog_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyFileLog_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyFileLog_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
    PyMODINIT_FUNC
initfilelog(void)
{
    PyObject* m;

    if (PyType_Ready(&PyFileLogType) < 0){
        return;
    }

    m = Py_InitModule3("filelog", module_methods,
            "Example module that creates an extension type.");

    if (m == NULL){
        return;
    }

    Py_INCREF(&PyFileLogType);
    PyModule_AddObject(m, "FileLog", (PyObject *)&PyFileLogType);
}
