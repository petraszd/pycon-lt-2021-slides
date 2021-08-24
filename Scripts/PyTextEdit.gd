extends TextEdit


export var py_filename = ""


func _ready():
    if py_filename != "":
        text = Util.get_file_content(py_filename)
