import numpy as np
import matplotlib.pyplot as plt

ly = np.loadtxt("1vly.txt")
e = np.loadtxt("1vE.txt")

plt.semilogx(e, ly)
plt.show()