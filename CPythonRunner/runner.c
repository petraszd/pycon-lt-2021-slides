#include <stdio.h>
#include <assert.h>

#include "gdnative/array.h"
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

    godot_method_attributes attrs = { GODOT_METHOD_RPC_MODE_DISABLED };
    /* run_code */
    godot_instance_method run_code = {
        .method = &runner_run_code,
        .method_data = NULL,
        .free_func = NULL,
    };
    nativescript_api->godot_nativescript_register_method(
            p_handle,
            CLASS_NAME,
            "run_code",
            attrs,
            run_code);
    /* get_int_structure */
    godot_instance_method get_int_structure = {
        .method = &runner_get_int_structure,
        .method_data = NULL,
        .free_func = NULL,
    };
    nativescript_api->godot_nativescript_register_method(
            p_handle,
            CLASS_NAME,
            "get_int_structure",
            attrs,
            get_int_structure);
}

void *runner_constructor(godot_object *p_instance, void *p_method_data)
{
    pyenv_init(); // TODO: check error code
    runner_data_t *data = core_api->godot_alloc(sizeof(runner_data_t)); // TODO: check if not NULL;
    data->buffer[0] = '\0';

    core_api->godot_string_new(&data->temp_out);
    core_api->godot_string_new(&data->temp_err);

    return (void*) data;
}

void runner_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    pyenv_deinit();

    runner_data_t *data = (runner_data_t*)(p_user_data);

    core_api->godot_string_destroy(&data->temp_out);
    core_api->godot_string_destroy(&data->temp_err);

    core_api->godot_free(p_user_data);
}

godot_variant runner_run_code(
        godot_object *p_instance,
        void *p_method_data,
        void *p_user_data,
        int p_num_args,
        godot_variant **p_args)
{
    assert(p_num_args == 1);

    godot_variant ret;

    runner_data_t *data = (runner_data_t*)p_user_data;

    godot_string arg = core_api->godot_variant_as_string(p_args[0]);
    godot_char_string c_str = core_api->godot_string_utf8(&arg);
    const char *code = core_api->godot_char_string_get_data(&c_str);

    pyenv_run_code(code);
    core_api->godot_char_string_destroy(&c_str);

    pyenv_flush_stdout(data->buffer, BUFFER_SIZE);
    core_api->godot_string_parse_utf8(&data->temp_out, data->buffer);

    pyenv_flush_stderr(data->buffer, BUFFER_SIZE);
    core_api->godot_string_parse_utf8(&data->temp_err, data->buffer);

    godot_array pipes;
    core_api->godot_array_new(&pipes);

    godot_variant pipes_item;

    core_api->godot_variant_new_string(&pipes_item, &data->temp_out);
    core_api->godot_array_push_back(&pipes, &pipes_item);
    core_api->godot_variant_destroy(&pipes_item);

    core_api->godot_variant_new_string(&pipes_item, &data->temp_err);
    core_api->godot_array_push_back(&pipes, &pipes_item);
    core_api->godot_variant_destroy(&pipes_item);

    core_api->godot_variant_new_array(&ret, &pipes);

    return ret;
}

godot_variant runner_get_int_structure(
        godot_object *p_instance,
        void *p_method_data,
        void *p_user_data,
        int p_num_args,
        godot_variant **p_args)
{
    godot_variant ret;

    runner_data_t *data = (runner_data_t*)p_user_data;

    pyenv_get_int_structure(data->buffer, BUFFER_SIZE);
    /* Using temp_out as temp var */
    core_api->godot_string_parse_utf8(&data->temp_out, data->buffer);

    core_api->godot_variant_new_string(&ret, &data->temp_out);

    return ret;
}

