extends HBoxContainer

export var default_color = Color(0, 0, 0)
export var interesting_color = Color(0, 0, 0)


func _ready():
    var struct = PoolStringArray();
    var interesting_color_html = interesting_color.to_html(false)
    struct.append("[color=#%s]{" % default_color.to_html(false))
    struct.append("  ob_base = {")
    struct.append("    ob_base = {")
    struct.append("      ob_refcnt = [color=#%s]TODO[/color];" % interesting_color_html)
    struct.append("      ob_type   = [color=#%s]PyLong_Type[/color];" % interesting_color_html)
    struct.append("    };")
    struct.append("  };")
    struct.append("  ob_size = [color=#%s]TODO[/color];" % interesting_color_html)
    struct.append("  ob_digit = [")
    struct.append("    [color=#%s]TODO[/color]," % interesting_color_html)
    struct.append("  ];")
    struct.append("}[/color]")

    $MarginContainer/IntStructure.bbcode_text = struct.join("\n")
