import glob

print("")
svg = glob.glob("ic_*.svg")


def iconName(file):
    file = file.replace("ic_", "")
    file = file.replace(".svg", "")

    file = file[0].capitalize() + file[1:]

    loc = file.find("_")
    while loc != -1:

        a = file[0:loc]
        b = file[loc + 1 :]

        file = a + b[0].capitalize() + b[1:]
        loc = file.find("_")

    return file


def logEnum(svg):
    print("")
    for file in svg:
        print(iconName(file) + ",")


def logResourceSwitch(svg):
    print("")
    print("switch (icon)")
    print("{")
    for file in svg:

        name = iconName(file)

        print("case %s:"%name)
        print('return QIcon(":/icons/%s");' % (file))

    print("default: return QIcon();")
    print("}")

logEnum(svg)
logResourceSwitch(svg)