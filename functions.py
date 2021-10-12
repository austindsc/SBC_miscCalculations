import numpy as np
import scipy.special as sp

from decimal import Decimal

from consts import m_e
from consts import c

######## printing + helpers ########
def p(number):
	test = '%.2E' % Decimal(number)
	print(test)
	return

def pm(name, number):
	test = '%.2E' % Decimal(number)
	print(name, "=", test)
	return

def pr(name, number):
	test = '%.2f' % number
	print(name, "=", test)
	return

def pmu(name, number, units):
	test = '%.3E' % Decimal(number)
	print(name, "=", test, units)
	return

######## general math functions ########
def exp_integral(x):
    return np.exp(-x)/x

def myGaussian(x, mean, sigma):
    pi = np.pi
    C = 1/(sigma*np.sqrt(2*pi))
    exp = np.exp((-((x - mean)/sigma)**2)/2)
    return C*exp

def simpGaussian(x, mean, sigma):
    arg = (x - mean)**2
    return np.exp(-arg/(2*sigma*sigma))

#sketchy function. This takes the derivative of 'func' wrt t and must be used in a specific way
#plt.plot(t1log, get_intensity(t1log, function, 0, [1, M, rho_e, tau_s, tau_t]), '.y')
#			^array				^array	     	^	^ leave both of these, don't touch them.
#last args (4 here) are normal function arguments. function must take time as first argument

#maybe i will return to this once i understand it and make it better
def get_intensity(t, func, var = 0, point=[]):
	args = point[:] #whole array of arguments
	def wraps(x):
		args[var] = x #take the variable we want
		return func(*args)
	return -misc.derivative(wraps, t, dx = 1e-6)