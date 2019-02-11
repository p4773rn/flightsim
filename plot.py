import matplotlib.pyplot as plt
import numpy as mp


ff = open('temp').read().split('\n')
ff = [f for f in ff if len(f) != 0]

pos = [f for f in ff if 'Pos:' in f]
pos = [p[6:-1].split(', ') for p in pos]
pos = [float(p[1]) for p in pos]

time = [float(f) for f in ff if f[0].isdigit()]

plt.plot(time, pos)
plt.savefig('plot.png', bbox_inches='tight')


