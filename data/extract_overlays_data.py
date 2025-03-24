import bpy
import re
import math

def separate_syntax(str : str) -> tuple[str, str, int]:
    name = ""
    num = 0

    split = str.split('.', 1)
    type = split[0]

    if len(split) > 1:
        split = split[1].rsplit('.', 1)
        name = split[0]

        if len(split) > 1:
            num = int(split[1])
    return (type, name, num)

def constant_to_pascal(const : str) -> str:
    words = const.split('_')

    res = list()
    for word in words:
        res += word.capitalize()

    return ''.join(res)

def pascal_to_constant(pascal : str) -> str:
    words = re.findall('[A-Z][^A-Z]*', pascal)
    return '_'.join(words).upper()

def generate_overlay_data(from_center_x, from_center_y, offset_x, offset_y, scale_x, scale_y) -> str:
    overlays_collection = bpy.data.collections["Overlays"]

    generated : dict[str, tuple[tuple[float, float], tuple[float, float], str]] = dict()

    build : list[str] = list()

    for obj in overlays_collection.objects:
        (type, name, num) = separate_syntax(obj.name)

        if type != "Overlay":
            raise Exception("Not an overlay!")

        if obj.type != 'EMPTY' or obj.empty_display_type != 'IMAGE':
            raise Exception("Not an image!")

        image = obj.data

        x = obj.location.x
        y = obj.location.y

        disp_size = obj.empty_display_size

        game_pos = (                                                \
                        (x - from_center_x) * scale_x + offset_x,   \
                        (y - from_center_y) * scale_y + offset_y    \
                   )

        game_size : tuple[float, float]
        if image.size[0] > image.size[1]:
            res = image.size[1] / image.size[0]
            game_size = (                       \
                abs(disp_size * scale_x),       \
                abs(disp_size * scale_y * res), \
            )
        else:
            res = image.size[0] / image.size[1]
            game_size = (                       \
                abs(disp_size * scale_x * res), \
                abs(disp_size * scale_y),       \
            )

        game_size = (game_size[0] * obj.scale.x, game_size[1] * obj.scale.y)

        file = image.filepath

        generated[name] = (game_pos, game_size, file)

    build.append("%d\n" % len(generated))
    for name, data in generated.items():
        (game_pos, game_size, file) = data
        game_size_int = (int(math.ceil(game_size[0])), int(math.ceil(game_size[1])))
        build.append("%s\n%f %f\n%d %d\n%s\n" % (name, game_pos[0], game_pos[1], game_size_int[0], game_size_int[1], file))

    return ''.join(build)


if __name__ == "__main__":
    # map to:
    # A = (51, 61) # top-left corner
    # B = (267, 189) # bottom-right corner
    SCREEN_RESOLUTION   = (320, 240)
    MAP_RESOLUTION      = (216, 128)
    OFFSET              = (-1, 5)

    # top-left corner
    A = (                                                                       \
            (SCREEN_RESOLUTION[0] / 2) - (MAP_RESOLUTION[0] / 2) + OFFSET[0],   \
            (SCREEN_RESOLUTION[1] / 2) - (MAP_RESOLUTION[1] / 2) + OFFSET[1],   \
        )

    # bottom-right corner
    B = (                                                                       \
            (SCREEN_RESOLUTION[0] / 2) + (MAP_RESOLUTION[0] / 2) + OFFSET[0],   \
            (SCREEN_RESOLUTION[1] / 2) + (MAP_RESOLUTION[1] / 2) + OFFSET[1],   \
        )

    # map from:
    a = (-2.5, 1.48148) # top-left corner
    b = (2.5, -1.48148) # bottom-right corner

    offset_x = A[0]
    offset_y = A[1]

    scale_x = (B[0] - A[0]) / (b[0] - a[0])
    scale_y = (B[1] - A[1]) / (b[1] - a[1])

    print("#__DATA_START__")
    print(generate_overlay_data(a[0], a[1], offset_x, offset_y, scale_x, scale_y))