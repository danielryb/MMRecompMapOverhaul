import bpy
import re

cursor_list = [
    "GREAT_BAY",
    "ZORA_HALL",
    "ROMANI_RANCH",
    "DEKU_PALACE",
    "WOODFALL",
    "CLOCK_TOWN",
    "SNOWHEAD",
    "IKANA_GRAVEYARD",
    "IKANA_CANYON",
    "GORON_VILLAGE",
    "STONE_TOWER",
]

warp_list = [
    "GREAT_BAY_COAST",
    "ZORA_CAPE",
    "SNOWHEAD",
    "MOUNTAIN_VILLAGE",
    "CLOCK_TOWN",
    "MILK_ROAD",
    "WOODFALL",
    "SOUTHERN_SWAMP",
    "IKANA_CANYON",
    "STONE_TOWER",
]

entr_list = [
    "MAYORS_RESIDENCE",
    "MAJORAS_LAIR",
    "MAGIC_HAGS_POTION_SHOP",
    "RANCH_HOUSE",
    "HONEY_AND_DARLINGS_SHOP",
    "BENEATH_THE_GRAVERYARD",
    "SOUTHERN_SWAMP_CLEARED",
    "CURIOSITY_SHOP",
    "UNSET_08",
    "UNSET_09",
    "GROTTOS",
    "UNSET_0B",
    "UNSET_0C",
    "UNSET_0D",
    "CUTSCENE",
    "UNSET_0F",
    "IKANA_CANYON",
    "PIRATES_FORTRESS",
    "MILK_BAR",
    "STONE_TOWER_TEMPLE",
    "TREASURE_CHEST_SHOP",
    "STONE_TOWER_TEMPLE_INVERTED",
    "CLOCK_TOWER_ROOFTOP",
    "OPENING_DUNGEON",
    "WOODFALL_TEMPLE",
    "PATH_TO_MOUNTAIN_VILLAGE",
    "IKANA_CASTLE",
    "DEKU_SCRUB_PLAYGROUND",
    "ODOLWAS_LAIR",
    "TOWN_SHOOTING_GALLERY",
    "SNOWHEAD_TEMPLE",
    "MILK_ROAD",
    "PIRATES_FORTRESS_INTERIOR",
    "SWAMP_SHOOTING_GALLERY",
    "PINNACLE_ROCK",
    "FAIRY_FOUNTAIN",
    "SWAMP_SPIDER_HOUSE",
    "OCEANSIDE_SPIDER_HOUSE",
    "ASTRAL_OBSERVATORY",
    "MOON_DEKU_TRIAL",
    "DEKU_PALACE",
    "MOUNTAIN_SMITHY",
    "TERMINA_FIELD",
    "POST_OFFICE",
    "MARINE_RESEARCH_LAB",
    "DAMPES_HOUSE",
    "UNSET_2E",
    "GORON_SHRINE",
    "ZORA_HALL",
    "TRADING_POST",
    "ROMANI_RANCH",
    "TWINMOLDS_LAIR",
    "GREAT_BAY_COAST",
    "ZORA_CAPE",
    "LOTTERY_SHOP",
    "UNSET_37",
    "PIRATES_FORTRESS_EXTERIOR",
    "FISHERMANS_HUT",
    "GORON_SHOP",
    "DEKU_KINGS_CHAMBER",
    "MOON_GORON_TRIAL",
    "ROAD_TO_SOUTHERN_SWAMP",
    "DOGGY_RACETRACK",
    "CUCCO_SHACK",
    "IKANA_GRAVEYARD",
    "GOHTS_LAIR",
    "SOUTHERN_SWAMP_POISONED",
    "WOODFALL",
    "MOON_ZORA_TRIAL",
    "GORON_VILLAGE_SPRING",
    "GREAT_BAY_TEMPLE",
    "WATERFALL_RAPIDS",
    "BENEATH_THE_WELL",
    "ZORA_HALL_ROOMS",
    "GORON_VILLAGE_WINTER",
    "GORON_GRAVERYARD",
    "SAKONS_HIDEOUT",
    "MOUNTAIN_VILLAGE_WINTER",
    "GHOST_HUT",
    "DEKU_SHRINE",
    "ROAD_TO_IKANA",
    "SWORDMANS_SCHOOL",
    "MUSIC_BOX_HOUSE",
    "IGOS_DU_IKANAS_LAIR",
    "TOURIST_INFORMATION",
    "STONE_TOWER",
    "STONE_TOWER_INVERTED",
    "MOUNTAIN_VILLAGE_SPRING",
    "PATH_TO_SNOWHEAD",
    "SNOWHEAD",
    "PATH_TO_GORON_VILLAGE_WINTER",
    "PATH_TO_GORON_VILLAGE_SPRING",
    "GYORGS_LAIR",
    "SECRET_SHRINE",
    "STOCK_POT_INN",
    "GREAT_BAY_CUTSCENE",
    "CLOCK_TOWER_INTERIOR",
    "WOODS_OF_MYSTERY",
    "LOST_WOODS",
    "MOON_LINK_TRIAL",
    "THE_MOON",
    "BOMB_SHOP",
    "GIANTS_CHAMBER",
    "GORMAN_TRACK",
    "GORON_RACETRACK",
    "EAST_CLOCK_TOWN",
    "WEST_CLOCK_TOWN",
    "NORTH_CLOCK_TOWN",
    "SOUTH_CLOCK_TOWN",
    "LAUNDRY_POOL",
]

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

def generate_position(name : str, x, y, from_center_x, from_center_y, offset_x, offset_y, scale_x, scale_y) -> str:
    build : list[str] = list()

    rx = (x - from_center_x) * scale_x + offset_x
    ry = (y - from_center_y) * scale_y + offset_y

    constant = pascal_to_constant(name)
    build += "#define %s_X %s\n" \
             "#define %s_Y %s\n" % (constant, str(rx), constant, str(ry))

    return ''.join(build)

def generate_cursor_positions(from_center_x, from_center_y, offset_x, offset_y, scale_x, scale_y) -> str:
    cursors_collection = bpy.data.collections["Cursors"]

    generated : dict[str, dict[tuple[float, float]]] = dict()

    build : list[str] = list()

    for obj in cursors_collection.objects:
        (type, name, num) = separate_syntax(obj.name)

        if type != "Cursor":
            raise Exception("Not a cursor!")

        location = obj.location

        x = location.x
        y = location.y

        rx = (x - from_center_x) * scale_x + offset_x
        ry = (y - from_center_y) * scale_y + offset_y

        if not name in generated:
            generated[name] = dict()
        generated[name] = (rx, ry)

    for i, name in enumerate(cursor_list):
        pascal = constant_to_pascal(name)
        if pascal in generated:
                (x, y)  = generated[pascal]

                build += "#define %s_X %s\n" \
                         "#define %s_Y %s" % (name, str(x), name, str(y))
                build += '\n\n'
    build.pop()

    return ''.join(build)

def generate_warp_positions(from_center_x, from_center_y, offset_x, offset_y, scale_x, scale_y) -> str:
    prefix       = "OWL_WARP_"

    cursors_collection = bpy.data.collections["Warps"]

    generated : dict[str, dict[tuple[float, float]]] = dict()

    build : list[str] = list()

    for obj in cursors_collection.objects:
        (type, name, num) = separate_syntax(obj.name)

        if type != "Warp":
            raise Exception("Not a warp!")

        location = obj.location

        x = location.x
        y = location.y

        rx = (x - from_center_x) * scale_x + offset_x
        ry = (y - from_center_y) * scale_y + offset_y

        if not name in generated:
            generated[name] = dict()
        generated[name] = (rx, ry)

    for i, name in enumerate(warp_list):
        pascal = constant_to_pascal(name)
        if pascal in generated:
                (x, y)  = generated[pascal]

                build += "#define %s%s_X %s\n" \
                         "#define %s%s_Y %s" % (prefix, name, str(x), prefix, name, str(y))
                build += '\n\n'
    build.pop()

    return ''.join(build)

class Map:
    def __init__(self, posY):
        self.posY = posY
        self.points = list()

def generate_minimap_to_worldmap_maps(from_center_x, from_center_y, offset_x, offset_y, scale_x, scale_y) -> str:
    tab_size = 4
    element_type        = "minimap_map_t"
    arr_name            = "MiniToWorld"
    subarr_prefix       = arr_name + "_"

    minimaps_collection = bpy.data.collections["Minimaps"]

    generated : dict[str, dict[int, Map]] = dict()

    build : list[str] = list()

    for obj in minimaps_collection.objects:
        (type, name, num) = separate_syntax(obj.name)

        if type != "Minimap":
            raise Exception("Not a minimap!")

        posY = obj['posY']
        map = Map(posY)

        i = 0
        mesh = obj.data
        for vert in mesh.vertices:
            v_global = obj.matrix_world @ vert.co # global vertex coordinates

            x = v_global.x
            y = v_global.y

            rx = (x - from_center_x) * scale_x + offset_x
            ry = (y - from_center_y) * scale_y + offset_y

            map.points.append((rx, ry))

            i += 1

        if i != 4:
            raise Exception("Invalid number of vertices in minimap!")

        if not name in generated:
            generated[name] = dict()
        generated[name][num] = map

    for i, name in enumerate(entr_list):
        pascal = constant_to_pascal(name)
        if pascal in generated:
            scene = generated[pascal]
            map_count = len(scene)

            build += "%s %s%s[%d] = {\n" % (element_type, subarr_prefix, pascal, map_count)
            for num in range(map_count):
                map = scene[num]
                build += "%s%s,\n" % (' ' * tab_size, str(map.posY))
                for (x, y) in map.points:
                    build += "%s%s, %s,\n" % (' ' * tab_size, str(x), str(y))
                build += '\n'
            build.pop()
            build += "};"
            build += '\n' * 2

    build += "%s* %s[ENTR_SCENE_MAX] = {\n" % (element_type, arr_name)
    for i, name in enumerate(entr_list):
        pascal = constant_to_pascal(name)

        contents : str
        if pascal in generated:
            contents = "%s%s" % (subarr_prefix, pascal)
        else:
            contents = "NULL"

        build += "%s%s,\n" % (' ' * tab_size, contents)
    build += "};"

    return ''.join(build)

if __name__ == "__main__":
    # map to:
    # A = (51, 61) # top-left corner
    # B = (267, 189) # bottom-right corner
    SCREEN_RESOLUTION   = (320, 240)
    MAP_RESOLUTION      = (216, 128)
    OFFSET              = (-1, 5)

    # top-left corner
    A = (                                                                   \
        (SCREEN_RESOLUTION[0] / 2) - (MAP_RESOLUTION[0] / 2) + OFFSET[0],   \
        (SCREEN_RESOLUTION[1] / 2) - (MAP_RESOLUTION[1] / 2) + OFFSET[1],   \
    )

    # bottom-right corner
    B = (                                                                   \
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


    with open("data/generated/map_data.c", "w+") as f:
        f.write("// center\n\n")
        f.write(generate_position("MapCenterPx", 0, 0, a[0], a[1], offset_x, offset_y, scale_x, scale_y))

        f.write("\n// cursor\n\n")
        f.write(generate_cursor_positions(a[0], a[1], offset_x, offset_y, scale_x, scale_y))

        f.write("\n// warps\n\n")
        f.write(generate_warp_positions(a[0], a[1], offset_x, offset_y, scale_x, scale_y))

        f.write("\n// minimaps\n\n")
        f.write(generate_minimap_to_worldmap_maps(a[0], a[1], offset_x, offset_y, scale_x, scale_y))