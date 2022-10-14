import sys
from random import *
if len(sys.argv) != 4:
    print("nieprawidlowa liczba argumentow")
    exit(1)
proc, tasks, maxlen = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
task_tab = [randint(1, maxlen) for i in range(tasks)] 
file = open('test.txt', 'w')
file.write(f'{proc}\n{tasks}\n')
for i in task_tab:
    file.write(f'{i}\n')
file.close()