class_name Util
extends Object


static func get_file_content(filename):
    var file = File.new()
    file.open("res://CodeSamples/" + filename, File.READ)
    var ret = file.get_as_text()
    file.close()

    ret = ret.replacen("\t", "")

    return ret.rstrip("\n")
