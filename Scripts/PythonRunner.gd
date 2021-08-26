extends Node


onready var _runner = preload("res://NativeScripts/CPythonRunner.gdns").new()


func run_code(python_code):
    var output = _runner.get_output(python_code)
    print("RUNNING")
    print(output)
