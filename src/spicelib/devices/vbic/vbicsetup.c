/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Model Author: 1995 Colin McAndrew Motorola
Spice3 Implementation: 2003 Dietmar Warning DAnalyse GmbH
**********/

/* 
 * This routine should only be called when circuit topology
 * changes, since its computations do not depend on most
 * device or model parameters, only on topology (as
 * affected by emitter, collector, and base resistances)
 */

#include "ngspice.h"
#include "cktdefs.h"
#include "smpdefs.h"
#include "vbicdefs.h"
#include "const.h"
#include "sperror.h"
#include "ifsim.h"
#include "suffix.h"

int
VBICsetup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit *ckt, int *states)
        /* load the VBIC structure with those pointers needed later 
         * for fast matrix loading 
         */
{
    VBICmodel *model = (VBICmodel*)inModel;
    VBICinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the transistor models */
    for( ; model != NULL; model = model->VBICnextModel ) {

        if(model->VBICtype != NPN && model->VBICtype != PNP) {
            model->VBICtype = NPN;
        }
        if(!model->VBICtnomGiven) {
            model->VBICtnom = 27.0;
        }
        if(!model->VBICextCollResistGiven) {
            model->VBICextCollResist = 0.0;
        }
        if(!model->VBICintCollResistGiven) {
            model->VBICintCollResist = 0.1;
        }
        if(!model->VBICepiSatVoltageGiven) {
            model->VBICepiSatVoltage = 0.0;
        }
        if(!model->VBICepiDopingGiven) {
            model->VBICepiDoping = 0.0;
        }
        if(!model->VBIChighCurFacGiven) {
            model->VBIChighCurFac = 1.0;
        }
        if(!model->VBICextBaseResistGiven) {
            model->VBICextBaseResist = 0.0;
        }
        if(!model->VBICintBaseResistGiven) {
            model->VBICintBaseResist = 0.1;
        }
        if(!model->VBICemitterResistGiven) {
            model->VBICemitterResist = 0.0;
        }
        if(!model->VBICsubstrateResistGiven) {
            model->VBICsubstrateResist = 0.0;
        }
        if(!model->VBICparBaseResistGiven) {
            model->VBICparBaseResist = 0.1;
        }
        if(!model->VBICsatCurGiven) {
            model->VBICsatCur = 1e-16;
        }
        if(!model->VBICemissionCoeffFGiven) {
            model->VBICemissionCoeffF = 1.0;
        }
        if(!model->VBICemissionCoeffRGiven) {
            model->VBICemissionCoeffR = 1.0;
        }
        if(!model->VBICdeplCapLimitFGiven) {
            model->VBICdeplCapLimitF = 0.9;
        }
        if(!model->VBICextOverlapCapBEGiven) {
            model->VBICextOverlapCapBE = 0.0;
        }
        if(!model->VBICdepletionCapBEGiven) {
            model->VBICdepletionCapBE = 0.0;
        }
        if(!model->VBICpotentialBEGiven) {
            model->VBICpotentialBE = 0.75;
        }
        if(!model->VBICjunctionExpBEGiven) {
            model->VBICjunctionExpBE = 0.33;
        }
        if(!model->VBICsmoothCapBEGiven) {
            model->VBICsmoothCapBE = -0.5;
        }
        if(!model->VBICextOverlapCapBCGiven) {
            model->VBICextOverlapCapBC = 0.0;
        }
        if(!model->VBICdepletionCapBCGiven) {
            model->VBICdepletionCapBC = 0.0;
        }
        if(!model->VBICepiChargeGiven) {
            model->VBICepiCharge = 0.0;
        }
        if(!model->VBICextCapBCGiven) {
            model->VBICextCapBC = 0.0;
        }
        if(!model->VBICpotentialBCGiven) {
            model->VBICpotentialBC = 0.75;
        }
        if(!model->VBICjunctionExpBCGiven) {
            model->VBICjunctionExpBC = 0.33;
        }
        if(!model->VBICsmoothCapBCGiven) {
            model->VBICsmoothCapBC = -0.5;
        }
        if(!model->VBICextCapSCGiven) {
            model->VBICextCapSC = 0.0;
        }
        if(!model->VBICpotentialSCGiven) {
            model->VBICpotentialSC = 0.75;
        }
        if(!model->VBICjunctionExpSCGiven) {
            model->VBICjunctionExpSC = 0.33;
        }
        if(!model->VBICsmoothCapSCGiven) {
            model->VBICsmoothCapSC = -0.5;
        }
        if(!model->VBICidealSatCurBEGiven) {
            model->VBICidealSatCurBE = 1e-18;
        }
        if(!model->VBICportionIBEIGiven) {
            model->VBICportionIBEI = 1.0;
        }
        if(!model->VBICidealEmissCoeffBEGiven) {
            model->VBICidealEmissCoeffBE = 1.0;
        }
        if(!model->VBICnidealSatCurBEGiven) {
            model->VBICnidealSatCurBE = 0.0;
        }
        if(!model->VBICnidealEmissCoeffBEGiven) {
            model->VBICnidealEmissCoeffBE = 2.0;
        }
        if(!model->VBICidealSatCurBCGiven) {
            model->VBICidealSatCurBC = 1e-16;
        }
	if(!model->VBICidealEmissCoeffBCGiven) {
	    model->VBICidealEmissCoeffBC = 1.0;
	}
	if(!model->VBICnidealSatCurBCGiven) {
	    model->VBICnidealSatCurBC = 0.0;
	}
        if(!model->VBICnidealEmissCoeffBCGiven) {
            model->VBICnidealEmissCoeffBC = 2.0;
        }
        if(!model->VBICavalanchePar1BCGiven) {
            model->VBICavalanchePar1BC = 0.0;
        }
        if(!model->VBICavalanchePar2BCGiven) {
            model->VBICavalanchePar2BC = 0.0;
        }
        if(!model->VBICparasitSatCurGiven) {
            model->VBICparasitSatCur = 0.0;
        }
        if(!model->VBICportionICCPGiven) {
            model->VBICportionICCP = 1.0;
        }
        if(!model->VBICparasitFwdEmissCoeffGiven) {
            model->VBICparasitFwdEmissCoeff = 1.0;
        }
        if(!model->VBICidealParasitSatCurBEGiven) {
            model->VBICidealParasitSatCurBE = 0.0;
        }
        if(!model->VBICnidealParasitSatCurBEGiven) {
            model->VBICnidealParasitSatCurBE = 0.0;
        }
        if(!model->VBICidealParasitSatCurBCGiven) {
            model->VBICidealParasitSatCurBC = 0.0;
        }
        if(!model->VBICidealParasitEmissCoeffBCGiven) {
            model->VBICidealParasitEmissCoeffBC = 1.0;
        }
        if(!model->VBICnidealParasitSatCurBCGiven) {
            model->VBICnidealParasitSatCurBC = 0.0;
        }
        if(!model->VBICnidealParasitEmissCoeffBCGiven) {
            model->VBICnidealParasitEmissCoeffBC = 2.0;
        }
        if(!model->VBICearlyVoltFGiven) {
            model->VBICearlyVoltF = 0.0;
        }
        if(!model->VBICearlyVoltRGiven) {
            model->VBICearlyVoltR = 0.0;
        }
        if(!model->VBICrollOffFGiven) {
            model->VBICrollOffF = 0.0;
        }
        if(!model->VBICrollOffRGiven) {
            model->VBICrollOffR = 0.0;
        }
        if(!model->VBICparRollOffGiven) {
            model->VBICparRollOff = 0.0;
        }
        if(!model->VBICtransitTimeFGiven) {
            model->VBICtransitTimeF = 0.0;
        }
        if(!model->VBICvarTransitTimeFGiven) {
            model->VBICvarTransitTimeF = 0.0;
        }
        if(!model->VBICtransitTimeBiasCoeffFGiven) {
            model->VBICtransitTimeBiasCoeffF = 0.0;
        }
        if(!model->VBICtransitTimeFVBCGiven) {
            model->VBICtransitTimeFVBC = 0.0;
        }
        if(!model->VBICtransitTimeHighCurrentFGiven) {
            model->VBICtransitTimeHighCurrentF = 0.0;
        }
        if(!model->VBICtransitTimeRGiven) {
            model->VBICtransitTimeR = 0.0;
        }
        if(!model->VBICdelayTimeFGiven) {
            model->VBICdelayTimeF = 0.0;
        }
        if(!model->VBICfNcoefGiven) {
            model->VBICfNcoef = 0.0;
        }
        if(!model->VBICfNexpAGiven) {
            model->VBICfNexpA = 1.0;
        }
        if(!model->VBICfNexpBGiven) {
            model->VBICfNexpB = 1.0;
        }
        if(!model->VBICtempExpREGiven) {
            model->VBICtempExpRE = 0.0;
        }
        if(!model->VBICtempExpRBIGiven) {
            model->VBICtempExpRBI = 0.0;
        }
        if(!model->VBICtempExpRCIGiven) {
            model->VBICtempExpRCI = 0.0;
        }
        if(!model->VBICtempExpRSGiven) {
            model->VBICtempExpRS = 0.0;
        }
        if(!model->VBICtempExpVOGiven) {
            model->VBICtempExpVO = 0.0;
        }
        if(!model->VBICactivEnergyEAGiven) {
            model->VBICactivEnergyEA = 1.12;
        }
        if(!model->VBICactivEnergyEAIEGiven) {
            model->VBICactivEnergyEAIE = 1.12;
        }
        if(!model->VBICactivEnergyEAICGiven) {
            model->VBICactivEnergyEAIC = 1.12;
        }
        if(!model->VBICactivEnergyEAISGiven) {
            model->VBICactivEnergyEAIS = 1.12;
        }
        if(!model->VBICactivEnergyEANEGiven) {
            model->VBICactivEnergyEANE = 1.12;
        }
	if(!model->VBICactivEnergyEANCGiven) {
	    model->VBICactivEnergyEANC = 1.12;
	}
	if(!model->VBICactivEnergyEANSGiven) {
	    model->VBICactivEnergyEANS = 1.12;
	}
        if(!model->VBICtempExpISGiven) {
            model->VBICtempExpIS = 3.0;
        }
        if(!model->VBICtempExpIIGiven) {
            model->VBICtempExpII = 3.0;
        }
        if(!model->VBICtempExpINGiven) {
            model->VBICtempExpIN = 3.0;
        }
        if(!model->VBICtempExpNFGiven) {
            model->VBICtempExpNF = 0.0;
        }
        if(!model->VBICtempExpAVCGiven) {
            model->VBICtempExpAVC = 0.0;
        }
        if(!model->VBICthermalResistGiven) {
            model->VBICthermalResist = 0.0;
        }
        if(!model->VBICthermalCapacitanceGiven) {
            model->VBICthermalCapacitance = 0.0;
        }
        if(!model->VBICpunchThroughVoltageBCGiven) {
            model->VBICpunchThroughVoltageBC = 0.0;
        }
        if(!model->VBICdeplCapCoeff1Given) {
            model->VBICdeplCapCoeff1 = 0.1;
        }
        if(!model->VBICfixedCapacitanceCSGiven) {
            model->VBICfixedCapacitanceCS = 0.0;
        }
        if(!model->VBICsgpQBselectorGiven) {
            model->VBICsgpQBselector = 0.0;
        }
        if(!model->VBIChighCurrentBetaRolloffGiven) {
            model->VBIChighCurrentBetaRolloff = 0.5;
        }
        if(!model->VBICtempExpIKFGiven) {
            model->VBICtempExpIKF = 0.0;
        }
        if(!model->VBICtempExpRCXGiven) {
            model->VBICtempExpRCX = 0.0;
        }
        if(!model->VBICtempExpRBXGiven) {
            model->VBICtempExpRBX = 0.0;
        }
        if(!model->VBICtempExpRBPGiven) {
            model->VBICtempExpRBP = 0.0;
        }
        if(!model->VBICsepISRRGiven) {
            model->VBICsepISRR = 1.0;
        }
        if(!model->VBICtempExpXISRGiven) {
            model->VBICtempExpXISR = 0.0;
        }
        if(!model->VBICdearGiven) {
            model->VBICdear = 0.0;
        }
        if(!model->VBICeapGiven) {
            model->VBICeap = 1.12;
        }
        if(!model->VBICvbbeGiven) {
            model->VBICvbbe = 0.0;
        }
        if(!model->VBICnbbeGiven) {
            model->VBICnbbe = 1.0;
        }
        if(!model->VBICibbeGiven) {
            model->VBICibbe = 1e-06;
        }
        if(!model->VBICtvbbe1Given) {
            model->VBICtvbbe1 = 0.0;
        }
        if(!model->VBICtvbbe2Given) {
            model->VBICtvbbe2 = 0.0;
        }
        if(!model->VBICtnbbeGiven) {
            model->VBICtnbbe = 0.0;
        }
        if(!model->VBICebbeGiven) {
            model->VBICebbe = 0.0;
        }
        if(!model->VBIClocTempDiffGiven) {
            model->VBIClocTempDiff = 0.0;
        }
        if(!model->VBICrevVersionGiven) {
            model->VBICrevVersion = 1.2;
        }
        if(!model->VBICrefVersionGiven) {
            model->VBICrefVersion = 0.0;
        }

        /* loop through all the instances of the model */
        for (here = model->VBICinstances; here != NULL ;
                here=here->VBICnextInstance) {
            CKTnode *tmpNode;
            IFuid tmpName;

	    if (here->VBICowner != ARCHme)
		goto matrixpointers;

            if(!here->VBICareaGiven) {
                here->VBICarea = 1.0;
            }
            if(!here->VBICmGiven) {
                here->VBICm = 1.0;
            }
            if(!here->VBICdtempGiven) {
                here->VBICdtemp = 0.0;
            }

            here->VBICstate = *states;
            *states += VBICnumStates;
            if(ckt->CKTsenInfo && (ckt->CKTsenInfo->SENmode & TRANSEN) ){
                *states += 8 * (ckt->CKTsenInfo->SENparms);
            }

matrixpointers:
            if(model->VBICextCollResist == 0) {
                here->VBICcollCXNode = here->VBICcollNode;
            } else if(here->VBICcollCXNode == 0) {
                error = CKTmkVolt(ckt,&tmp,here->VBICname,"collector");
                if(error) return(error);
                here->VBICcollCXNode = tmp->number;
                if (ckt->CKTcopyNodesets) {
                  if (CKTinst2Node(ckt,here,1,&tmpNode,&tmpName)==OK) {
                     if (tmpNode->nsGiven) {
                       tmp->nodeset=tmpNode->nodeset; 
                       tmp->nsGiven=tmpNode->nsGiven; 
                    }
                  }
                }
            }
            if(model->VBICextBaseResist == 0) {
                here->VBICbaseBXNode = here->VBICbaseNode;
            } else if(here->VBICbaseBXNode == 0){
                error = CKTmkVolt(ckt,&tmp,here->VBICname, "base");
                if(error) return(error);
                here->VBICbaseBXNode = tmp->number;
                if (ckt->CKTcopyNodesets) {
                  if (CKTinst2Node(ckt,here,2,&tmpNode,&tmpName)==OK) {
                     if (tmpNode->nsGiven) {
                       tmp->nodeset=tmpNode->nodeset; 
                       tmp->nsGiven=tmpNode->nsGiven; 
                     }
                  }
                }
            }
            if(model->VBICemitterResist == 0) {
                here->VBICemitEINode = here->VBICemitNode;
            } else if(here->VBICemitEINode == 0) {
                error = CKTmkVolt(ckt,&tmp,here->VBICname, "emitter");
                if(error) return(error);
                here->VBICemitEINode = tmp->number;
                if (ckt->CKTcopyNodesets) {
                  if (CKTinst2Node(ckt,here,3,&tmpNode,&tmpName)==OK) {
                     if (tmpNode->nsGiven) {
                       tmp->nodeset=tmpNode->nodeset; 
                       tmp->nsGiven=tmpNode->nsGiven; 
                     }
                  }
                }
            }

            error = CKTmkVolt(ckt, &tmp, here->VBICname, "collCI");
            if(error) return(error);
            here->VBICcollCINode = tmp->number;  

            error = CKTmkVolt(ckt, &tmp, here->VBICname, "baseBP");
            if(error) return(error);
            here->VBICbaseBPNode = tmp->number;  

            error = CKTmkVolt(ckt, &tmp, here->VBICname, "baseBI");
            if(error) return(error);
            here->VBICbaseBINode = tmp->number;  

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = SMPmakeElt(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}
            TSTALLOC(VBICcollCollPtr,VBICcollNode,VBICcollNode)
            TSTALLOC(VBICbaseBasePtr,VBICbaseNode,VBICbaseNode)
            TSTALLOC(VBICemitEmitPtr,VBICemitNode,VBICemitNode)
            TSTALLOC(VBICcollCXCollCXPtr,VBICcollCXNode,VBICcollCXNode)
            TSTALLOC(VBICcollCICollCIPtr,VBICcollCINode,VBICcollCINode)
            TSTALLOC(VBICbaseBXBaseBXPtr,VBICbaseBXNode,VBICbaseBXNode)
            TSTALLOC(VBICbaseBIBaseBIPtr,VBICbaseBINode,VBICbaseBINode)
            TSTALLOC(VBICemitEIEmitEIPtr,VBICemitEINode,VBICemitEINode)
            TSTALLOC(VBICbaseBPBaseBPPtr,VBICbaseBPNode,VBICbaseBPNode)

            TSTALLOC(VBICbaseEmitPtr,VBICbaseNode,VBICemitNode)
            TSTALLOC(VBICemitBasePtr,VBICemitNode,VBICbaseNode)
            TSTALLOC(VBICbaseCollPtr,VBICbaseNode,VBICcollNode)
            TSTALLOC(VBICcollBasePtr,VBICcollNode,VBICbaseNode)
            TSTALLOC(VBICcollCollCXPtr,VBICcollNode,VBICcollCXNode)
            TSTALLOC(VBICbaseBaseBXPtr,VBICbaseNode,VBICbaseBXNode)
            TSTALLOC(VBICemitEmitEIPtr,VBICemitNode,VBICemitEINode)
            TSTALLOC(VBICcollCXCollCIPtr,VBICcollCXNode,VBICcollCINode)
            TSTALLOC(VBICcollCXBaseBXPtr,VBICcollCXNode,VBICbaseBXNode)
            TSTALLOC(VBICcollCXBaseBIPtr,VBICcollCXNode,VBICbaseBINode)
            TSTALLOC(VBICcollCXBaseBPPtr,VBICcollCXNode,VBICbaseBPNode)
            TSTALLOC(VBICcollCIBaseBIPtr,VBICcollCINode,VBICbaseBINode)
            TSTALLOC(VBICcollCIEmitEIPtr,VBICcollCINode,VBICemitEINode)
            TSTALLOC(VBICbaseBXBaseBIPtr,VBICbaseBXNode,VBICbaseBINode)
            TSTALLOC(VBICbaseBXEmitEIPtr,VBICbaseBXNode,VBICemitEINode)
            TSTALLOC(VBICbaseBXBaseBPPtr,VBICbaseBXNode,VBICbaseBPNode)
            TSTALLOC(VBICbaseBIEmitEIPtr,VBICbaseBINode,VBICemitEINode)

            TSTALLOC(VBICcollCXCollPtr,VBICcollCXNode,VBICcollNode)
            TSTALLOC(VBICbaseBXBasePtr,VBICbaseBXNode,VBICbaseNode)
            TSTALLOC(VBICemitEIEmitPtr,VBICemitEINode,VBICemitNode)
            TSTALLOC(VBICcollCICollCXPtr,VBICcollCINode,VBICcollCXNode)
            TSTALLOC(VBICbaseBICollCXPtr,VBICbaseBINode,VBICcollCXNode)
            TSTALLOC(VBICbaseBPCollCXPtr,VBICbaseBPNode,VBICcollCXNode)
            TSTALLOC(VBICbaseBXCollCIPtr,VBICbaseBXNode,VBICcollCINode)
            TSTALLOC(VBICbaseBICollCIPtr,VBICbaseBINode,VBICcollCINode)
            TSTALLOC(VBICemitEICollCIPtr,VBICemitEINode,VBICcollCINode)
            TSTALLOC(VBICbaseBPCollCIPtr,VBICbaseBPNode,VBICcollCINode)
            TSTALLOC(VBICbaseBIBaseBXPtr,VBICbaseBINode,VBICbaseBXNode)
            TSTALLOC(VBICemitEIBaseBXPtr,VBICemitEINode,VBICbaseBXNode)
            TSTALLOC(VBICbaseBPBaseBXPtr,VBICbaseBPNode,VBICbaseBXNode)
            TSTALLOC(VBICemitEIBaseBIPtr,VBICemitEINode,VBICbaseBINode)
            TSTALLOC(VBICbaseBPBaseBIPtr,VBICbaseBPNode,VBICbaseBINode)

        }
    }
    return(OK);
}

int
VBICunsetup(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    VBICmodel *model;
    VBICinstance *here;

    for (model = (VBICmodel *)inModel; model != NULL;
	    model = model->VBICnextModel)
    {
        for (here = model->VBICinstances; here != NULL;
                here=here->VBICnextInstance)
	{
	    if (here->VBICcollCXNode
		    && here->VBICcollCXNode != here->VBICcollNode)
	    {
		CKTdltNNum(ckt, here->VBICcollCXNode);
		here->VBICcollCXNode = 0;
	    }
	    if (here->VBICcollCINode
		    && here->VBICcollCINode != here->VBICcollNode)
	    {
		CKTdltNNum(ckt, here->VBICcollCINode);
		here->VBICcollCINode = 0;
	    }
	    if (here->VBICbaseBXNode
		    && here->VBICbaseBXNode != here->VBICbaseNode)
	    {
		CKTdltNNum(ckt, here->VBICbaseBXNode);
		here->VBICbaseBXNode = 0;
	    }
	    if (here->VBICbaseBINode
		    && here->VBICbaseBINode != here->VBICbaseNode)
	    {
		CKTdltNNum(ckt, here->VBICbaseBINode);
		here->VBICbaseBINode = 0;
	    }
	    if (here->VBICbaseBPNode
		    && here->VBICbaseBPNode != here->VBICbaseNode)
	    {
		CKTdltNNum(ckt, here->VBICbaseBPNode);
		here->VBICbaseBPNode = 0;
	    }
	    if (here->VBICemitEINode
		    && here->VBICemitEINode != here->VBICemitNode)
	    {
		CKTdltNNum(ckt, here->VBICemitEINode);
		here->VBICemitEINode = 0;
	    }
	}
    }
    return OK;
}