#!/bin/bash
make clean
rm -f etapa2.tgz
tar cvzf etapa1.tgz --exclude='*.py' --exclude='*.sh' --exclude='tests' --exclude='*.pdf' --exclude='.vscode*'  .

