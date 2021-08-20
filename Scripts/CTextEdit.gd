extends TextEdit


func _unhandled_input(event):
    if has_focus() and event.is_action_released("ui_cancel"):
        release_focus()
