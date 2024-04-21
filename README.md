# Tape Sorter
A program for sorting tape-like files containing integers.

## Building
```
mkdir build && cd build
cmake ../
make -j tape_sorter
```

## Testing
You can run all tests or partial ones.
```
make -j test_all
./test_all

make -j test_tape
./tape/tests/test_tape
```
Or run script.
```
./gen_and_check.sh path/to/tape_sorter file-size mem-size
```

## Usage
```
Usage: ./tape_sorter src-file dst-file [MEMSIZE]
```
Configuration file tape.cfg in format: read-lat write-lat move-lat rewind-lat
