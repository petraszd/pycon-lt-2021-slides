#include <string.h>
#include <Python.h>

#include "pythonenv.h"


#define STDOUT_BUFFER_SIZE 4096 /* 4 MB */

static char stdout_buffer[STDOUT_BUFFER_SIZE + 1]; /* + 1 byte for '\0' */
static size_t stdout_buffer_idx = 0;

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

    size_t stdout_available_space = STDOUT_BUFFER_SIZE - stdout_buffer_idx;
    strncpy(
            stdout_buffer + stdout_buffer_idx,
            utf8_str,
            n_utf8_str > stdout_available_space ? stdout_available_space: n_utf8_str);
    stdout_buffer[STDOUT_BUFFER_SIZE] = '\0';
    stdout_buffer_idx += n_utf8_str;

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

pyenv_init_result_e pyenv_init()
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
            "sys.stderr = stdout_capture\n"
            );

    return PYENV_INIT_OK;
}

void pyenv_deinit()
{
    Py_Finalize();
    /* Py_Finalize returns error on failure. But who cares. I am quiting anyway. */
}

void pyenv_run_code(const char* input, size_t n_input, char* output, size_t n_output)
{
    PyRun_SimpleString(input);
    size_t n = stdout_buffer_idx > n_output ? n_output : stdout_buffer_idx;
    strncpy(output, stdout_buffer, n);
    output[n] = '\0'; /* It is assumes that output size is n_output + 1 */
    if (stdout_buffer_idx == 0) {
        output[0] = '\0';
    }
    stdout_buffer[0] = '\0';
    stdout_buffer_idx = 0;
}

