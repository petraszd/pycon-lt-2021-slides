extends Node


const NO_SLIDE = -1

var slides = [
    preload("res://Scenes/Slides/Slide_01.tscn").instance(),
    preload("res://Scenes/Slides/Slide_02.tscn").instance()
]
var current_idx = NO_SLIDE
var next_idx = NO_SLIDE


func _ready():
    assert(slides.size() > 0)
    _set_next_slide(0)


func _process(_delta):
    if _is_asking_for_next_slide():
        if current_idx < slides.size() - 1:
            _set_next_slide(current_idx + 1)
    elif _is_asking_for_prev_slide():
        if current_idx > 0:
            _set_next_slide(current_idx - 1)


func switch_current_to_next():
    var current_slide = slides[current_idx]
    var next_slide = slides[next_idx]

    $CurrentSlidePlace.add_child(next_slide)
    $CurrentSlidePlace.remove_child(current_slide)

    current_idx = next_idx
    next_idx = NO_SLIDE


func _set_next_slide(idx):
    if current_idx == NO_SLIDE:
        current_idx = idx
        print("WTF?")
        $CurrentSlidePlace.add_child(slides[idx])
        return

    next_idx = idx
    $AnimationPlayer.play("SwitchSlides", -1, 4.0)


func _is_asking_for_next_slide():
    return (
        Input.is_action_just_released("ui_accept") or
        Input.is_action_just_released("ui_right")
    )

func _is_asking_for_prev_slide():
    return Input.is_action_just_released("ui_left")


