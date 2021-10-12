import numpy as np
import numpy.random as rand
import scipy.misc as misc
import scipy.special as sp

from consts import *

import sys
sys.path.append('..')
from physics_functions import *


def P_rcoin(rate, area, tcoin): #probability a random event is in timewindow Tcoin
    #rate in Hz/mm
    #tcoin in ns
    #area is SiPM area in mm (include factor of 4 for ganged SiPMs)
    tcoin = tcoin * 1e-9
    rate = rate*area
    return 1 - np.exp(-rate*tcoin)


