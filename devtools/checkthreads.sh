#!/bin/sh

valgrind --tool=drd ./cursebook 2>check.log && less check.log
