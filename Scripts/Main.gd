extends Node


const NO_SLIDE = -1

var slides = [
    preload("res://Scenes/Slides/Slide_01.tscn").instance(),
    preload("res://Scenes/Slides/Slide_02.tscn").instance(),
    preload("res://Scenes/Slides/Slide_03.tscn").instance(),
    preload("res://Scenes/Slides/Slide_04.tscn").instance(),
    preload("res://Scenes/Slides/Slide_05.tscn").instance(),
    preload("res://Scenes/Slides/Slide_06.tscn").instance(),
    preload("res://Scenes/Slides/Slide_07.tscn").instance(),
    preload("res://Scenes/Slides/Slide_08.tscn").instance(),
    preload("res://Scenes/Slides/Slide_09.tscn").instance(),
    preload("res://Scenes/Slides/Slide_10.tscn").instance(),
]
var current_idx = NO_SLIDE
var next_idx = NO_SLIDE


func _ready():
    assert(slides.size() > 0)
    _set_next_slide(0)


func _unhandled_input(event):
    var owner = $CurrentSlidePlace.get_focus_owner()
    if owner != null and owner.get_class() == "TextEdit":
        return

    if $AnimationPlayer.is_playing():
        return

    if _is_asking_for_next_slide(event):
        if current_idx < slides.size() - 1:
            _set_next_slide(current_idx + 1)
    elif _is_asking_for_prev_slide(event):
        if current_idx > 0:
            _set_next_slide(current_idx - 1)


func switch_current_to_next():
    var current_slide = slides[current_idx]
    var next_slide = slides[next_idx]

    $CurrentSlidePlace.add_child(next_slide)
    $CurrentSlidePlace.remove_child(current_slide)

    current_idx = next_idx
    next_idx = NO_SLIDE


func show_particles_and_move_to_next_slide():
    var view_size = get_viewport().size
    $ParticlesFor42.position = view_size * 0.5

    next_idx = current_idx + 1
    $AnimationPlayer.play("ShowParticlesFor42")


func _set_next_slide(idx):
    if current_idx == NO_SLIDE:
        current_idx = idx
        $CurrentSlidePlace.add_child(slides[idx])
        return

    next_idx = idx
    $AnimationPlayer.play("SwitchSlides", -1, 4.0)


func _is_asking_for_next_slide(event):
    return (
        event.is_action_released("ui_accept") or
        event.is_action_released("ui_right")
    )

func _is_asking_for_prev_slide(event):
    return event.is_action_released("ui_left")
