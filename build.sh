#!/bin/sh
if [ ! -d "./_build" ]; then
	meson setup _build
fi
ninja -C _build
ninja -C _build install
ln -sf $PWD/_build/compile_commands.json $PWD/compile_commands.json
