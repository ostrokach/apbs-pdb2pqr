#
# * This library is free software; you can redistribute it and/or
# * modify it under the terms of the GNU Lesser General Public
# * License as published by the Free Software Foundation; either
# * version 2.1 of the License, or (at your option) any later version.
# *
# * This library is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# * Lesser General Public License for more details.
#

#propka3.0, revision 182                                                                      2011-08-09
#-------------------------------------------------------------------------------------------------------
#--                                                                                                   --
#--                                   PROPKA: A PROTEIN PKA PREDICTOR                                 --
#--                                                                                                   --
#--                              VERSION 3.0,  01/01/2011, COPENHAGEN                                 --
#--                              BY MATS H.M. OLSSON AND CHRESTEN R. SONDERGARD                       --
#--                                                                                                   --
#-------------------------------------------------------------------------------------------------------
#
#
#-------------------------------------------------------------------------------------------------------
# References:
#
#   Very Fast Empirical Prediction and Rationalization of Protein pKa Values
#   Hui Li, Andrew D. Robertson and Jan H. Jensen
#   PROTEINS: Structure, Function, and Bioinformatics 61:704-721 (2005)
#
#   Very Fast Prediction and Rationalization of pKa Values for Protein-Ligand Complexes
#   Delphine C. Bas, David M. Rogers and Jan H. Jensen
#   PROTEINS: Structure, Function, and Bioinformatics 73:765-783 (2008)
#
#   PROPKA3: Consistent Treatment of Internal and Surface Residues in Empirical pKa predictions
#   Mats H.M. Olsson, Chresten R. Sondergard, Michal Rostkowski, and Jan H. Jensen
#   Journal of Chemical Theory and Computation, 7, 525-537 (2011)
#-------------------------------------------------------------------------------------------------------

from lib import pka_print

def resName2Type(resName=None):
    """
    definition of which parameter-group each residues belongs to
    """

    resType = {'C- ': "COO",
               'ASP': "COO",
               'GLU': "COO",
               'HIS': "HIS",
               'CYS': "CYS",
               'TYR': "TYR",
               'LYS': "LYS",
               'ARG': "ARG",
               'N+ ': "N+ ",
               'SER': "ROH",
               'THR': "ROH",
               'ASN': "AMD",
               'GLN': "AMD",
               'TRP': "TRP"}

    if resName in resType:
        return resType[resName]
    else:
        return resType



def getQs(resName=None):
    """
    Returns a dictionary with residue charges
    """
    Q  =      {'COO': -1.0,
               'ASP': -1.0,
               'GLU': -1.0,
               'C- ': -1.0,
               'TYR': -1.0,
               'CYS': -1.0,
               'HIS':  1.0,
               'LYS':  1.0,
               'ARG':  1.0,
               'N+ ':  1.0}

    if resName in Q:
        return Q[resName]
    else:
        return Q


def pKa_mod(resName=None):
    """
    returns a dictionary with model/water pKa values
    """
    pKa_mod = {'C- ':  3.20,
               'ASP':  3.80,
               'GLU':  4.50,
               'HIS':  6.50,
               'CYS':  9.00,
               'TYR': 10.00,
               'LYS': 10.50,
               'ARG': 12.50,
               'N+ ':  8.00}

    if   resName == None:
        return pKa_mod
    elif resName in pKa_mod:
        return pKa_mod[resName]
    else:
        # generic value for 'uninteresting' residues, e.g. ASN, GLN
        return 20.00


def getInteraction():
    """
      matrix for propka interactions; Note that only the LOWER part of the matrix is used!
   
      'N'   non-iterative interaction
      'I'   iterative interaction
      '-'   no interaction
    """
    
    #                     COO  CYS  TYR  HIS   N+  LYS  ARG
    side_chain = {'COO': ["I", "I", "N", "N", "N", "N", "N"],
                  'CYS': ["I", "I", "N", "I", "N", "N", "N"],
                  'TYR': ["N", "N", "I", "I", "I", "I", "N"],
                  'HIS': ["I", "I", "I", "I", "N", "N", "N"],
                  'N+ ': ["N", "N", "I", "N", "I", "N", "N"],
                  'LYS': ["N", "N", "I", "N", "N", "I", "N"],
                  'ARG': ["N", "N", "N", "N", "N", "N", "I"],
                  'ROH': ["N", "N", "N", "-", "-", "-", "-"],
                  'AMD': ["N", "N", "N", "N", "-", "-", "-"],
                  'TRP': ["N", "N", "N", "-", "-", "-", "-"]}

    return  side_chain


# ------- Coulomb parameters --------- #


def getCoulombParameters(label=None):
    """
    storage of Coulomb default parameters
    """
    CoulombParameters = {}
    CoulombParameters['Linear']                 = {'cutoff':             [4.0,  7.0],
                                                   'max_dpka':                  2.40,
                                                   'scaled':                    True,
                                                  }
    CoulombParameters['Coulomb']                = {'cutoff':             [4.0, 10.0],
                                                   'diel':                     80.00,
                                                   'scaled':                    True,
                                                  }

    if label in CoulombParameters:
      return CoulombParameters[label]
    else:
      return CoulombParameters



# ------- Desolvation parameters --------- #


def getDesolvationParameters(label=None):
    """
    storage of desolvation default parameters
    """
    DesolvationParameters = {}
    DesolvationParameters['propka2']            = {'allowance':               400.00,
                                                   'prefactor':                -0.01,
                                                   'local':                    -0.07,
                                                   'radii':          getLocalRadii(),
                                                  }
    DesolvationParameters['ContactModel']       = {'allowance':               400.00,
                                                   'prefactor':                -0.01,
                                                   'local':                    -0.07,
                                                   'radii':          getLocalRadii(),
                                                  }
    DesolvationParameters['VolumeModel']        = {'allowance':                 0.00,
                                                   'prefactor':               -13.50,
                                                   'surface':                   0.25,
                                                   'volume': getVanDerWaalsVolumes(),
                                                  }
    DesolvationParameters['ScaledVolumeModel']  = {'allowance':                 0.00,
                                                   'prefactor':               -13.50,
                                                   'surface':                   0.00,
                                                   'volume': getVanDerWaalsVolumes(),
                                                  }

    if label in DesolvationParameters:
      return DesolvationParameters[label]
    else:
      return DesolvationParameters


def getVanDerWaalsVolumes():
    """
    storing relative Van der Waals volumes for volume desolvation models
    """
    #                         relative   tabulated
    VanDerWaalsVolume = {'C':    1.40,    # 20.58  all 'C' and 'CA' atoms
                         'C4':   2.64,    # 38.79  hydrodphobic carbon atoms + unidentified atoms
                         'N':    1.06,    # 15.60  all nitrogen atoms
                         'O':    1.00,    # 14.71  all oxygen atoms
                         'S':    1.66,    # 24.43  all sulphur atoms
                        }

    return VanDerWaalsVolume


def getLocalRadii():
    """
    local radii used in the 'propka2' and 'contact' desolvation models
    """
    local_radius = {'ASP': 4.5,
                    'GLU': 4.5,
                    'HIS': 4.5,
                    'CYS': 3.5,
                    'TYR': 3.5,
                    'LYS': 4.5,
                    'ARG': 5.0,
                    'C- ': 4.5,
                    'N+ ': 4.5}

    return local_radius





# ------- hydrogen-bond parameters --------- #


def getHydrogenBondParameters(type=None):
    """ 
    definitions of default back-bone or side-chain interaction parameters
    IMPORTANT: parameters with assigned to 'None' are given by the reverse
               (e.g. CYS-COO is given by COO-CYS) generated at the end.
    """
    if type == "back-bone":

            # --- old 'propka1' back-bone parameter set ---
            # parameters determining the interaction with back-bone NH or CO groups

            parameters        = {"COO": [-1.20, [2.00, 3.50]],
                                 "CYS": [-2.40, [3.50, 4.50]],
                                 "TYR": [-1.20, [3.50, 4.50]],
                                 "HIS": [ 1.20, [2.00, 3.50]],
                                 "N+ ": [ 1.20, [2.00, 3.50]],
                                 "LYS": [ 1.20, [2.00, 3.50]],
                                 "ARG": [ 1.20, [2.00, 3.50]]}

    elif type == "side-chain":


            # --- old 'propka1' side-chain parameter set ---
            # parameters determining the interaction with side-chain NH or CO groups
            # IMPORTANT: parameters with assigned to 'None' are given by the reverse
            # (e.g. CYS-COO is given by COO-CYS) generated at the end.

            parameters = {}
            parameters["COO"] = {"COO": [-0.80, [ 2.50, 3.50]],
                                 "CYS": [-0.80, [ 3.00, 4.00]],
                                 "TYR": [-0.80, [ 3.00, 4.00]],
                                 "HIS": [-0.80, [ 2.00, 3.00]],
                                 "N+ ": [-1.20, [ 3.00, 4.50]],
                                 "LYS": [-0.80, [ 3.00, 4.00]],
                                 "ARG": [-0.80, [ 2.00, 4.00]],
                                 "ROH": [-0.80, [ 3.00, 4.00]],
                                 "AMD": [-0.80, [ 2.00, 3.00]],
                                 "TRP": [-0.80, [ 2.00, 3.00]]}
            parameters["CYS"] = {"COO": None,
                                 "CYS": [-1.60, [ 3.00, 5.00]],
                                 "TYR": [-0.80, [ 3.50, 4.50]],
                                 "HIS": [-1.60, [ 3.00, 4.00]],
                                 "N+ ": [-2.40, [ 3.00, 4.50]],
                                 "LYS": [-1.60, [ 3.00, 4.00]],
                                 "ARG": [-1.60, [ 2.50, 4.00]],
                                 "ROH": [-1.60, [ 3.50, 4.50]],
                                 "AMD": [-1.60, [ 2.50, 3.50]],
                                 "TRP": [-1.60, [ 2.50, 3.50]]}
            parameters["TYR"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": [ 0.80, [ 3.50, 4.50]],
                                 "HIS": [-0.80, [ 2.00, 3.00]],
                                 "N+ ": [-1.20, [ 3.00, 4.50]],
                                 "LYS": [-0.80, [ 3.00, 4.00]],
                                 "ARG": [-0.80, [ 2.50, 4.00]],
                                 "ROH": [-0.80, [ 3.50, 4.50]],
                                 "AMD": [-0.80, [ 2.50, 3.50]],
                                 "TRP": [-0.80, [ 2.50, 3.50]]}
            parameters["HIS"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": [ 0.00, [ 0.00, 0.00]],
                                 "N+ ": [ 0.00, [ 0.00, 0.00]],
                                 "LYS": [ 0.00, [ 0.00, 0.00]],
                                 "ARG": [ 0.00, [ 0.00, 0.00]],
                                 "ROH": [ 0.00, [ 0.00, 0.00]],
                                 "AMD": [ 0.80, [ 2.00, 3.00]],
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}
            parameters["N+ "] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": None,
                                 "N+ ": [ 0.00, [ 0.00, 0.00]],
                                 "LYS": [ 0.00, [ 0.00, 0.00]],
                                 "ARG": [ 0.00, [ 0.00, 0.00]],
                                 "ROH": [ 0.00, [ 0.00, 0.00]],
                                 "AMD": [ 0.00, [ 0.00, 0.00]],
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}
            parameters["LYS"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": None,
                                 "N+ ": None,
                                 "LYS": [ 0.00, [ 0.00, 0.00]],
                                 "ARG": [ 0.00, [ 0.00, 0.00]],
                                 "ROH": [ 0.00, [ 0.00, 0.00]],
                                 "AMD": [ 0.00, [ 0.00, 0.00]],
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}
            parameters["ARG"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": None,
                                 "N+ ": None,
                                 "LYS": None,
                                 "ARG": [ 0.00, [ 0.00, 0.00]],
                                 "ROH": [ 0.00, [ 0.00, 0.00]],
                                 "AMD": [ 0.00, [ 0.00, 0.00]],
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}
            parameters["ROH"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": None,
                                 "N+ ": None,
                                 "LYS": None,
                                 "ARG": None,
                                 "ROH": [ 0.00, [ 0.00, 0.00]],
                                 "AMD": [ 0.00, [ 0.00, 0.00]],
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}
            parameters["AMD"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": None,
                                 "N+ ": None,
                                 "LYS": None,
                                 "ARG": None,
                                 "ROH": None,
                                 "AMD": [ 0.00, [ 0.00, 0.00]],
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}
            parameters["TRP"] = {"COO": None,
                                 "CYS": None,
                                 "TYR": None,
                                 "HIS": None,
                                 "N+ ": None,
                                 "LYS": None,
                                 "ARG": None,
                                 "ROH": None,
                                 "AMD": None,
                                 "TRP": [ 0.00, [ 0.00, 0.00]]}


            # updating side-chain parameter matrix to full matrix
            keys = parameters.keys()
            for key1 in keys:
              for key2 in keys:
                if key2 not in parameters[key1]:
                  parameters[key1][key2] == [ 0.00, [ 0.00, 0.00]]
                elif parameters[key1][key2] == None:
                  parameters[key1][key2] = parameters[key2][key1]


    else:
      pka_print("cannot determine what type of hydrogen-bonding interactions you want type=\"%s\" ['back-bone', 'side-chain']" % (label))
      sys.exit(9)

    return parameters


