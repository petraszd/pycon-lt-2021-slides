#ifndef PZ_GODOT_NSCRIPT_RUNNER
#define PZ_GODOT_NSCRIPT_RUNNER 1

#include <gdnative_api_struct.gen.h>


void *runner_constructor(godot_object *p_instance, void *p_method_data);
void runner_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);
godot_variant runner_run_code(
        godot_object *p_instance,
        void *p_method_data,
        void *p_user_data,
        int p_num_args,
        godot_variant **p_args);


#define OUTPUT_BUFFER_SIZE 4096

typedef struct runner_data_t {
    godot_string temp;

    char output[OUTPUT_BUFFER_SIZE + 1];
} runner_data_t;


#endif /* ifndef PZ_GODOT_NSCRIPT_RUNNER */

