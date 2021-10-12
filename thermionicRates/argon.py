import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as integrate

c = 3*10**8 # m/s
amu = 931.05*10**6 # eV/amu
k = 8.617 *10**(-5) # eV/K
q = 1.6*10**(-19)
pi = np.pi

def getK(m, v): # m in eV/c^2, v in m/s

    mass = m*amu/c/c
    return mass/2 * v**2

def getV(m, K):

	mass = m*amu/c/c
	return np.sqrt(2*K/mass)

def mB_velocity(v, m, T):

    m = m*amu/c/c
    # something wrong here? or somewhere with units
    A = (m/2/pi/k/T)**(3/2)
    B = 4*pi*v**2
    C = np.exp(-m*v**2/2/k/T)

    return A*B*C

def mB_energy(m, T, E): # takes in particle mass (eV/c^2),  T (K) and energy (eV)

    m = m*amu/c/c #something definitely wrong here with units and c
    A = np.sqrt(m/2)
    B = 4*pi*(pi*k*T)**(-3/2)
    C = np.exp(-E/(k*T))

    return E * A * B * C


E_list = np.arange(0, 1, 0.001)  #1 eV
v_list = np.arange(0, 800, 10)

m = 40

plt.ion()
fig, axes = plt.subplots(2, 1, figsize = (10,5))

ax0 = axes[0]
ax1 = axes[1]

ax0.plot(v_list, mB_velocity(m = 40, T = 100, v = v_list))

vthresh = getV(m = 40, K = 40)


E_listFar = np.arange(40, 1000, 1)
V_listFar = np.arange(vthresh, 10*vthresh, 100)

print(integrate.trapz(x = E_listFar, y = mB_energy(E = E_listFar, m = 40, T = 100)))
print(integrate.trapz(x = V_listFar, y = mB_velocity(v = V_listFar, m = 40, T = 100)))

# ax1.plot(getK(m = 40, v = v_list), mB_velocity(m = 40, T = 100, v = v_list))

ax1.plot(E_list, mB_energy(E = E_list, m = 40, T = 300))



plt.show()





