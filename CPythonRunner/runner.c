#include <stdio.h>

#include "pythonenv.h"
#include "runner.h"

#define CLASS_NAME "C_PYTHON_RUNNER"

const godot_gdnative_core_api_struct *core_api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;


void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options)
{
    core_api = p_options->api_struct;
    for (size_t i = 0; i < core_api->num_extensions; ++i) {
        switch (core_api->extensions[i]->type) {
            case GDNATIVE_EXT_NATIVESCRIPT:
                nativescript_api = (godot_gdnative_ext_nativescript_api_struct*)core_api->extensions[i];
                break;
            default:
                break;
        }
    }
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options)
{
    core_api = NULL;
    nativescript_api = NULL;
}

void GDN_EXPORT godot_nativescript_init(void *p_handle)
{
    printf("HELLO SAILOR\n");
    godot_instance_create_func create = {
        .create_func = &runner_constructor,
        .free_func = NULL,
        .method_data = NULL,
    };

    godot_instance_destroy_func destroy = {
        .destroy_func = &runner_destructor,
        .free_func = NULL,
        .method_data = NULL,
    };

    nativescript_api->godot_nativescript_register_class(
            p_handle,
            CLASS_NAME,
            "Reference",
            create,
            destroy);

    godot_instance_method get_output = {
        .method = &runner_get_output,
        .method_data = NULL,
        .free_func = NULL,
    };
    godot_method_attributes attrs = { GODOT_METHOD_RPC_MODE_DISABLED };
    nativescript_api->godot_nativescript_register_method(
            p_handle,
            CLASS_NAME,
            "get_output",
            attrs,
            get_output);
}

void *runner_constructor(godot_object *p_instance, void *p_method_data)
{
    pyenv_init();
    // TODO: check error code
    runner_data_t *data = core_api->godot_alloc(sizeof(runner_data_t));
    // TODO: check if not NULL;
    return (void*) data;
}

void runner_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    pyenv_deinit();

    core_api->godot_free(p_user_data);
}

godot_variant runner_get_output(
        godot_object *p_instance,
        void *p_method_data,
        void *p_user_data,
        int p_num_args,
        godot_variant **p_args)
{
    runner_data_t *data = (runner_data_t*)p_user_data;
    pyenv_update_buffer(data->buffer, 2048);

    godot_string output;
    core_api->godot_string_new(&output);

    godot_variant ret;
    core_api->godot_string_parse_utf8(&output, data->buffer);
    core_api->godot_variant_new_string(&ret, &output);

    core_api->godot_string_destroy(&output);

    return ret;
}

