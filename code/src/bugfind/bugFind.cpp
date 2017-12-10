/**CppFile**************************************************************

  FileName    [bugFind.cpp]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [bug finder]

  Synopsis    [Command file]

  Author      [Danny Wang]

  Affiliation [NTUEE]

  Date        [15, December, 2016]

  Revision    []

***********************************************************************/

#include "aig/aig/aig.h"
#include "base/abc/abc.h"
#include "base/main/mainInt.h"
#include "sat/cnf/cnf.h"

#include <limits.h>
#include <iostream>
#include "bugFind.h"

using namespace std;

#ifndef _WIN32
#include <unistd.h>
#endif

// #define BUGFIND_DEVELOP

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

extern Aig_Man_t *Abc_NtkToDar(Abc_Ntk_t *pNtk, int fExors,
                               int fRegisters);
extern void Abc_NtkCecSat(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2,
                          int nConfLimit, int nInsLimit);
extern Vec_Ptr_t *Abc_NtkDfs(Abc_Ntk_t *pNtk, int fCollectAll);
extern word Aig_ManRandom64(int fReset);
extern void Abc_NtkVerifyReportError(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2,
                                     int *pModel);
extern Abc_Ntk_t *Abc_NtkMulti(Abc_Ntk_t *pNtk, int nThresh, int nFaninMax,
                               int fCnf, int fMulti, int fSimple,
                               int fFactor);

void BugFind_Init(Abc_Frame_t *pAbc);

static int Abc_CommandBugFind(Abc_Frame_t *pAbc, int argc, char **argv);
static int Abc_CommandBugFindPrint(Abc_Frame_t *pAbc, int argc,
                                   char **argv);

void BugFind_End(Abc_Frame_t *pAbc);

int BugFind_PrepareTwoNtks(FILE *pErr, Abc_Ntk_t *pNtk, char **argv,
                           int argc, Abc_Ntk_t **ppNtk1,
                           Abc_Ntk_t **ppNtk2, int *pfDelete1,
                           int *pfDelete2);
word BugFind_ObjSopSimulate(Abc_Obj_t *pObj);
bool BugFind_CecSat(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2);

void BugFind_SimCec(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2);
bool BugFind_CecTwo(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2, bool sign,
                    Abc_Obj_t *pObj, Abc_Obj_t *pFanin);
bool BugFind_SimTwo(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2);

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Start / Stop the ssat package]

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/

void BugFind_Init(Abc_Frame_t *pAbc) {
  Cmd_CommandAdd(pAbc, "z Bug", "bugfind", Abc_CommandBugFind, 0);
  Cmd_CommandAdd(pAbc, "z Bug", "ntkprint", Abc_CommandBugFindPrint, 0);
}

void BugFind_End(Abc_Frame_t *pAbc) {}

/**Function*************************************************************

  Synopsis    [mux command]

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/

int Abc_CommandBugFind(Abc_Frame_t *pAbc, int argc, char **argv) {
  int c, fDelete1 = 0, fDelete2 = 0;
  char **pArgvNew;
  int nArgcNew;
  Abc_Ntk_t *pNtk1, *pNtk2;
  // abctime clkTotal = Abc_Clock();

  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "")) != EOF) {
    switch (c) {
      default:
        goto usage;
    }
  }

  pArgvNew = argv + globalUtilOptind;
  nArgcNew = argc - globalUtilOptind;
  if (nArgcNew != 2) {
    printf("Invalid command !\n");
    goto usage;
  }
  if (!BugFind_PrepareTwoNtks(stdout, NULL, pArgvNew, nArgcNew, &pNtk1,
                              &pNtk2, &fDelete1, &fDelete2))
    return 1;

  BugFind_SimCec(pNtk1, pNtk2);
  cout << endl;

  if (fDelete1) Abc_NtkDelete(pNtk1);
  if (fDelete2) Abc_NtkDelete(pNtk2);

  return 0;

usage:
  Abc_Print(-2, "usage: bugfind <blif> <blif> \n");
  return 1;
}

int Abc_CommandBugFindPrint(Abc_Frame_t *pAbc, int argc, char **argv) {
  Abc_Ntk_t *pNtk;
  pNtk = Abc_FrameReadNtk(pAbc);
  BugFind_PrintObj(pNtk);
  return 0;
}

// Vec_Ptr_t METHOD
// static inline int Vec_PtrSize( Vec_Ptr_t * p )
// static inline int Vec_PtrCap( Vec_Ptr_t * p )
// static inline void * Vec_PtrEntry( Vec_Ptr_t * p, int i )
// #define Vec_PtrForEachEntry( Type, vVec, pEntry, i )                                               \
//     for ( i = 0; (i < Vec_PtrSize(vVec)) && (((pEntry) = (Type)Vec_PtrEntry(vVec, i)), 1); i++ )
// #define Vec_PtrForEachEntryStart( Type, vVec, pEntry, i, Start )
// static inline void Vec_PtrPush( Vec_Ptr_t * p, void * Entry )
// static inline int  Vec_PtrFind( Vec_Ptr_t * p, void * Entry )
// static inline void Vec_PtrRemove( Vec_Ptr_t * p, void * Entry )
// static inline Vec_Ptr_t * Vec_PtrAlloc( int nCap )
// static inline void Vec_WrdSetEntry( Vec_Wrd_t * p, int i, word Entry )
void BugFind_SimCec(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2) {
  Vec_Ptr_t *dfs1;

  Abc_Obj_t *pObj, *pFanin, *pConst0, *pConst1;
  int i, j;
  bool sign = 0;

  dfs1 = Abc_NtkDfs(pNtk1, 0);
  pConst0 = Abc_NtkCreateNodeConst0(pNtk1);
  pConst0->pTemp = (void *)0;
  pConst1 = Abc_NtkCreateNodeConst1(pNtk1);
  pConst1->pTemp = (void *)ULONG_MAX;

  Vec_PtrForEachEntry(Abc_Obj_t *, dfs1, pObj, i) {
    Abc_ObjForEachFanin(pObj, pFanin, j) {
      Abc_ObjPatchFanin(pObj, pFanin, pConst0);
      if (BugFind_SimTwo(pNtk1, pNtk2)) {
        sign = 0;
        if (BugFind_CecTwo(pNtk1, pNtk2, sign, pObj, pFanin)) {
          Vec_PtrFree(dfs1);
          return;
        }
      }
      Abc_ObjPatchFanin(pObj, pConst0, pConst1);
      if (BugFind_SimTwo(pNtk1, pNtk2)) {
        sign = 1;
        if (BugFind_CecTwo(pNtk1, pNtk2, sign, pObj, pFanin)) {
          Vec_PtrFree(dfs1);
          return;
        }
      }
      Abc_ObjPatchFanin(pObj, pConst1, pFanin);
    }
    Abc_ObjTransferFanout(pObj, pConst0);
    if (BugFind_SimTwo(pNtk1, pNtk2)) {
      sign = 0;
      if (BugFind_CecTwo(pNtk1, pNtk2, sign, pObj, NULL)) {
        Vec_PtrFree(dfs1);
        return;
      }
    }
    Abc_ObjTransferFanout(pConst0, pConst1);
    if (BugFind_SimTwo(pNtk1, pNtk2)) {
      sign = 1;
      if (BugFind_CecTwo(pNtk1, pNtk2, sign, pObj, NULL)) {
        Vec_PtrFree(dfs1);
        return;
      }
    }
    Abc_ObjTransferFanout(pConst1, pObj);
  }
}

bool BugFind_CecTwo(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2, bool sign,
                    Abc_Obj_t *pObj, Abc_Obj_t *pFanin) {
  Abc_Ntk_t *pSNtk1, *pSNtk2;
  bool result;

  pSNtk1 = Abc_NtkDup(pNtk1);
  pSNtk2 = Abc_NtkDup(pNtk2);

  result = BugFind_CecSat(pSNtk1, pSNtk2);
  if (result) {
    cout << "$SA" << sign << "@" << Abc_ObjName(pObj);
    if (pFanin == NULL)
      cout << "[output";
    else
      cout << "[input:" << Abc_ObjName(pFanin);
    cout << "]" << endl;
  }

  Abc_NtkDelete(pSNtk1);
  Abc_NtkDelete(pSNtk2);
  return result;
}

bool BugFind_SimTwo(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2) {
  Vec_Ptr_t *dfs1, *dfs2;
  Abc_Obj_t *pPi, *pObj, *pPo;
  int i;

  dfs1 = Abc_NtkDfs(pNtk1, 0);
  dfs2 = Abc_NtkDfs(pNtk2, 0);

  Abc_NtkForEachPi(pNtk2, pPi, i) pPi->pTemp =
      (word *)(ABC_PTRUINT_T)Aig_ManRandom64(0);
  Vec_PtrForEachEntry(Abc_Obj_t *, dfs2, pObj, i) pObj->pTemp =
      (word *)(ABC_PTRUINT_T)BugFind_ObjSopSimulate(pObj);
  Abc_NtkForEachPo(pNtk2, pPo, i) pPo->pTemp = Abc_ObjFanin0(pPo)->pTemp;

  Abc_NtkForEachPi(pNtk1, pPi, i) pPi->pTemp = Abc_NtkPi(pNtk2, i)->pTemp;
  Vec_PtrForEachEntry(Abc_Obj_t *, dfs1, pObj, i) pObj->pTemp =
      (word *)BugFind_ObjSopSimulate(pObj);

  Vec_PtrFree(dfs1);
  Vec_PtrFree(dfs2);

  Abc_NtkForEachPo(pNtk1, pPo,
                   i) if (Abc_ObjFanin0(pPo)->pTemp !=
                          Abc_NtkPo(pNtk2, i)->pTemp) return false;

  return true;
}

// Using word !! (64 bits)
// ULONG_MAX = 64'b1111_..._1111;
//
word BugFind_ObjSopSimulate(Abc_Obj_t *pObj) {
  char *pCube, *pSop = (char *)pObj->pData;
  int nVars, Value, v;
  word ResOr, ResAnd, ResVar;
  assert(pSop && !Abc_SopIsExorType(pSop));
  // simulate the SOP of the node
  ResOr = 0;
  nVars = Abc_SopGetVarNum(pSop);
  Abc_SopForEachCube(pSop, nVars, pCube) {
    ResAnd = ULONG_MAX;
    Abc_CubeForEachVar(pCube, Value, v) {
      if (Value == '0')
        ResVar = ULONG_MAX ^
                 ((word)(ABC_PTRUINT_T)Abc_ObjFanin(pObj, v)->pTemp);
      else if (Value == '1')
        ResVar = (word)(ABC_PTRUINT_T)Abc_ObjFanin(pObj, v)->pTemp;
      else
        continue;
      ResAnd &= ResVar;
    }
    ResOr |= ResAnd;
  }
  // complement the result if necessary
  if (!Abc_SopGetPhase(pSop)) ResOr ^= ULONG_MAX;
  return ResOr;
}

bool BugFind_CecSat(Abc_Ntk_t *pNtk1, Abc_Ntk_t *pNtk2) {
  extern Abc_Ntk_t *Abc_NtkMulti(Abc_Ntk_t * pNtk, int nThresh,
                                 int nFaninMax, int fCnf, int fMulti,
                                 int fSimple, int fFactor);
  Abc_Ntk_t *pMiter;
  Abc_Ntk_t *pCnf;
  int RetValue;

  // get the miter of the two networks
  pMiter = Abc_NtkMiter(pNtk1, pNtk2, 1, 0, 0, 0);
  if (pMiter == NULL) {
    printf("Miter computation has failed.\n");
    return false;
  }
  RetValue = Abc_NtkMiterIsConstant(pMiter);
  if (RetValue == 0) {
    printf("Networks are NOT EQUIVALENT after structural hashing.\n");
    // report the error
    pMiter->pModel = Abc_NtkVerifyGetCleanModel(pMiter, 1);
    Abc_NtkVerifyReportError(pNtk1, pNtk2, pMiter->pModel);
    ABC_FREE(pMiter->pModel);
    Abc_NtkDelete(pMiter);
    return false;
  }
  if (RetValue == 1) {
    Abc_NtkDelete(pMiter);
    printf("Networks are equivalent after structural hashing.\n");
    return true;
  }

  // convert the miter into a CNF
  pCnf = Abc_NtkMulti(pMiter, 0, 100, 1, 0, 0, 0);
  Abc_NtkDelete(pMiter);
  if (pCnf == NULL) {
    printf("Renoding for CNF has failed.\n");
    return false;
  }

  // solve the CNF using the SAT solver
  RetValue =
      Abc_NtkMiterSat(pCnf, (ABC_INT64_T)0, (ABC_INT64_T)0, 0, NULL, NULL);
  ABC_FREE(pCnf->pModel);
  Abc_NtkDelete(pCnf);

  if (RetValue == -1)
    printf("Networks are undecided (SAT solver timed out).\n");
  else if (RetValue == 0) {
    printf("Networks are NOT EQUIVALENT after SAT.\n");
    return false;
  } else {
    printf("Networks are equivalent after SAT.\n");
    return true;
  }
  if (pCnf->pModel) Abc_NtkVerifyReportError(pNtk1, pNtk2, pCnf->pModel);
  return false;
}

int BugFind_PrepareTwoNtks(FILE *pErr, Abc_Ntk_t *pNtk, char **argv,
                           int argc, Abc_Ntk_t **ppNtk1,
                           Abc_Ntk_t **ppNtk2, int *pfDelete1,
                           int *pfDelete2) {
  int fCheck = 1;
  FILE *pFile;
  Abc_Ntk_t *pNtk1, *pNtk2;  // * pNtkTemp;
  int util_optind = 0;

  *pfDelete1 = 0;
  *pfDelete2 = 0;
  if (argc == util_optind) {  // use the spec
    if (pNtk == NULL) {
      fprintf(pErr, "Empty current network.\n");
      return 0;
    }
    if (pNtk->pSpec == NULL) {
      fprintf(pErr, "The external spec is not given.\n");
      return 0;
    }
    pFile = fopen(pNtk->pSpec, "r");
    if (pFile == NULL) {
      fprintf(pErr, "Cannot open the external spec file \"%s\".\n",
              pNtk->pSpec);
      return 0;
    } else
      fclose(pFile);
    pNtk1 = Abc_NtkDup(pNtk);
    pNtk2 = Io_Read(pNtk->pSpec, Io_ReadFileType(pNtk->pSpec), fCheck, 0);
    if (pNtk2 == NULL) return 0;
    *pfDelete1 = 1;
    *pfDelete2 = 1;
  } else if (argc == util_optind + 1) {
    if (pNtk == NULL) {
      fprintf(pErr, "Empty current network.\n");
      return 0;
    }
    pNtk1 = Abc_NtkDup(pNtk);
    pNtk2 = Io_Read(argv[util_optind], Io_ReadFileType(argv[util_optind]),
                    fCheck, 0);
    if (pNtk2 == NULL) return 0;
    *pfDelete1 = 1;
    *pfDelete2 = 1;
  } else if (argc == util_optind + 2) {
    pNtk1 = Io_Read(argv[util_optind], Io_ReadFileType(argv[util_optind]),
                    fCheck, 0);
    if (pNtk1 == NULL) return 0;
    pNtk2 = Io_Read(argv[util_optind + 1],
                    Io_ReadFileType(argv[util_optind + 1]), fCheck, 0);
    if (pNtk2 == NULL) {
      Abc_NtkDelete(pNtk1);
      return 0;
    }
    *pfDelete1 = 1;
    *pfDelete2 = 1;
  } else {
    fprintf(pErr, "Wrong number of arguments.\n");
    return 0;
  }

  *ppNtk1 = pNtk1;
  *ppNtk2 = pNtk2;
  return 1;
}

ABC_NAMESPACE_IMPL_END
