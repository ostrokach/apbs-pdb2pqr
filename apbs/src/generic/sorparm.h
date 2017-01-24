/** @defgroup SORparm SORparm class
 *  @brief    Parameter which holds useful parameters for SOR calculations.
 */

/**
 *  @file     SORparm.h
 *  @ingroup  SORparm
 *  @brief    Contains declarations for class SORparm
 *  @version  $Id$
 *  @author   Juan Brandi
 *
 *  @attention
 *  @verbatim
 *
 * APBS -- Adaptive Poisson-Boltzmann Solver
 *
 *  Nathan A. Baker (nathan.baker@pnnl.gov)
 *  Pacific Northwest National Laboratory
 *
 *  Additional contributing authors listed in the code documentation.
 *
 * Copyright (c) 2010-2014 Battelle Memorial Institute. Developed at the
 * Pacific Northwest National Laboratory, operated by Battelle Memorial
 * Institute, Pacific Northwest Division for the U.S. Department of Energy.
 *
 * Portions Copyright (c) 2002-2010, Washington University in St. Louis.
 * Portions Copyright (c) 2002-2010, Nathan A. Baker.
 * Portions Copyright (c) 1999-2002, The Regents of the University of
 * California.
 * Portions Copyright (c) 1995, Michael Holst.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of the developer nor the names of its contributors may be
 * used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @endverbatim
 */


#ifndef _SORPARM_H_
#define _SORPARM_H_

/* Generic header files */
#include "maloc/maloc.h"

#include "generic/vhal.h"
#include "generic/vstring.h"

/**
 * @brief  Calculation type
 * @ingroup SORparm
 */
enum eSORparm_CalcType {
    //other methods disabled for now only auto currently implemented.
	SORCT_AUTO=0,  /**< SOR-manual */
};

/**
 * @brief  Declare SORparm_CalcType type
 * @ingroup  SORparm
 */
typedef enum eSORparm_CalcType SORparm_CalcType;

/**
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @brief   Parameter structure for SOR-specific variables from input files
 *  @note    If you add/delete/change something in this class, the member
 *           functions -- especially SORparm_copy -- must be modified
 *           accordingly
 */
typedef struct sSORparm {

    SORparm_CalcType type;  /**< Type of SOR calculation */
    int parsed;  /**< Has this structure been filled? (0 = no, 1 = yes) */

    /* *** GENERIC PARAMETERS *** */
    int omega;
    int setomega;
    double etol; /**< user defined error tolerance */

} SORparm;

/** @brief   Construct SORparm object
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @param   type Type of SOR calculation
 *  @returns Newly allocated and initialized SORparm object
 */
VEXTERNC SORparm*  SORparm_ctor(SORparm_CalcType type);

/** @brief   Stub to construct SORparm object
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @param   thee Space for SORparm object
 *  @param   type Type of SOR calculation
 *  @returns Success enumeration
 */
VEXTERNC Vrc_Codes      SORparm_ctor2(SORparm *thee, SORparm_CalcType type);

/** @brief   Object destructor
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @param   thee  Pointer to memory location of SORparm object
 */
VEXTERNC void     SORparm_dtor(SORparm **thee);

/** @brief   Stub for object destructor
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @param   thee  Pointer to SORparm object
 */
VEXTERNC void     SORparm_dtor2(SORparm *thee);

/** @brief   Consistency check for parameter values stored in object
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @param   thee   SORparm object
 *  @returns Success enumeration
 */
VEXTERNC Vrc_Codes      SORparm_check(SORparm *thee);

/** @brief   Parse a SOR keyword from an input file
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @param   thee   SORparm object
 *  @param   tok    Token to parse
 *  @param   sock   Stream for more tokens
 *  @returns Success enumeration (1 if matched and assigned; -1 if matched, but there's some sort
 *            of error (i.e., too few args); 0 if not matched)
 */
VEXTERNC Vrc_Codes      SORparm_parseToken(SORparm *thee, char tok[VMAX_BUFSIZE],
                    Vio *sock);
/**
 * @brief copy SORparm object int thee.
 * @ingroup SORparm
 * @author
 * @param thee SORparm object to be copied into
 * @param parm SORparm object.
 */
VEXTERNC void SORparm_copy(SORparm *thee, SORparm *parm);

VPRIVATE Vrc_Codes SORparm_parseOMEGA(SORparm *thee, Vio *sock);

VPRIVATE Vrc_Codes SORparm_parseETOL(SORparm *thee, Vio *sock);

#endif

