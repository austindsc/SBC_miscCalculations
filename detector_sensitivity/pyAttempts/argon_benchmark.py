import matplotlib.pyplot as plt
import numpy as np

import nestpy as nest
print('nestpy version: v'+nest.__version__)
print('NEST version: v'+nest.__nest_version__)
print(nest.__path__)

# Nest NR Light Yield, argon
def nlyar(E=10, field = 1, rho = 1.4, interaction = nest.nr,):
    pars = {'interaction': interaction,'energy': E,'drift_field': field, 'density': rho, 'A': 40, 'Z': 20}
    return nest.Yield(**pars)["photon"]/E

# a.values()
Emin = 0.1 #keV
Emax = 20 #keV
en = np.logspace(np.log10(Emin), np.log10(Emax), 1000)

plt.semilogx(en, nlyar(E = en, field = 1, interaction = nest.nr, rho = 1.4))

plt.title("Argon - fails! (looks like xenon)")
plt.xlabel("energy - keV")
plt.show()

