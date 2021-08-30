#include <string.h>
#include <Python.h>

#include "pythonenv.h"


#define PIPE_SIZE 2048 /* 2 MB */
#define INT_STRUCT_SIZE 512 /* 0.5 MB */


/* It should be possible to pass Godot object and not use static global objects
 * But I have no time for that. So globals rules!!!
 */
static char stdout_pipe[PIPE_SIZE + 1]; /* + 1 byte for '\0' */
static size_t stdout_pipe_idx = 0;

static char stderr_pipe[PIPE_SIZE + 1]; /* + 1 byte for '\0' */
static size_t stderr_pipe_idx = 0;

static char int_struct[INT_STRUCT_SIZE + 1];
static size_t int_struct_idx = 0;


/* stdout_capture functions */
/* ------------------------ */
static PyObject* stdout_write(PyObject* mod, PyObject* args)
{
    Py_ssize_t args_size = PyTuple_Size(args);
    if (args_size == 0) {
        return Py_None;
    }

    PyObject *py_str = PyTuple_GetItem(args, 0);
    Py_INCREF(py_str);
    Py_ssize_t n_py_str = PyUnicode_GetLength(py_str);

    Py_ssize_t n_utf8_str;
    // According to docs there is no need to free utf8_str
    const char *utf8_str = PyUnicode_AsUTF8AndSize(py_str, &n_utf8_str);
    Py_DECREF(py_str);

    assert(n_utf8_str > 0);
    assert(utf8_str != NULL);
    assert(n_utf8_str >= n_py_str);

    size_t stdout_available_space = PIPE_SIZE - stdout_pipe_idx;
    strncpy(
            stdout_pipe + stdout_pipe_idx,
            utf8_str,
            n_utf8_str > stdout_available_space ? stdout_available_space: n_utf8_str);
    stdout_pipe[PIPE_SIZE] = '\0';
    stdout_pipe_idx += n_utf8_str;

    return Py_None;
}

static PyObject* stdout_flush(PyObject* mod, PyObject* args)
{
    return Py_None;
}

static PyMethodDef stdout_capture_methods[] =
{
    {
        .ml_name  = "write",
        .ml_meth  = &stdout_write,
        .ml_flags = METH_VARARGS,
        .ml_doc   = NULL,
    },
    {
        .ml_name  = "flush",
        .ml_meth  = &stdout_flush,
        .ml_flags = METH_VARARGS,
        .ml_doc   = NULL,
    },
    {
        .ml_name  = NULL,
        .ml_meth  = NULL,
        .ml_flags = 0,
        .ml_doc   = NULL,
    }
};

static PyModuleDef stdout_capture_module = {
    .m_base     = PyModuleDef_HEAD_INIT,
    .m_name     = "stdout_capture",
    .m_doc      = NULL,
    .m_size     = -1,
    .m_methods  = stdout_capture_methods,
    .m_slots    = NULL,
    .m_traverse = NULL,
    .m_clear    = NULL,
    .m_free     = NULL,
};

PyObject* create_stdout_capture_module()
{
    return PyModule_Create(&stdout_capture_module);
}


/* stderr_capture functions */
/* ------------------------ */
static PyObject* stderr_write(PyObject* mod, PyObject* args)
{
    Py_ssize_t args_size = PyTuple_Size(args);
    if (args_size == 0) {
        return Py_None;
    }

    PyObject *py_str = PyTuple_GetItem(args, 0);
    Py_INCREF(py_str);
    Py_ssize_t n_py_str = PyUnicode_GetLength(py_str);

    Py_ssize_t n_utf8_str;
    // According to docs there is no need to free utf8_str
    const char *utf8_str = PyUnicode_AsUTF8AndSize(py_str, &n_utf8_str);
    Py_DECREF(py_str);

    assert(n_utf8_str > 0);
    assert(utf8_str != NULL);
    assert(n_utf8_str >= n_py_str);

    size_t stderr_available_space = PIPE_SIZE - stderr_pipe_idx;
    strncpy(
            stderr_pipe + stderr_pipe_idx,
            utf8_str,
            n_utf8_str > stderr_available_space ? stderr_available_space: n_utf8_str);
    stderr_pipe[PIPE_SIZE] = '\0';
    stderr_pipe_idx += n_utf8_str;

    return Py_None;
}

static PyObject* stderr_flush(PyObject* mod, PyObject* args)
{
    return Py_None;
}

static PyMethodDef stderr_capture_methods[] =
{
    {
        .ml_name  = "write",
        .ml_meth  = &stderr_write,
        .ml_flags = METH_VARARGS,
        .ml_doc   = NULL,
    },
    {
        .ml_name  = "flush",
        .ml_meth  = &stderr_flush,
        .ml_flags = METH_VARARGS,
        .ml_doc   = NULL,
    },
    {
        .ml_name  = NULL,
        .ml_meth  = NULL,
        .ml_flags = 0,
        .ml_doc   = NULL,
    }
};

static PyModuleDef stderr_capture_module = {
    .m_base     = PyModuleDef_HEAD_INIT,
    .m_name     = "stderr_capture",
    .m_doc      = NULL,
    .m_size     = -1,
    .m_methods  = stderr_capture_methods,
    .m_slots    = NULL,
    .m_traverse = NULL,
    .m_clear    = NULL,
    .m_free     = NULL,
};

PyObject* create_stderr_capture_module()
{
    return PyModule_Create(&stderr_capture_module);
}


/* pzint functions */
/* --------------- */
static PyObject* pzint_inspect(PyObject* mod, PyObject* args)
{
    /* Must be one arg */
    Py_ssize_t args_size = PyTuple_Size(args);
    if (args_size != 1) {
        PyErr_SetString(PyExc_TypeError, "inspect() requires 1 argument.");
        return Py_None;
    }

    /* It must be an long */
    PyObject* raw_number = PyTuple_GetItem(args, 0);
    if (raw_number->ob_type != &PyLong_Type) {
        PyErr_SetString(PyExc_TypeError, "inspect() argument 1 must be int.");
        return Py_None;
    }

    PyLongObject* number = (PyLongObject*)raw_number;

    int_struct[0] = '\0';
    int_struct_idx = 0;

    int n_written;
    n_written = snprintf(
            int_struct, INT_STRUCT_SIZE,
            "%ld,%ld",
            number->ob_base.ob_base.ob_refcnt,
            number->ob_base.ob_size);

    if (n_written <= 0) {
        return Py_None;
    }
    int_struct_idx = n_written;

    int real_size = number->ob_base.ob_size > 0 ? number->ob_base.ob_size : -number->ob_base.ob_size;
    for (size_t i = 0; i < real_size; ++i) {
        if (int_struct_idx >= INT_STRUCT_SIZE) {
            break;
        }
        n_written = snprintf(
                int_struct + int_struct_idx, INT_STRUCT_SIZE - int_struct_idx,
                ",%d",
                number->ob_digit[i]);
        if (n_written <= 0) {
            break;
        }
        int_struct_idx += n_written;
    }

    int_struct[INT_STRUCT_SIZE] = '\0';
    return Py_None;
}

static PyObject* pzint_get_parts(PyObject* mod, PyObject* args)
{
    /* Must be one arg */
    Py_ssize_t args_size = PyTuple_Size(args);
    if (args_size != 1) {
        PyErr_SetString(PyExc_TypeError, "get_parts() requires 1 argument.");
        return Py_None;
    }

    /* It must be an long */
    PyObject* raw_number = PyTuple_GetItem(args, 0);
    if (raw_number->ob_type != &PyLong_Type) {
        PyErr_SetString(PyExc_TypeError, "get_parts() argument 1 must be int.");
        return Py_None;
    }

    PyLongObject* number = (PyLongObject*)raw_number;
    int real_size = number->ob_base.ob_size > 0 ? number->ob_base.ob_size : -number->ob_base.ob_size;

    PyObject* ret = PyList_New(real_size);
    PyObject* part;
    for (size_t i = 0; i < real_size; ++i) {
        part = PyLong_FromLong(number->ob_digit[i]);
        PyList_SetItem(ret, i, part);
    }
    return ret;
}

static PyMethodDef pzint_methods[] =
{
    {
        .ml_name  = "inspect",
        .ml_meth  = &pzint_inspect,
        .ml_flags = METH_VARARGS,
        .ml_doc   = NULL,
    },
    {
        .ml_name  = "get_parts",
        .ml_meth  = &pzint_get_parts,
        .ml_flags = METH_VARARGS,
        .ml_doc   = NULL,
    },
    {
        .ml_name  = NULL,
        .ml_meth  = NULL,
        .ml_flags = 0,
        .ml_doc   = NULL,
    }
};

static PyModuleDef pzint_module = {
    .m_base     = PyModuleDef_HEAD_INIT,
    .m_name     = "pzint",
    .m_doc      = NULL,
    .m_size     = -1,
    .m_methods  = pzint_methods,
    .m_slots    = NULL,
    .m_traverse = NULL,
    .m_clear    = NULL,
    .m_free     = NULL,
};

PyObject* create_pzint_module()
{
    return PyModule_Create(&pzint_module);
}


/* pyenv_* functions */
/* ----------------- */
pyenv_init_result_e pyenv_init()
{
    wchar_t *program_name = Py_DecodeLocale("Slides", NULL);
    if (program_name == NULL) {
        return PYENV_INIT_CAN_NOT_DECODE_PROGRAM_NAME;
    }

    if (PyImport_AppendInittab("stdout_capture", &create_stdout_capture_module) == -1) {
        return PYENV_INIT_CAN_NOT_REGISTER_MODULE;
    }
    if (PyImport_AppendInittab("stderr_capture", &create_stderr_capture_module) == -1) {
        return PYENV_INIT_CAN_NOT_REGISTER_MODULE;
    }
    if (PyImport_AppendInittab("pzint", &create_pzint_module) == -1) {
        return PYENV_INIT_CAN_NOT_REGISTER_MODULE;
    }

    Py_Initialize();
    PyRun_SimpleString(
            "import sys\n"
            "import stdout_capture\n"
            "sys.stdout = stdout_capture\n"
            "import stderr_capture\n"
            "sys.stderr = stderr_capture\n"
            "import pzint\n"
            );

    return PYENV_INIT_OK;
}

void pyenv_deinit()
{
    Py_Finalize();
    /* Py_Finalize returns error on failure. But who cares. I am quiting anyway. */
}

void pyenv_run_code(const char* input)
{
    PyRun_SimpleString(input);
}

void pyenv_flush_stdout(char* out, size_t n_out)
{
    size_t n = stdout_pipe_idx > n_out ? n_out : stdout_pipe_idx;
    strncpy(out, stdout_pipe, n);
    out[n] = '\0'; /* It is assumes that output size is n_output + 1 */
    stdout_pipe[0] = '\0';
    stdout_pipe_idx = 0;
}

void pyenv_flush_stderr(char* out, size_t n_out)
{
    size_t n = stderr_pipe_idx > n_out ? n_out : stderr_pipe_idx;
    strncpy(out, stderr_pipe, n);
    out[n] = '\0'; /* It is assumes that output size is n_output + 1 */
    stderr_pipe[0] = '\0';
    stderr_pipe_idx = 0;
}

void pyenv_get_int_structure(char* out, size_t n_out)
{
    size_t n = int_struct_idx > n_out ? n_out : int_struct_idx;
    strncpy(out, int_struct, n);
    out[n] = '\0'; /* It is assumes that output size is n_output + 1 */
}

