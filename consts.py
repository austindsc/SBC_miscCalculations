#ants
import numpy as np
pi = np.pi

#universal conts
k = 8.617e-5 #boltzmann's, ev/K
mu0 = 4*pi*1e-7 #henry/m or ohm s/m
q = 1.602e-19 #electron charge, C
eps0 = 8.854E-12 #farads/meter
eps0_nm = 8.854E-21 #farads/nm = C^2/J nm
mu0 = 4*pi*1e-7 #henry/m or ohm s/m
c = 2.99792E8 #speed of light in meters/second
fine_structure = 1/137.035 #fine structure constant

#from python code00
#conversions
sigma_to_FWHM = 2*np.sqrt(2*np.log(2))
MeV_to_kg = 1.79E-30
j_to_eV = 6.242E18 #this many eV in 1 joule

#constants from python
k_J = k/j_to_eV #boltzmann's, J/K
m_e = 0.5109e6 #electron mass in eV/c^2
m_p = 938.27e6 #proton mass in eV/c^2
h = 4.136E-15 #plancks constant in eVs
hbar = h/2/pi
hc = c*1e9*h #hc in nmeV
amu = 931.494e6 #amu in eV/c^2

#more consts (added when doing dE/dx)
#r_e = (q**2)/(4*pi*eps0*m_e*1e-9*MeV_to_kg*c**2)
r_e = 2.817e-15 #classical electron radius, m
K_dedx = 0.307075 #dedx coefficient, MeV cm^2/mol

#from daniel
hbar_c = 1.97327e-5 #eV cm
alpha = 0.0072973525664 #fine struct 
#  elec_mass = 1
N_a = 6.022140857e23 #avagadro number
#  N_atom = 1.330200966e+22 #atomic # density (atom/cm^3)
N_atom = 1.376e+22 #atomic # density (atom/cm^3)

#  N_atom = 2.
c_cm = 29.9792458 #speed of light in vac cm/ns