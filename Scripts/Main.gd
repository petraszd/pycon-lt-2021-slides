extends Node

const NO_SLIDE = -1

var slides = []
var current_slide_idx = NO_SLIDE


func _ready():
    slides = $Slides.get_children()
    assert(slides.size() > 0)
    set_current_slide(0)

func _process(_delta):
    if is_asking_for_next_slide():
        if current_slide_idx < slides.size() - 1:
            set_current_slide(current_slide_idx + 1)
    elif is_asking_for_prev_slide():
        if current_slide_idx > 0:
            set_current_slide(current_slide_idx - 1)


func set_current_slide(next_idx):
    var previous_slide = null
    if current_slide_idx != NO_SLIDE:
        previous_slide = slides[current_slide_idx]

    current_slide_idx = next_idx
    var next_slide = slides[current_slide_idx]

    if previous_slide != null:
        previous_slide.visible = false
    next_slide.visible = true

func is_asking_for_next_slide():
    return (
        Input.is_action_just_released("ui_accept") or
        Input.is_action_just_released("ui_right")
    )

func is_asking_for_prev_slide():
    return Input.is_action_just_released("ui_left")


