import numpy as np
import scipy.special as sp

from decimal import Decimal

from consts import m_e
from consts import c

######## physics ########
def get_lambda(freq):
	return c/freq

def get_gamma(beta):
	return 1/np.sqrt(1 - beta**2)

def gamma(m0, K):
	gamma = K/m0 + 1
	return gamma

def get_beta(m0, K):
	return np.sqrt(1 - gamma(m0, K)**-2)

def get_gb(m0, K):
    beta = get_beta(m0, K)
    return beta*get_gamma(beta)

def getP(m0, K): # units in eV/c
    return np.sqrt((K + m0)**2 - m0**2)

def getK(m0, P):
    K = np.sqrt(P**2 + m0**2) - m0
    return K

def get_energy(m0, gamma):
	return gamma*m0

## scattering kinematics ##
def reducedMass(m1, m2):
    return m1*m2/(m1 + m2)

def frac(M, K):
	gam = gamma(M, K)
	top = 2*m_e*(gam + 1)/M
	bottom = (1 + 2*gam*m_e/M + (m_e/M)**2)
	return top/bottom

def get_maxW(M, K):
	gam = gamma(M, K)
	beta = get_beta(gam)
	Eo = M
	return 2*beta**2*(K**2/Eo +2*K + Eo)/(M/m_e + 2*gam + m_e/M)