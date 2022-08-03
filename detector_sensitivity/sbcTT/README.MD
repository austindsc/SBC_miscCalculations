sbcTT: Scintillating Bubble Chamber Threshold Transformation
or SBC True Threshold

SBC defines their detection threshold as heat, which must 
be deposited in a certain volume. Conventional liquid noble experiments
define threshold as a kinetic energy of the recoiling particle (typically
an electrong or nuclear recoil, ER or NR).

NOBODY measures total energy. These experiments measure charge and/or light,
and by doing neutron calibrations or using sources with known energy spectra,
the kinetic energy is inferred and the 'true threshold' calculated.

SBC uses a 'heat threshold'. This code will use NEST to very roughly calculate
the scaling/transformation from kinetic energy to heat energy, in turn
allowing the transformation from heat threshold to true threshold.
Approximations are made for this toy model.

Rc  - critical radius
Th  - heat threshold
K   - Kinetic energy of primary
Eqh - heat energy from thermalizing charges

--- Model Basics ---
K = light + charge + heat

* assume light leaves system fast enough (ns) to have no effect
on bubble formation (us)
* NR: assume all inteaction and energy deposition occurs within critical radius
* ER: not done yet. Edep likely over area > Rc

charge treatment is non-trivial. Light from recombination is charge that doesn't escape
What about heat from escaping charges? Electrons loose energy as they thermalize

What is estimate of electron thermalization energy?
Eqh = ??
K = 
Ephoton = NP * hc/wl
Echarge = NQ * Ebandgap
Heat = trueHeat + Eqh

Eqh ~ Nq * (Average electronic energy given - bandgap)
Eqh ~ Nq * (Wvalue - bandgap)

what about: this is something else. Eheat in scintillation process?
check my heat from RCX 1
Eqh ~ Nq * (W - max Excitation energy)
Eqh ~ Nq * (W - 178nm)
^^ can do some correction for photons, and direct excitations
^^ all ignoring possibility of IR light

Three cases are outlined below:
Case A: sparse burst model (SB)
charges leaves the Rc promptly. No E loss within Rc
Heat = K - light - charge

Case B: strong coupling model (SC)
charges thermalize quickly within Rc. All electron thermalizing energy 
available is lost within Rc

Case C: quasi-real model (QR)
somewhere between A and B

