extends TextEdit


export var c_filename = ""

export var color_string = Color(0.0, 0.0, 0.0)
export var color_type = Color(0.0, 0.0, 0.0)
export var color_keyword = Color(0.0, 0.0, 0.0)


func _ready():
    _set_text_from_file()

    add_color_region('"', '"', color_string)

    add_keyword_color("int", color_type)
    add_keyword_color("char", color_type)
    add_keyword_color("int8_t", color_type)
    add_keyword_color("int16_t", color_type)
    add_keyword_color("int32_t", color_type)
    add_keyword_color("int64_t", color_type)

    add_keyword_color("return", color_keyword)
    add_color_region("#", ">", color_keyword)



func _unhandled_input(event):
    if has_focus() and event.is_action_released("ui_cancel"):
        release_focus()


func _set_text_from_file():
    var file = File.new()
    file.open("res://CodeSamples/" + c_filename, File.READ)
    text = file.get_as_text()
    file.close()
