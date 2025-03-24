from PIL import Image, ImageFile
import math
import os
import subprocess

replacement_out_dir = "data/generated/textures/replacement"
thumbnail_out_dir = "data/generated/textures/thumbnails"

tab_size = 4
thumbnail_size = (32, 32)

def resize_canvas(img : ImageFile.ImageFile, new_size : tuple[int]) -> Image.Image:
    new_image = Image.new("RGBA", (new_size[0], new_size[1]), (255, 255, 255, 0))

    offset = (0, 0)
    new_image.paste(img, offset)

    return new_image

def generate_overlay_images(file : str, game_size : tuple[int, int]) -> tuple[tuple[str, ImageFile.ImageFile], tuple[str, ImageFile.ImageFile]]:
    img = Image.open(file)
    filename = os.path.basename(file)

    size = img.size

    ratio = (math.ceil(size[0] / game_size[0]), math.ceil(size[1] / game_size[1]))
    canvas_size = (ratio[0] * game_size[0], ratio[1] * game_size[1])

    replacement_image = resize_canvas(img, canvas_size)
    replacement_file = os.path.join(replacement_out_dir, filename)
    try:
        replacement_image.save(replacement_file, "PNG")
    except IOError:
        print("cannot create replacement texture for '%s'" % (file))

    thumbnail_file = os.path.join(thumbnail_out_dir, filename)
    thumbnail_image = replacement_image.copy()
    thumbnail_image.thumbnail(thumbnail_size, Image.Resampling.LANCZOS)
    try:
        thumbnail_image.save(thumbnail_file, "PNG")
    except IOError:
        print("cannot create thumbnail texture for '%s'" % (file))

    return ((replacement_file, replacement_image), (thumbnail_file, thumbnail_image))

def image_to_rgba32_inc_c(img : ImageFile.ImageFile) -> str:
    global tab_size
    build : list[str] = list()
    px_count = 0

    for px in img.getdata():
        if px_count % 8 == 0:
            byte_count = px_count * 4
            build += ' ' * tab_size

        if px_count % 2 == 0:
            build += "0x"

        for val in px:
            hex = f'{val:02x}'.upper()
            build += hex

        if px_count % 2 == 1:
            build += ", "

        if px_count % 8 == 7:
            build += " // 0x%s" % (f'{byte_count:08x}'.upper())
            build += '\n'

        px_count += 1
    build.pop()

    return ''.join(build)

def generate_overlays_data(overlays : dict[str, tuple[tuple[float, float], tuple[int, int], str]]) -> str:
    global tab_size
    build : list[str] = list()

    for name, data in overlays.items():
        (game_pos, game_size, file) = data

        (dir, filename) = os.path.split(blend_file)
        file_split = file.split('//')
        file = file_split[len(file_split) - 1]
        file = os.path.join(dir, file)

        (replacement, thumbnail) = generate_overlay_images(file, game_size)

        # Generate .rgba32.inc.c file for thumbnail.
        (thumbnail_file, thumbnail_img) = thumbnail
        thumbnail_c_file = thumbnail_file.rsplit('.')[0] + ".rgba32.inc.c"
        with open(thumbnail_c_file, "w+") as f:
            f.write(image_to_rgba32_inc_c(thumbnail_img))

        # Generate C data.
        vertices = [
            (
                0,
                0,
                0,
                0,
            ),
            (
                int(game_size[0]),
                0,
                thumbnail_img.width,
                0,
            ),
            (
                int(game_size[0]),
                -int(game_size[1]),
                thumbnail_img.width,
                thumbnail_img.height,
            ),
            (
                0,
                -int(game_size[1]),
                0,
                thumbnail_img.height,
            ),
        ]

        build += "#define %s_OVERLAY_X %f" % (name, game_pos[0])
        build += "\n"
        build += "#define %s_OVERLAY_Y %f" % (name, game_pos[1])
        build += "\n\n"

        build += "#define %s_OVERLAY_WIDTH %d" % (name, thumbnail_img.width)
        build += "\n"
        build += "#define %s_OVERLAY_HEIGHT %d" % (name, thumbnail_img.height)
        build += "\n\n"

        build += "Vtx %sOverlayVtx[] = {\n" % (name)
        for vertex in vertices:
            (x, y, s, t) = vertex
            build += "%sVTX(%d, %d, 0, %d << 5, %d << 5, 0, 0, 0, 0xFF),\n" % (' ' * tab_size, x, y, s, t)
        build += "};"
        build += "\n\n"

        build += "u64 %sOverlayTex[] = {\n" % (name)
        build += '#include "%s"\n' % (thumbnail_c_file.split("data/generated/")[1])
        build += "};"
        build += "\n\n"
    build.pop()

    return ''.join(build)

def get_overlay_data() -> dict[str, tuple[tuple[float, float], tuple[int, int], str]]:
    data = dict()

    # blender ./data/map.blend --background --python data/extract_overlays_data.py
    blender : str
    if os.name == 'nt':
        blender = 'blender.exe'
    else:
        blender = 'blender'
    process_res = subprocess.run([blender, './data/map.blend', '--background', '--python', 'data/extract_overlays_data.py'], stdout=subprocess.PIPE)

    input = process_res.stdout.decode('utf-8')
    input = input.split("#__DATA_START__")[1]
    input = input.split()
    i = 0

    data_count = int(input[i])
    i += 1

    for j in range(data_count):
        name = input[i]
        i += 1
        x = float(input[i])
        i += 1
        y = float(input[i])
        i += 1
        width = int(input[i])
        i += 1
        height = int(input[i])
        i += 1
        file = input[i]
        i += 1

        data[name] = ((x, y), (width, height), file)

    return data

if __name__ == "__main__":
    blend_file = './data/map.blend'
    overlays = get_overlay_data()

    with open("data/generated/overlay_data.c", "w+") as f:
        f.write(generate_overlays_data(overlays))