from os import listdir
from os.path import isfile, join
import os
import subprocess
import filecmp


PATH='tests/'

only_files = [f for f in listdir(PATH) if isfile(join(PATH, f))]
test_files = [f for f in only_files if f.endswith('.txt')]
result_files = [f for f in only_files if f.endswith('.exp')]
dict_files = {f : next(filter(lambda s: s.startswith(f[:-4]), result_files))  for f in test_files}

subprocess.run(["make", "clean"], stdout=open(os.devnull, 'wb'))
subprocess.run(["make"], stdout=open(os.devnull, 'wb')) 
command = './etapa1 < {0} > result.txt'
msg_errors = []
for test_file in dict_files:
    os.system(command.format(join(PATH, test_file))) # It's not safe to use but I'm a bad boy
    if not filecmp.cmp('result.txt', join(PATH, dict_files[test_file]), shallow=False):
        msg_errors.append(test_file)

os.remove('result.txt')
subprocess.run(["make", "clean"], stdout=open(os.devnull, 'wb'))
if msg_errors:
    print("Tests with errors:")
    for e in msg_errors:
        print(e)