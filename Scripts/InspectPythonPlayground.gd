extends HBoxContainer

export var default_color = Color(0, 0, 0)
export var value_color = Color(0, 0, 0)


var refcnt_val = "Unknown"
var type_val = "Unknown"
var size_val = "Unknown"
var digit_vals = []


func _ready():
    _redraw_int_structure()


func _on_PyTextEditWithRunBtn_after_python_exec(_stdout, stderr):
    if stderr:
        $IntStructure.bbcode_text = stderr
    else:
        _redraw_int_structure()


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

    $IntStructure.bbcode_text = arr.join("\n")
