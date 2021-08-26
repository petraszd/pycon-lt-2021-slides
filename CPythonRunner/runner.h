#ifndef PZ_GODOT_NSCRIPT_RUNNER
#define PZ_GODOT_NSCRIPT_RUNNER 1

#include <gdnative_api_struct.gen.h>


void *runner_constructor(godot_object *p_instance, void *p_method_data);
void runner_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);
godot_variant runner_get_output(
        godot_object *p_instance,
        void *p_method_data,
        void *p_user_data,
        int p_num_args,
        godot_variant **p_args);

typedef struct runner_data_t {
    // TODO: use godot_string
    char buffer[2048];
} runner_data_t;


#endif /* ifndef PZ_GODOT_NSCRIPT_RUNNER */

