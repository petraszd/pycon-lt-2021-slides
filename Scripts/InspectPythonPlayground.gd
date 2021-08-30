extends HBoxContainer

export var default_color = Color(0, 0, 0)
export var value_color = Color(0, 0, 0)

var DEFAULT_REFCNT = "Unknown"
var DEFAULT_TYPE = "Unknown"
var DEFAULT_SIZE = "Unknown"

var refcnt_val = DEFAULT_REFCNT
var type_val = DEFAULT_TYPE
var size_val = DEFAULT_SIZE
var digit_vals = []


func _ready():
    _reset_vals()
    _redraw_int_structure()


func _on_PyTextEditWithRunBtn_after_python_exec(stdout, stderr):
    $CombinedOutput/PythonOutput.set_text(stdout + stderr)
    if stderr:
        _reset_vals()
        _redraw_int_structure()
    else:
        _update_vals(PythonRunner.get_int_structure())
        _redraw_int_structure()

        # Hack, hack, a horrible hack
        # ------------------------------------
        if digit_vals.size() == 1 and digit_vals[0] == "42":
            var mainScene = get_tree().current_scene
            if mainScene.name == "Main":
                mainScene.show_particles_and_move_to_next_slide()
        # ---------------------------


func _reset_vals():
    refcnt_val = DEFAULT_REFCNT
    type_val = DEFAULT_TYPE
    size_val = DEFAULT_SIZE
    digit_vals.clear()


func _update_vals(raw_struct):
    if raw_struct == "":
        _reset_vals()
        return

    type_val = "&PyLong_Type"  # It is always long type

    var struct_items = raw_struct.split(",")
    if struct_items.size() < 3:
        # Should not happen
        $CombinedOutput/IntStructure.bbcode_text = "ERROR PARSING LIB OUTPUT"
        return

    refcnt_val = struct_items[0]
    size_val = struct_items[1]
    digit_vals.clear()
    var i = 2
    while i < struct_items.size():
        digit_vals.append(struct_items[i])
        i += 1


func _redraw_int_structure():
    var arr = PoolStringArray();
    var col = value_color.to_html(false)
    arr.append("[color=#%s]{" % default_color.to_html(false))
    arr.append("  ob_base = {")
    arr.append("    ob_base = {")
    arr.append("      ob_refcnt = [color=#%s]%s[/color];" % [col, refcnt_val])
    arr.append("      ob_type   = [color=#%s]%s[/color];" % [col, type_val])
    arr.append("    };")
    arr.append("  };")
    arr.append("  ob_size = [color=#%s]%s[/color];" % [col, size_val])

    if digit_vals.size() == 0:
        arr.append("  ob_digit = [];")
    else:
        arr.append("  ob_digit = [")
        for digit_val in digit_vals:
            arr.append("    [color=#%s]%s[/color]," % [col, digit_val])
        arr.append("  ];")
    arr.append("}[/color]")

    $CombinedOutput/IntStructure.bbcode_text = arr.join("\n")
