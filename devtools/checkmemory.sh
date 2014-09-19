#!/bin/sh

valgrind --tool=memcheck --leak-check=yes --leak-resolution=high ./cursebook 2>check.log && less check.log
