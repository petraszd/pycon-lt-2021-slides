extends Node


func _on_RunButton_pressed():
    var python_code = $VBoxContainer/PyTextEdit.text
    $PythonOutput.set_text(PythonRunner.run_code(python_code))
