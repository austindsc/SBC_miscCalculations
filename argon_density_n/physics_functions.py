import numpy as np

def pVapor_T(temp): #return vapour pressure in bar, temp in K
    # using McCain and Ziegler from NIST
    #https://webbook.nist.gov/cgi/cbook.cgi?ID=C7440371&Mask=4
    A = 4.469
    B = 481.012
    C = 22.156
    
    P = 10**(A - B/(temp + C))

    return P

def arDensity_Terry(T): #takes temp T in kelvin, returns density in g/cm^3
    Tr = 83.82
    Vr = 28.295
    A = 1.1577/10
    B = 1.55095/1e3
    C = -3.08415/1e5
    D = 8.42918/1e7
    
    Vm = Vr + A*(T - Tr) + B*(T - Tr)**2 + C*(T-Tr)**3 + D*(T - Tr)**4
    M_argon = 39.948
    rho = M_argon/Vm
    return rho

def getSellmeierN_T83(lam):
    lamUV = 106.6
    lamIR = 908.3
    
    a0 = 1.24
    aUV = 0.27
    aIR = 0.00047
    
    return np.sqrt(a0 + aUV*lam**2/(lam**2 - lamUV**2) + aIR*lam**2/(lam**2 - lamIR**2))

def getSellmeierN_T90(lam):
    lamUV = 106.6
    lamIR = 908.3
    
    a0 = 1.26
    aUV = 0.23
    aIR = 0.0023
    
    return np.sqrt(a0 + aUV*lam**2/(lam**2 - lamUV**2) + aIR*lam**2/(lam**2 - lamIR**2))

def getAlambda(lam, T, nFunction, rhoFunction):
    
    n = nFunction(lam)
    rho = rhoFunction(T)
    
    return (n**2 - 1)/(n**2 + 2)/rho

def getMeanAlambda(lam, rhoFunction):
    
    n1 = getSellmeierN_T83(lam)
    rho1 = rhoFunction(83.81)
    
    n2 = getSellmeierN_T90(lam)
    rho2 = rhoFunction(90.)
    
    A1 = (n1**2 - 1)/(n1**2 + 2)/rho1
    A2 = (n2**2 - 1)/(n2**2 + 2)/rho2
    return (A1 + A2)/2
    
def getMeanN_lamT(lam, T, rhoFunction):
    
    meanA = getMeanAlambda(lam, rhoFunction)
    rho = rhoFunction(T)
    b = meanA*rho
    
    return np.sqrt((1 + 2*b)/(1-b))