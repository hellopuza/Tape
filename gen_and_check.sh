#!/usr/bin/env bash

if [ $# -lt 3 ]; then
    echo "Usage: $0 path/to/tape_sorter file-size mem-size"
    exit 1
fi

sorter=$1
file_size=$2
mem_size=$3

src_file="/tmp/tape.src"
dst_file="/tmp/tape.dst"

openssl rand -out $src_file $(( $file_size * 4 ))

$sorter $src_file $dst_file $mem_size

dst_file_dec="$dst_file.dec"
sorted_file_dec="$src_file.dec"

hexdump -e '1/4 "%d" "\n"' $dst_file > $dst_file_dec
hexdump -e '1/4 "%d" "\n"' $src_file | sort -n > $sorted_file_dec

diff $dst_file_dec $sorted_file_dec &>/dev/null