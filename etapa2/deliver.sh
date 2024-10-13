#!/bin/bash
make clean
rm -f etapa2.tgz
tar cvzf etapa1.tgz --exclude='*.py' --exclude='deliver.sh' --exclude='tests' --exclude='*.pdf' .

