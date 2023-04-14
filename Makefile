GODOT_BINARY_PATH	?= godot4
GODOT_PROJECT_PATH	:= demo

build-debug:
	scons target=template_debug platform=linux arch=x86_64

build-release:
	scons target=template_release platform=linux arch=x86_64
#	strip project/addons/luaAPI/bin/*

edit:
	$(GODOT_BINARY_PATH) --path $(GODOT_PROJECT_PATH) -e

test: build-debug
	$(GODOT_BINARY_PATH) --path $(GODOT_PROJECT_PATH) -d