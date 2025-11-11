from . import theme


def light_mode(light_mode: bool) -> None:
    if light_mode:
        theme.TEXT_COLOR = "black"
        theme.ACCENT_COLOR = "black"
        theme.BACKGROUND_COLOR = "white"
