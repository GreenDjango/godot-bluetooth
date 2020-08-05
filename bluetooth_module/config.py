def can_build(env, platform):
    return (platform != "osx")


def configure(env):
    pass


def get_doc_classes():
    return [
        "NetworkedMultiplayerBt",
    ]


def get_doc_path():
    return "doc_classes"
