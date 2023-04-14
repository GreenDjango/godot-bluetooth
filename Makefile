
build-debug:
	scons target=template_debug platform=linux arch=x86_64

build-release:
	scons target=template_release platform=linux arch=x86_64
#	strip project/addons/luaAPI/bin/*
