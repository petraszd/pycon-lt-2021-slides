extends Node


signal after_python_exec(output)


func _on_RunButton_pressed():
    var python_code  = $PyTextEdit.text
    emit_signal("after_python_exec", PythonRunner.run_code(python_code))
