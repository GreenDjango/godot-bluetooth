# {%- assign bin_dir = "../bin" -%}

# build-debug:
# {%  for target in all_targets -%}
# {%-   assign t = target | split: "," -%}
# {%-   assign build_target = t[0] -%}
# {%-   assign target_type = t[2] -%}
# {%-   if target_type == "debug" -%}
# #	make build-{{build_target}}-{{target_type}}
# {%    endif %}
# {%- endfor %}

# build-release:
# {%  for target in all_targets -%}
# {%-   assign t = target | split: "," -%}
# {%-   assign build_target = t[0] -%}
# {%-   assign target_type = t[2] -%}
# {%-   if target_type == "release" -%}
# #	make build-{{build_target}}-{{target_type}}
# {%    endif %}
# {%- endfor %}

# export-debug:
# {%  for target in all_targets -%}
# {%-   assign t = target | split: "," -%}
# {%-   assign build_target = t[0] -%}
# {%-   assign target_type = t[2] -%}
# {%-   if target_type == "debug" -%}
# #	make export-{{build_target}}-{{target_type}}
# {%   endif %}
# {%- endfor %}

# export-release:
# {%  for target in all_targets -%}
# {%-   assign t = target | split: "," -%}
# {%-   assign build_target = t[0] -%}
# {%-   assign target_type = t[2] -%}
# {%-   if target_type == "release" -%}
# #	make export-{{build_target}}-{{target_type}}
# {%    endif %}
# {%- endfor %}

# {%- for target in all_targets %}
# {%-   assign t = target | split: "," -%}
# {%-   assign build_target = t[0] -%}
# {%-   assign export_target = t[1] -%}
# {%-   assign target_type = t[2] -%}

# {%-   capture exported_project -%}
# {%-     case export_target -%}
# {%-       when "Android" -%}
# {{project-name}}.{{target_type}}.{{build_target}}.apk
# {%-       when "Linux/X11" -%}
# {{project-name}}.{{target_type}}.{{build_target}}
# {%-       when "Mac OSX" -%}
# {{project-name}}.{{target_type}}.{{build_target}}
# {%-       when "iOS" -%}
# {{project-name}}.{{target_type}}.{{build_target}}.ipa
# {%-       when "Windows Desktop" -%}
# {{project-name}}.{{target_type}}.{{build_target}}.exe
# {%-     endcase -%}
# {%-   endcapture -%}

# {%-   capture lib_ext -%}
# {%-     case export_target -%}
# {%-       when "Android" -%}
# so
# {%-       when "Linux/X11" -%}
# so
# {%-       when "Mac OSX" -%}
# dylib
# {%-       when "iOS" -%}
# a
# {%-       when "Windows Desktop" -%}
# dll
# {%-     endcase -%}
# {%-   endcapture -%}

# {%-   capture build_arg -%}
# {%-     case target_type -%}
# {%-       when "debug" -%}

# {%-       when "release" -%}
# --release
# {%-     endcase -%}
# {%-   endcapture -%}

# {%-   capture export_arg -%}
# {%-     case target_type -%}
# {%-       when "debug" -%}
# --export-debug
# {%-       when "release" -%}
# --export
# {%-     endcase -%}
# {%-   endcapture %}

# build-{{build_target}}-{{target_type}}:
# 	cargo build --target {{build_target}} {{build_arg}}
# 	mv -b {{target_dir}}/{{build_target}}/{{target_type}}/*.{{lib_ext}} {{lib_dir}}/{{build_target}}

# export-{{build_target}}-{{target_type}}: clean build-{{build_target}}-{{target_type}}
# 	cd godot/ ; godot {{export_arg}} "{{export_target}}.{{build_target}}.{{target_type}}" {{bin_dir}}/{{build_target}}/{{exported_project}}
# {% endfor -%}

RUST_BUILD_DIR	:= ./target
LIB_DIR			:= ./addons/blueplug/lib

LIB_EXT_aarch64-linux-android := so
LIB_EXT_armv7-linux-androideabi := so
LIB_EXT_i686-linux-android := so
LIB_EXT_x86_64-linux-android := so

LIB_EXT_i686-unknown-linux-gnu := so
LIB_EXT_x86_64-unknown-linux-gnu := so

LIB_EXT_x86_64-apple-darwin := dylib
LIB_EXT_aarch64-apple-ios := a

LIB_EXT_i686-pc-windows-gnu := dll
LIB_EXT_x86_64-pc-windows-gnu := dll
LIB_EXT_i686-pc-windows-msvc := dll
LIB_EXT_x86_64-pc-windows-msvc := dll

build-%-debug:
	cargo build --target $*
	mv -b $(RUST_BUILD_DIR)/$*/debug/*.$(LIB_EXT_$*) $(LIB_DIR)/$*

build-%-release:
	cargo build --target $* --release
	mv -b $(RUST_BUILD_DIR)/$*/release/*.$(LIB_EXT_$*) $(LIB_DIR)/$*


PROJECT_NAME		:= blueplug
GODOT_BINARY_PATH	:= /home/leo/Epitech/esp/Godot_v3.5.1-stable_x11.64
GODOT_PROJECT_PATH	:= project

all: run

create-debug-keystore:
	keytool -keyalg RSA -genkeypair -alias androiddebugkey -keypass android -keystore $(PROJECT_NAME).debug.keystore -storepass android -dname "CN=Android Debug,O=Android,C=US" -validity 9999 -deststoretype pkcs12

create-release-keystore:
	keytool -v -genkey -v -keystore $(PROJECT_NAME).release.keystore -alias $(PROJECT_NAME) -keyalg RSA -validity 10000

audit:
	cargo audit

check: clean
	cargo check

clean:
	cargo clean

doc: clean
	cargo doc --no-deps --open -v

edit:
	$(GODOT_BINARY_PATH) --path $(GODOT_PROJECT_PATH) -e &

run:
	make build-x86_64-unknown-linux-gnu-debug
#	$(GODOT_BINARY_PATH) --path $(GODOT_PROJECT_PATH) -d

test: clean
	cargo test
