/**
 *  @file    sorparm.c
 *  @ingroup SORparm
 *  @author  Juan Brandi
 *  @brief   Class SORparm methods
 *  @version $Id$
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

#include "sorparm.h"

VEMBED(rcsid="$Id$")

#if !defined(VINLINE_SORPARM)

#endif


VPUBLIC SORparm* SORparm_ctor(SORparm_CalcType type) {

    /* Set up the structure */
    SORparm *thee = VNULL;
    thee = (SORparm*)Vmem_malloc(VNULL, 1, sizeof(SORparm));
    VASSERT( thee != VNULL);
    VASSERT( SORparm_ctor2(thee, type) == VRC_SUCCESS );

    return thee;
}

VPUBLIC Vrc_Codes SORparm_ctor2(SORparm *thee, SORparm_CalcType type) {

    int i;

    if (thee == VNULL) return VRC_FAILURE;

    thee->parsed = 0;
    thee->type = type;
    thee->omega = 0;
    thee->setomega = 0;
    thee->etol = 1.0e-6;

    return VRC_SUCCESS;
}

VPUBLIC void SORparm_dtor(SORparm **thee) {
    if ((*thee) != VNULL) {
        SORparm_dtor2(*thee);
        Vmem_free(VNULL, 1, sizeof(SORparm), (void **)thee);
        (*thee) = VNULL;
    }
}

VPUBLIC void SORparm_dtor2(SORparm *thee) { ; }

VPUBLIC Vrc_Codes SORparm_check(SORparm *thee) {

    Vrc_Codes rc;

    rc = VRC_SUCCESS;

    Vnm_print(0, "SORparm_check:  checking SORparm object of type %d.\n",
              thee->type);

    /* Check to see if we were even filled... */
    if (!thee->parsed) {
        Vnm_print(2, "SORparm_check:  not filled!\n");
        return VRC_FAILURE;
    }


    /* Check type settings */
    if(thee->type != SORCT_AUTO) {
         Vnm_print(2,"SORparm_check: type not set");
         rc = VRC_FAILURE;
    }

    return rc;
}

VPUBLIC void SORparm_copy(SORparm *thee, SORparm *parm) {
    VASSERT(thee != VNULL);
    VASSERT(parm != VNULL);

    thee->type = parm->type;
    thee->parsed = parm->parsed;

    thee->omega = parm->omega;
    thee->etol = parm->etol;
}

Vrc_Codes NOMORE(const char* name){
    Vnm_print(2, "parseSOR:  ran out of tokens on %s!\n", name);
    return VRC_WARNING;
}

Vrc_Codes parsePositive(double* tf, double def, int* set, char* name, Vio* sock){
    char tok[VMAX_BUFSIZE];
    if(Vio_scanf(sock, "%s", tok) == 0) {
        *tf = def;
        return NOMORE(name);
    }

    if (sscanf(tok, "%lf", tf) == 0){
        Vnm_print(2, "NOsh:  Read non-float (%s) while parsing %s keyword!\n", tok, name);
        *tf = def;
        return VRC_WARNING;
    }else if(*tf < 0.0){
        Vnm_print(2, "parseSOR:  %s must be greater than 0!\n", name);
        *tf = def;
        return VRC_WARNING;
    }

    *set = 1;
    return VRC_SUCCESS;
}

VPRIVATE Vrc_Codes SORparm_parseOMEGA(SORparm *thee, Vio *sock){
    const char* name = "omega";
    char tok[VMAX_BUFSIZE];
	int tf;
    if(Vio_scanf(sock, "%s", tok) == 0) {
        return NOMORE(name);
    }


    if (sscanf(tok, "%u", &tf) == 0){
        Vnm_print(2, "NOsh:  Read non-unsigned int (%s) while parsing %s keyword!\n", tok, name);
        return VRC_WARNING;
    }else if(tf < 0 && tf > 2){
        Vnm_print(2, "parseSOR:  %s must be between 0 or 2!\n", name);
        return VRC_WARNING;
    }else{
        thee->omega = tf;
    }
    thee->setomega = 1;
    return VRC_SUCCESS;
}

VPRIVATE Vrc_Codes SORparm_parseETOL(SORparm *thee, Vio *sock){

	char tok[VMAX_BUFSIZE];
	double tf;

	VJMPERR1(Vio_scanf(sock, "%s", tok) == 1);
	if(sscanf(tok, "%lf", &tf) == 0){
		Vnm_print(2, "NOsh: Read non-float (%s) while parsing etol keyword!\n", tok);
		return VRC_WARNING;
	} else if(tf <= 0.0) {
		Vnm_print(2,"parseSOR: etol must be greater than 0!\n");
		return VRC_WARNING;
	} else {
		thee->etol = tf;
	}


	return VRC_SUCCESS;


	VERROR1:
		Vnm_print(2, "parseSOR: ran out of tokens!\n");
		return VRC_WARNING;

}

VPUBLIC Vrc_Codes SORparm_parseToken(SORparm *thee, char tok[VMAX_BUFSIZE],
  Vio *sock) {

    if (thee == VNULL) {
        Vnm_print(2, "parseSOR:  got NULL thee!\n");
        return VRC_WARNING;
    }
    if (sock == VNULL) {
        Vnm_print(2, "parseSOR:  got NULL socket!\n");
        return VRC_WARNING;
    }

    Vnm_print(0, "SORparm_parseToken:  trying %s...\n", tok);

    if (Vstring_strcasecmp(tok, "omega") == 0) {
        return SORparm_parseOMEGA(thee, sock);
    } else if(Vstring_strcasecmp(tok, "etol") == 0){
    	return SORparm_parseETOL(thee, sock);
    }else {
        Vnm_print(2, "parseSOR:  Unrecognized keyword (%s)!\n", tok);
        return VRC_WARNING;
    }


    return VRC_FAILURE;
}
