extends Node


onready var _runner = preload("res://NativeScripts/CPythonRunner.gdns").new()


func run_code(python_code):
    return _runner.run_code(python_code)
