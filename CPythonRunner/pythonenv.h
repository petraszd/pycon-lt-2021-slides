#ifndef PZ_GODOT_NSCRIPT_PYTHONENV
#define PZ_GODOT_NSCRIPT_PYTHONENV 1

#include <stdlib.h>


typedef enum pyenv_init_result {
    PYENV_INIT_OK = 0,
    PYENV_INIT_CAN_NOT_DECODE_PROGRAM_NAME = 1,
    PYENV_INIT_CAN_REGISTER_STDOUT_MODULE = 2,
} pyenv_init_result_e;

pyenv_init_result_e pyenv_init();
void pyenv_deinit();
void pyenv_update_buffer(char* buffer, size_t n_buffer);

#endif /* ifndef PZ_GODOT_NSCRIPT_PYTHONENV */

