extends TextEdit


export var c_filename = ""

export var color_string = Color(0.0, 0.0, 0.0)
export var color_type = Color(0.0, 0.0, 0.0)
export var color_keyword = Color(0.0, 0.0, 0.0)
export var color_flow = Color(0.0, 0.0, 0.0)


func _ready():
    text = Util.get_file_content(c_filename)

    add_color_region('"', '"', color_string)

    add_keyword_color("int", color_type)
    add_keyword_color("char", color_type)
    add_keyword_color("int8_t", color_type)
    add_keyword_color("int16_t", color_type)
    add_keyword_color("int32_t", color_type)
    add_keyword_color("int64_t", color_type)

    add_keyword_color("return", color_keyword)
    add_color_region("#", ">", color_keyword)

    add_keyword_color("if", color_flow)
    add_keyword_color("else", color_flow)
    add_keyword_color("while", color_flow)



func _unhandled_input(event):
    if has_focus() and event.is_action_released("ui_cancel"):
        release_focus()
