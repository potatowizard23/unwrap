#!/bin/sh
if [ ! -f "_build/io.github.potatowizard23.unwrap" ]; then
	./build.sh
fi
./_build/io.github.potatowizard23.unwrap
