#!/bin/bash

# Test short flags
./opts -v -h -o output.txt -p /path/somewhere

# Test long flags
./opts  --verbose --help --output output.txt --path /path/somewhere

# Test short and long flags together
./opts  -v --verbose -h --help -o output.txt --output /path/somewhere

# Test invalid short flag
./opts  -x

# Test invalid long flag
./opts  --invalid

# Test short flag with argument
./opts  -o output.txt

# Test long flag with argument
./opts  --output output.txt

# Test short flag with no argument
./opts  -p

# Test long flag with no argument
./opts  --path
