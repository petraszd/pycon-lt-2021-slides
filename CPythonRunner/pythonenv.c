#include <string.h>
#include <Python.h>

#include "pythonenv.h"

static char stdout_buffer[2048 + 1];
static size_t stdout_buffer_current_size = 0;

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

    size_t stdout_available_space = 2048 - stdout_buffer_current_size;
    strncpy(
            stdout_buffer + stdout_buffer_current_size,
            utf8_str,
            n_utf8_str > stdout_available_space ? stdout_available_space: n_utf8_str);
    stdout_buffer[2048] = '\0';
    stdout_buffer_current_size += n_utf8_str;

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


// pyenv_* functions
// -----------------

pyenv_init_result_e pyenv_init(char* error, size_t max_error_length)
{
    wchar_t *program_name = Py_DecodeLocale("Slides", NULL);
    if (program_name == NULL) {
        return PYENV_INIT_CAN_NOT_DECODE_PROGRAM_NAME;
    }

    if (PyImport_AppendInittab("stdout_capture", &create_stdout_capture_module) == -1) {
        return PYENV_INIT_CAN_REGISTER_STDOUT_MODULE;
    }

    Py_Initialize();
    PyRun_SimpleString(
            "import sys\n"
            "import stdout_capture\n"
            "sys.stdout = stdout_capture\n"
            );

    return PYENV_INIT_OK;
}

void pyenv_deinit()
{
    Py_Finalize();
    /*if (Py_FinalizeEx() < 0) {*/
        /*log_error_and_exit("Can not finalize");*/
    /*}*/
}

void pyenv_update_buffer(char* buffer, size_t n_buffer)
{
    PyRun_SimpleString(
            "from datetime import datetime\n"
            "print(datetime.now().isoformat())\n"
            "for i in range(10):\n"
            "    print(f'{i}. Hello Sailor')\n"
            );

    strncpy(buffer, stdout_buffer, n_buffer);
    buffer[n_buffer - 1] = '\0';
}

