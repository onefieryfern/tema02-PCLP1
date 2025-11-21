#!/bin/bash

checker_dir='./pclp-cs-21.11.2025'

if [ ! -d "$checker_dir" ]; then
	exit -1
fi


cp ./*.{c,h} $checker_dir/

cd $checker_dir/
./cs.sh ./

rm ./*.{c,h}

