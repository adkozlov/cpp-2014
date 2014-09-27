#!/bin/bash
valgrind --tool=memcheck --leak-check=yes ./matrix
