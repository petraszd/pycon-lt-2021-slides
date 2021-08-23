extends Label


export var out_filename = ""


func _ready():
    text = Util.get_file_content(out_filename)
