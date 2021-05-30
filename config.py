def can_build(env, platform):
    return (platform == "x11")
    # for futur: or platform == "windows" or platform == "osx" or platform == "android"


def configure(env):
    pass


def get_doc_classes():
    return [
        "Bluetooth",
        "NetworkedMultiplayerBt",
    ]


def get_doc_path():
    return "doc_classes"
