#!/bin/bash

checker_dir='./check_capybara_remastered'

if [ ! -d "$checker_dir" ]; then
	exit -1
fi

cp ./*.{c,h} $checker_dir/
cp ./README $checker_dir/
cp ./Makefile $checker_dir/


source ./.venv/bin/activate

cd $checker_dir/

if [ -z $1 ]; then
	python3 ./check
else
	python3 ./check --task "$1"
fi

deactivate


rm ./*.{c,h} ./README ./Makefile

