#!/bin/bash
make clean
rm etapa1.tgz
tar cvzf etapa1.tgz --exclude='*.py' --exclude='deliver.sh' --exclude='tests' .

