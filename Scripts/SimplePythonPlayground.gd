extends Node


func _on_PyTextEditWithRunBtn_after_python_exec(output):
    $PythonOutput.set_text(output)
