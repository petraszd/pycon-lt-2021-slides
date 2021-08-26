extends Label


export var out_filename = ""


func _ready():
    text = Utils.get_file_content(out_filename)
