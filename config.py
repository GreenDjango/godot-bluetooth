def can_build(env, platform):
    return (platform != "server")

def configure(env):
    pass


def get_doc_classes():
    return [
        "Bluetooth",
        "NetworkedMultiplayerBt",
    ]


def get_doc_path():
    return "doc_classes"
