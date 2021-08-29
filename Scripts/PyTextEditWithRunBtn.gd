extends Node


signal after_python_exec(stdout, stderr)


func _on_RunButton_pressed():
    var python_code  = $PyTextEdit.text
    var res = PythonRunner.run_code(python_code)
    emit_signal("after_python_exec", res[0], res[1])
