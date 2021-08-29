extends Node


func _on_PyTextEditWithRunBtn_after_python_exec(stdout, stderr):
    var combined = PoolStringArray()
    if stdout != "":
        combined.append(stdout)
    if stderr != "":
        combined.append(stderr)
    $PythonOutput.set_text(combined.join("\n"))
