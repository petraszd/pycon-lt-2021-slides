extends Node

const NO_SLIDE = -1

var slides = [
    preload("res://Scenes/Slides/Slide_01.tscn").instance(),
    preload("res://Scenes/Slides/Slide_02.tscn").instance()
]
var current_idx = NO_SLIDE


func _ready():
    assert(slides.size() > 0)
    set_current_slide(0)

func _process(_delta):
    if is_asking_for_next_slide():
        if current_idx < slides.size() - 1:
            set_current_slide(current_idx + 1)
    elif is_asking_for_prev_slide():
        if current_idx > 0:
            set_current_slide(current_idx - 1)


func set_current_slide(next_idx):
    var previous_slide = null
    if current_idx != NO_SLIDE:
        previous_slide = slides[current_idx]

    current_idx = next_idx
    var next_slide = slides[current_idx]

    if previous_slide != null:
        $CurrentSlidePlace.remove_child(previous_slide)
    $CurrentSlidePlace.add_child(next_slide)

func is_asking_for_next_slide():
    return (
        Input.is_action_just_released("ui_accept") or
        Input.is_action_just_released("ui_right")
    )

func is_asking_for_prev_slide():
    return Input.is_action_just_released("ui_left")


