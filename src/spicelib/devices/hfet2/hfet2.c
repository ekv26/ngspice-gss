
#include "ngspice.h"
#include <stdio.h>
#include "ifsim.h"
#include "devdefs.h"
#include "hfet2defs.h"
#include "suffix.h"


IFparm HFET2pTable[] = { /* parameters */ 
 OP("off",       HFET2_OFF,            IF_FLAG    ,""),
 IOP("l",        HFET2_LENGTH,         IF_REAL    ,""),
 IOP("w",        HFET2_WIDTH,          IF_REAL    ,""),
 IOP("icvds",    HFET2_IC_VDS,         IF_REAL    ,""),
 IOP("icvgs",    HFET2_IC_VGS,         IF_REAL    ,""),
 IOP("temp",     HFET2_TEMP,           IF_REAL    ,""),
 OP("dnode",     HFET2_DRAINNODE,      IF_INTEGER ,""),
 OP("gnode",     HFET2_GATENODE,       IF_INTEGER ,""),
 OP("snode",     HFET2_SOURCENODE,     IF_INTEGER ,""),
 OP("dprimenode",HFET2_DRAINPRIMENODE, IF_INTEGER ,""),
 OP("sprimenode",HFET2_SOURCEPRIMENODE,IF_INTEGER ,""),
 OP("vgs",       HFET2_VGS,            IF_REAL    ,""),
 OP("vgd",       HFET2_VGD,            IF_REAL    ,""),
 OP("cg",        HFET2_CG,             IF_REAL    ,""),
 OP("cd",        HFET2_CD,             IF_REAL    ,""),
 OP("cgd",       HFET2_CGD,            IF_REAL    ,""),
 OP("gm",        HFET2_GM,             IF_REAL    ,""),
 OP("gds",       HFET2_GDS,            IF_REAL    ,""),
 OP("ggs",       HFET2_GGS,            IF_REAL    ,""),
 OP("ggd",       HFET2_GGD,            IF_REAL    ,""),
 OP("qgs",       HFET2_QGS,            IF_REAL    ,""),
 OP("cqgs",      HFET2_CQGS,           IF_REAL    ,""),
 OP("qgd",       HFET2_QGD,            IF_REAL    ,""),
 OP("cqgd",      HFET2_CQGD,           IF_REAL    ,""),
 OP("cs",        HFET2_CS,             IF_REAL    ,""),
 OP("p",         HFET2_POWER,          IF_REAL    ,"")

};

IFparm HFET2mPTable[] = { /* model parameters */
  OP( "type",    HFET2_MOD_TYPE,    IF_STRING,"NHFET or PHFET"),
 IOP( "nhfet",   HFET2_MOD_NHFET,   IF_FLAG,"N type HFET model"),
 IOP( "phfet",   HFET2_MOD_PHFET,   IF_FLAG,"P type HFET model"),
 IOP( "cf",      HFET2_MOD_CF,      IF_REAL,""),
 IOP( "d1",      HFET2_MOD_D1,      IF_REAL,""),
 IOP( "d2",      HFET2_MOD_D2,      IF_REAL,""),
 IOP( "del",     HFET2_MOD_DEL,     IF_REAL,""),
 IOP( "delta",   HFET2_MOD_DELTA,   IF_REAL,""),
 IOP( "deltad",  HFET2_MOD_DELTAD,  IF_REAL,"Thickness correction"),  
 IOP( "di",      HFET2_MOD_DI,      IF_REAL,"Depth of device"),
 IOP( "epsi",    HFET2_MOD_EPSI,    IF_REAL,""),
 IOP( "eta",     HFET2_MOD_ETA,     IF_REAL,"Subthreshold ideality factor"),
 IOP( "eta1",    HFET2_MOD_ETA1,    IF_REAL,""), 
 IOP( "eta2",    HFET2_MOD_ETA2,    IF_REAL,""),
 IOP( "gamma",   HFET2_MOD_GAMMA,   IF_REAL,"Knee shape parameter"),
 IOP( "ggr",     HFET2_MOD_GGR,     IF_REAL,""),
 IOP( "js",      HFET2_MOD_JS,      IF_REAL,""),
 IOP( "klambda", HFET2_MOD_KLAMBDA, IF_REAL,""),
 IOP( "kmu",     HFET2_MOD_KMU,     IF_REAL,""),
 IOP( "knmax",   HFET2_MOD_KNMAX,   IF_REAL,""),
 IOP( "kvto",    HFET2_MOD_KVTO,    IF_REAL,""),
 IOP( "lambda",  HFET2_MOD_LAMBDA,  IF_REAL,"Output conductance parameter"),
 IOP( "m",       HFET2_MOD_M,       IF_REAL,"Knee shape parameter"),
 IOP( "mc",      HFET2_MOD_MC,      IF_REAL,"Knee shape parameter"),
 IOP( "mu",      HFET2_MOD_MU,      IF_REAL,"Moblity"),
 IOP( "n",       HFET2_MOD_N,       IF_REAL,""),
 IOP( "nmax",    HFET2_MOD_NMAX,    IF_REAL,""),
 IOP( "p",       HFET2_MOD_P,       IF_REAL,""),
 IOP( "rd",      HFET2_MOD_RD,      IF_REAL,"Drain ohmic resistance"),
 IOP( "rdi",     HFET2_MOD_RDI,     IF_REAL,"Drain ohmic resistance"),
 IOP( "rs",      HFET2_MOD_RS,      IF_REAL,"Source ohmic resistance"),
 IOP( "rsi",     HFET2_MOD_RSI,     IF_REAL,"Source ohmic resistance"),
 IOP( "sigma0",  HFET2_MOD_SIGMA0,  IF_REAL,"DIBL parameter"),
 IOP( "vs",      HFET2_MOD_VS,      IF_REAL,"Saturation velocity"), 
 IOP( "vsigma",  HFET2_MOD_VSIGMA,  IF_REAL,""), 
 IOP( "vsigmat", HFET2_MOD_VSIGMAT, IF_REAL,""),
 IOP( "vt0",     HFET2_MOD_VTO,     IF_REAL,""),
 IOP( "vt1",     HFET2_MOD_VT1,     IF_REAL,""),
 IOP( "vt2",     HFET2_MOD_VT2,     IF_REAL,""),
 IOP( "vto",     HFET2_MOD_VTO,     IF_REAL,"")
 
};

char *HFET2names[] = {
    "Drain",
    "Gate",
    "Source"
};

int HFET2nSize = NUMELEMS(HFET2names);
int HFET2pTSize = NUMELEMS(HFET2pTable);
int HFET2mPTSize = NUMELEMS(HFET2mPTable);
int HFET2iSize = sizeof(HFET2instance);
int HFET2mSize = sizeof(HFET2model);