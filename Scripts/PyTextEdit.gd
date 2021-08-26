extends TextEdit


export var py_filename = ""

export var values_color = Color(0, 0, 0) # TODO: select
export var other_color = Color(0, 0, 0) # TODO: select
export var flow_color = Color(0, 0, 0)


const VALUES_KEY = "values"
const OTHER_KEY = "other"
const FLOW_KEY = "flow"


const keywords_by_key = {
    VALUES_KEY: [
        "None",
        "True",
        "False",
    ],
    OTHER_KEY: [
        "await",
        "async",
        "import",
        "from",
        "pass",
        "break",
        "raise",
        "class",
        "return",
        "and",
        "or",
        "not",
        "continue",
        "def",
        "nonlocal",
        "assert",
        "del",
        "global",
        "yield",
        "self"
    ],
    FLOW_KEY: [
        "else",
        "elif",
        "if",
        "except",
        "in",
        "finally",
        "is",
        "for",
        "lambda",
        "try",
        "as",
        "while",
        "with",
    ]
}



func _ready():
    _update_editor_colors()

    if py_filename != "":
        text = Utils.get_file_content(py_filename)


func _unhandled_input(event):
    if has_focus() and event.is_action_released("ui_cancel"):
        release_focus()


func _input(event):
    if event is InputEventKey:
        if event.scancode == KEY_TAB and event.is_pressed():
            if get_selection_text() == "":
                if event.shift:
                    var cursor_col_idx = cursor_get_column()
                    var cursor_line_idx = cursor_get_line()

                    var new_lines = PoolStringArray()

                    var lines = text.split("\n")
                    var n_lines = lines.size()
                    var i = 0
                    var col_idx_delta = 0
                    while i < n_lines:
                        if i == cursor_line_idx:
                            if lines[i].begins_with("    "):
                                col_idx_delta = 4
                                new_lines.append(lines[i].substr(4))
                            else:
                                new_lines.append(lines[i])
                        else:
                            new_lines.append(lines[i])
                        i += 1
                    text = new_lines.join("\n")
                    accept_event()
                    cursor_set_line(cursor_line_idx)
                    cursor_set_column(cursor_col_idx - col_idx_delta)
                else:
                    insert_text_at_cursor("    ")
                    accept_event()
            else:
                var col_idx_from = get_selection_from_column()
                var col_idx_to = get_selection_to_column()
                var line_idx_from = get_selection_from_line()
                var line_idx_to = get_selection_to_line()
                var cursor_col_idx = cursor_get_column()
                var cursor_line_idx = cursor_get_line()

                var new_lines = PoolStringArray()

                var lines = text.split("\n")
                var n_lines = lines.size()
                var i = 0
                while i < n_lines:
                    if i >= line_idx_from and i <= line_idx_to:
                        if event.shift:
                            if lines[i].begins_with("    "):
                                new_lines.append(lines[i].substr(4))
                            else:
                                new_lines.append(lines[i])
                        else:
                            new_lines.append("    " + lines[i])
                    else:
                        new_lines.append(lines[i])
                    i += 1
                text = new_lines.join("\n")
                accept_event()
                if event.shift:
                    select(line_idx_from, col_idx_from - 4, line_idx_to, col_idx_to - 4)
                    cursor_set_column(cursor_col_idx - 4)
                else:
                    select(line_idx_from, col_idx_from + 4, line_idx_to, col_idx_to + 4)
                    cursor_set_column(cursor_col_idx + 4)
                cursor_set_line(cursor_line_idx)

        elif event.scancode == KEY_ENTER and event.is_pressed():
            var line = get_line(cursor_get_line())
            var indent = _get_current_indent_level(line)

            insert_text_at_cursor("\n")
            if line.ends_with(":"):
                indent += 1
            for _i in range(indent):
                insert_text_at_cursor("    ")

            accept_event()
        elif event.scancode == KEY_BACKSPACE and event.is_pressed() and get_selection_text() == "":
            var line_idx = cursor_get_line()
            var col_idx = cursor_get_column()

            var current_line = get_line(line_idx)

            if current_line.substr(0, col_idx).ends_with("    "):
                var new_lines = PoolStringArray()

                var lines = text.split("\n")
                var n_lines = lines.size()
                var i = 0
                while i < n_lines:
                    if i == line_idx:
                        new_lines.append(
                            lines[i].substr(0, col_idx - 4) +
                            lines[i].substr(col_idx)
                        )
                    else:
                        new_lines.append(lines[i])
                    i += 1

                text = new_lines.join("\n")
                accept_event()
                cursor_set_line(line_idx)
                cursor_set_column(col_idx - 4)


func _update_editor_colors():
    clear_colors()
    var color_by_key = {
        VALUES_KEY: values_color,
        OTHER_KEY: other_color,
        FLOW_KEY: flow_color,
    }
    for key in keywords_by_key:
        var color = color_by_key[key]
        var keywords = keywords_by_key[key]
        for keyword in keywords:
            add_keyword_color(keyword, color)


func _get_current_indent_level(line):
    var n_spaces = 0
    for chr in line:
        if chr != " ":
            break
        n_spaces += 1

    return n_spaces / 4
