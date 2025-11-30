#!/bin/bash

checker_dir='./PCLP2025-Tema2-2025.11.24/cs'

if [ ! -d "$checker_dir" ]; then
	exit -1
fi


cp ./*.{c,h} $checker_dir/

cd $checker_dir/
./cs.sh ./

rm ./*.{c,h}

