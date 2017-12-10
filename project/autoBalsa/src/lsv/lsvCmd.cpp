/**CFile****************************************************************

  FileName    [lsvCmd.cpp]

  SystemName  [ABC: Logic synthesis and verification
system.]

  PackageName [lsv: Logic Synthesis and Verification PA.]

  Synopsis    [command file.]

  Author      [Nian-Ze Lee]

  Affiliation [NTU]

  Date        [17, Sep., 2017.]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES ///
////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <vector>
#include "aig/aig/aig.h"
#include "base/abc/abc.h"
#include "base/main/mainInt.h"
#include "sat/cnf/cnf.h"

#include "aig/gia/giaAig.h"
#include "aig/saig/saig.h"
#include "base/abc/abc.h"
#include "base/acb/acbPar.h"
#include "base/cmd/cmd.h"
#include "base/main/main.h"
#include "base/main/mainInt.h"
#include "bdd/bbr/bbr.h"
#include "bdd/llb/llb.h"
#include "bool/kit/kit.h"
#include "bool/rpo/rpo.h"
#include "map/amap/amap.h"
#include "map/cov/cov.h"
#include "map/fpga/fpga.h"
#include "map/if/if.h"
#include "map/mpm/mpm.h"
#include "misc/tim/tim.h"
#include "opt/cgt/cgt.h"
#include "opt/cut/cut.h"
#include "opt/dar/dar.h"
#include "opt/fret/fretime.h"
#include "opt/fxch/Fxch.h"
#include "opt/fxu/fxu.h"
#include "opt/lpk/lpk.h"
#include "opt/mfs/mfs.h"
#include "opt/nwk/nwkMerge.h"
#include "opt/res/res.h"
#include "opt/ret/retInt.h"
#include "opt/sbd/sbd.h"
#include "opt/sfm/sfm.h"
#include "opt/sim/sim.h"
#include "proof/abs/abs.h"
#include "proof/acec/acec.h"
#include "proof/cec/cec.h"
#include "proof/dch/dch.h"
#include "proof/fra/fra.h"
#include "proof/fraig/fraig.h"
#include "proof/int/int.h"
#include "proof/pdr/pdr.h"
#include "proof/ssc/ssc.h"
#include "proof/ssw/ssw.h"
#include "sat/bmc/bmc.h"
#include "sat/cnf/cnf.h"
#include "sat/satoko/satoko.h"
#include "sat/xsat/xsat.h"

#include <limits.h>
#include <iostream>
using namespace std;

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS ///
////////////////////////////////////////////////////////////////////////

// export to mainInit.c
extern "C" void Lsv_Init(Abc_Frame_t *);
extern "C" void Lsv_End(Abc_Frame_t *);

// command functions
static int Abc_CommandMajFind(Abc_Frame_t *, int, char **);

// external functions defined in lsv package
extern void Lsv_NtkMajFind(Abc_Ntk_t *);
extern void Lsv_Ntk1SubFind(Abc_Frame_t *, Abc_Ntk_t *);
extern int Abc_Command1SubFind(Abc_Frame_t *, int, char **);

extern "C" Aig_Man_t *Abc_NtkToDar(Abc_Ntk_t *pNtk,
                                   int fExors,
                                   int fRegisters);
extern "C" void Abc_NtkCecSat(Abc_Ntk_t *pNtk1,
                              Abc_Ntk_t *_pNtkNew,
                              int nConfLimit,
                              int nInsLimit);
extern "C" Vec_Ptr_t *Abc_NtkDfs(Abc_Ntk_t *pNtk,
                                 int fCollectAll);
extern "C" word Aig_ManRandom64(int fReset);
extern "C" void Abc_NtkVerifyReportError(
    Abc_Ntk_t *pNtk1, Abc_Ntk_t *_pNtkNew, int *pModel);
extern "C" Abc_Ntk_t *Abc_NtkMulti(Abc_Ntk_t *pNtk,
                                   int nThresh,
                                   int nFaninMax, int fCnf,
                                   int fMulti, int fSimple,
                                   int fFactor);
extern "C" Abc_Obj_t *Abc_ObjFanin0(Abc_Obj_t *pObj);
extern "C" Abc_Obj_t *Abc_ObjFanin1(Abc_Obj_t *pObj);
class OneSubFind {
 public:
  OneSubFind(Abc_Frame_t *pAbc, Abc_Ntk_t *pNtk) {
    _pAbc = pAbc;
    _pNtk = pNtk;
    _pNewNtk = Abc_NtkDup(pNtk);
    _pSat = sat_solver_new();
    _var = Abc_NtkObjNum(pNtk);
    _subs.resize(Abc_NtkObjNum(pNtk));
  }
  void simInit();
  void satInit();
  void solve();

 private:
  // assert(Abc_NtkNumFanouts)
  // obj->Id == var
  bool inFaninCone(Abc_Obj_t *pParent, Abc_Obj_t *pChild);
  void propagate(Abc_Obj_t *pObj);
  bool sim(Abc_Obj_t *pOld, Abc_Obj_t *pNew);
  bool sat(Abc_Obj_t *pOld, Abc_Obj_t *pNew);
  bool sat2(Abc_Obj_t *pOld, Abc_Obj_t *pNew);
  void disableObj(int var);
  void enableObj(int var);
  bool addAndGateClause(int input0, int input1, int output,
                        vector<int> auxiliaryLits);
  int getVar() { return ++_var; }

  Abc_Frame_t *_pAbc;
  vector<Abc_Obj_t *> _updateObjs;
  Abc_Ntk_t *_pNtk;
  Abc_Ntk_t *_pNewNtk;
  sat_solver *_pSat;
  int _var;
  vector<int> _ctrlLits;  // lit_sign (_ctrlLit[var]) =
                          // true, disable the
  // gate[var] with its fanouts
  vector<vector<int>> _subs;
};

int objToLit(Abc_Obj_t *pObj) {
  Abc_Obj_t *pR = Abc_ObjRegular(pObj);
  return 2 * pR->Id + Abc_ObjIsComplement(pObj);
}

void OneSubFind::satInit() {
  Abc_Obj_t *pObj;
  int out;
  int i;
  _ctrlLits.resize(_var);
  Abc_NtkForEachObj(_pNtk, pObj, i) _ctrlLits[pObj->Id] =
      getVar() * 2 + 1;

  // assert each AndGate
  Abc_NtkForEachNode(_pNtk, pObj, i) {
    vector<int> aux;
    Abc_Obj_t *in0 = Abc_ObjChild0(pObj);
    Abc_Obj_t *in1 = Abc_ObjChild1(pObj);
    int varIn0 = Abc_ObjFanin0(pObj)->Id;
    int varIn1 = Abc_ObjFanin1(pObj)->Id;
    assert(varIn0 < _ctrlLits.size());
    assert(varIn1 < _ctrlLits.size());
    aux.push_back(_ctrlLits[varIn0] ^ 1);
    aux.push_back(_ctrlLits[varIn1] ^ 1);
    out = 2 * pObj->Id;
    assert(addAndGateClause(objToLit(in0), objToLit(in1),
                            out, aux));
  }

  // assert po is same as fanin
  Abc_NtkForEachPo(_pNtk, pObj, i) {
    // (inCtrl + in + out')(inCtrl + in' + out)
    int inLit = objToLit(Abc_ObjChild0(pObj));
    int outputLit = pObj->Id * 2;
    int inCtrlLit = _ctrlLits[inLit / 2] ^ 1;

    vector<vector<int>> clauses(3);
    clauses[0].push_back(lit_neg(inLit));
    clauses[0].push_back(outputLit);
    clauses[0].push_back(inCtrlLit);

    clauses[1].push_back(inLit);
    clauses[1].push_back(lit_neg(outputLit));
    clauses[1].push_back(inCtrlLit);

    assert(sat_solver_addclause(
        _pSat, &clauses[0][0],
        &clauses[0][0] + clauses[0].size()));

    assert(sat_solver_addclause(
        _pSat, &clauses[1][0],
        &clauses[1][0] + clauses[1].size()));
  }

  // set standard ntk
  int baseVar = _var;
  Abc_NtkForEachObj(_pNtk, pObj, i) {
    if (Abc_ObjIsPo(pObj)) {
      vector<vector<int>> clauses(2);
      int faninVar = Abc_ObjFanin0(pObj)->Id + baseVar;
      int faninLit = faninVar * 2 + Abc_ObjFaninC0(pObj);
      int pObjLit = (pObj->Id + baseVar) * 2;
      clauses[0].push_back(faninLit);
      clauses[0].push_back(lit_neg(pObjLit));

      clauses[1].push_back(lit_neg(faninLit));
      clauses[1].push_back(pObjLit);

      assert(sat_solver_addclause(
          _pSat, &clauses[0][0],
          &clauses[0][0] + clauses[0].size()));
      assert(sat_solver_addclause(
          _pSat, &clauses[1][0],
          &clauses[1][0] + clauses[1].size()));
    } else if (Abc_ObjIsNode(pObj)) {
      // a&b <=> c     To    (a'+b'+c)(ab+c')
      int fanin0Var = Abc_ObjFanin0(pObj)->Id + baseVar;
      if (Abc_ObjIsPi(Abc_ObjFanin0(pObj)))
        fanin0Var = Abc_ObjFanin0(pObj)->Id;
      int fanin0Lit = fanin0Var * 2 + Abc_ObjFaninC0(pObj);
      int fanin1Var = Abc_ObjFanin1(pObj)->Id + baseVar;
      if (Abc_ObjIsPi(Abc_ObjFanin1(pObj)))
        fanin1Var = Abc_ObjFanin1(pObj)->Id;
      int fanin1Lit = fanin1Var * 2 + Abc_ObjFaninC1(pObj);
      int pObjLit = (pObj->Id + baseVar) * 2;
      vector<vector<int>> clauses(3);
      clauses[0].push_back(lit_neg(fanin0Lit));
      clauses[0].push_back(lit_neg(fanin1Lit));
      clauses[0].push_back(pObjLit);

      clauses[1].push_back(fanin0Lit);
      clauses[1].push_back(lit_neg(pObjLit));

      clauses[2].push_back(fanin1Lit);
      clauses[2].push_back(lit_neg(pObjLit));

      assert(sat_solver_addclause(
          _pSat, &clauses[0][0],
          &clauses[0][0] + clauses[0].size()));
      assert(sat_solver_addclause(
          _pSat, &clauses[1][0],
          &clauses[1][0] + clauses[1].size()));
      assert(sat_solver_addclause(
          _pSat, &clauses[2][0],
          &clauses[2][0] + clauses[2].size()));
    }
  }

  // xor output, or together
  _var += Abc_NtkObjNum(_pNtk);
  vector<int> xorLits;
  // a XOR b = c   to (a'+b'+c')(a+b+c')(a+b'+c)(a'+b+c)
  Abc_NtkForEachPo(_pNtk, pObj, i) {
    vector<vector<int>> clauses(4);
    int newOutputLit = pObj->Id * 2;
    int oldOutputLit = (pObj->Id + baseVar) * 2;
    int xorLit = getVar() * 2;
    xorLits.push_back(xorLit);
    clauses[0].push_back(lit_neg(newOutputLit));
    clauses[0].push_back(lit_neg(oldOutputLit));
    clauses[0].push_back(lit_neg(xorLit));

    clauses[1].push_back(newOutputLit);
    clauses[1].push_back(oldOutputLit);
    clauses[1].push_back(lit_neg(xorLit));

    clauses[2].push_back(newOutputLit);
    clauses[2].push_back(lit_neg(oldOutputLit));
    clauses[2].push_back(xorLit);

    clauses[3].push_back(lit_neg(newOutputLit));
    clauses[3].push_back(oldOutputLit);
    clauses[3].push_back(xorLit);
    assert(sat_solver_addclause(
        _pSat, &clauses[0][0],
        &clauses[0][0] + clauses[0].size()));

    assert(sat_solver_addclause(
        _pSat, &clauses[1][0],
        &clauses[1][0] + clauses[1].size()));

    assert(sat_solver_addclause(
        _pSat, &clauses[2][0],
        &clauses[2][0] + clauses[2].size()));

    assert(sat_solver_addclause(
        _pSat, &clauses[3][0],
        &clauses[3][0] + clauses[3].size()));
  }

  // or together
  assert(sat_solver_addclause(
      _pSat, &xorLits[0], &xorLits[0] + xorLits.size()));
  int status =
      sat_solver_solve(_pSat, &_ctrlLits[0],
                       &_ctrlLits[0] + _ctrlLits.size(),
                       10000000, 10000000, 0, 0);
  assert(status == l_False);
}

void OneSubFind::simInit() {
  Abc_Obj_t *pObj, *pFanin;
  int i;
  Abc_NtkForEachObj(_pNtk, pObj, i) pObj->fMarkA = false;

  Abc_NtkForEachPi(_pNtk, pObj, i) {
    pObj->iData = (int)Aig_ManRandom64(0);
    // cerr << "pObj->iData " << pObj->iData << endl;
    pObj->fMarkA = true;
  }

  Abc_NtkForEachPo(_pNtk, pObj, i) {
    assert(Abc_ObjFaninNum(pObj) == 1);
    assert(!pObj->fMarkA);
    propagate(pObj);
    pObj->iTemp = pObj->iData;
    // cerr << "pObj->iTemp " << pObj->iTemp << endl;
  }
  for (i = 0; i < _updateObjs.size(); ++i)
    _updateObjs[i]->fMarkA = false;
  _updateObjs.clear();
}

bool OneSubFind::inFaninCone(Abc_Obj_t *pParent,
                             Abc_Obj_t *pChild) {
  if (Abc_ObjIsPi(pParent)) return false;
  Abc_Obj_t *c0 = Abc_ObjFanin0(pParent);
  if (!Abc_ObjIsPo(pParent))
    if (c0 == pChild) return true;
  if (!Abc_ObjIsPi(c0))
    if (inFaninCone(c0, pChild)) return true;
  if (Abc_ObjIsPo(pParent)) return false;

  Abc_Obj_t *c1 = Abc_ObjFanin1(pParent);
  if (c1 == pChild) return true;
  if (!Abc_ObjIsPi(c1))
    if (inFaninCone(c1, pChild)) return true;
  return false;
}
void OneSubFind::solve() {
  int i, j;
  Abc_Obj_t *pOld, *pNew, *pFanout;
  Abc_NtkForEachObj(_pNtk, pOld, i) pOld->fMarkA = 0;
  _pNtk = Abc_NtkStrash(_pNtk, 0, 1, 0);
  _pNewNtk = Abc_NtkDup(_pNtk);
  simInit();
  satInit();
  assert(Abc_NtkPo(_pNtk, 1)->iTemp);
  vector<vector<Abc_Obj_t *>> replacePair;
  Abc_NtkForEachNode(_pNtk, pOld, i) {
    Abc_NtkForEachNode(_pNtk, pNew, j) {
      if (i == j) continue;
      if (Abc_ObjIsPo(pOld) || Abc_ObjIsPo(pNew)) continue;
      if (inFaninCone(pNew, pOld)) continue;
      if (sim(pOld, pNew)) {
        vector<Abc_Obj_t *> tmp(2);
        tmp[0] = pOld;
        tmp[1] = pNew;
        replacePair.push_back(tmp);
      }
    }
  }
  int repeatTimes =
      max(3, (int)log(Abc_NtkNodeNum(_pNtk))) * 10;
  for (int t = 0; t < repeatTimes; ++t) {
    vector<vector<Abc_Obj_t *>> tmp = replacePair;
    replacePair.clear();
    simInit();
    for (i = 0; i < tmp.size(); ++i) {
      if (sim(tmp[i][0], tmp[i][1]))
        replacePair.push_back(tmp[i]);
    }
  }
  Abc_Obj_t *pObj;
  Abc_NtkForEachObj(_pNtk, pObj, i) pObj->iData = 0;
  for (i = 0; i < replacePair.size(); ++i) {
    if (sat2(replacePair[i][0], replacePair[i][1]))
      _subs[replacePair[i][0]->Id].push_back(
          replacePair[i][1]->Id);
  }

  for (i = 1; i < _subs.size(); ++i) {
    cout << "n" << i << ":";
    for (j = 0; j < _subs[i].size(); ++j)
      cout << " n" << _subs[i][j];
    cout << endl;
  }

  Abc_NtkForEachObj(_pNtk, pOld, i) {
    pOld->fMarkA = false;
    pOld->pData = 0;
  }
}

void OneSubFind::propagate(Abc_Obj_t *pObj) {
  Abc_Obj_t *pFanout;
  int i;
  if (pObj->fMarkA) return;
  pObj->fMarkA = true;
  _updateObjs.push_back(pObj);
  if (Abc_ObjIsPi(pObj))
    Abc_ObjForEachFanout(pObj, pFanout, i)
        propagate(pFanout);
  assert(!Abc_ObjIsPi(pObj));

  if (!Abc_ObjFanin0(pObj)->fMarkA)
    propagate(Abc_ObjFanin0(pObj));

  int input0 = Abc_ObjFanin0(pObj)->iData;
  if (Abc_ObjFaninC0(pObj)) input0 = ~input0;

  if (Abc_ObjIsPo(pObj)) {
    assert(Abc_ObjFaninNum(pObj) == 1);
    pObj->iData = Abc_ObjFanin0(pObj)->iData;
    // cerr << "p " << pObj->Id << " "
    //      << bitset<32>(pObj->iData).to_string() << endl;
    return;
  }

  assert(Abc_ObjFaninNum(pObj) == 2);
  if (!Abc_ObjFanin1(pObj)->fMarkA)
    propagate(Abc_ObjFanin1(pObj));
  int input1 = Abc_ObjFanin1(pObj)->iData;
  if (Abc_ObjFaninC1(pObj)) input1 = ~input1;
  pObj->iData = input0 & input1;
  // cerr << "p " << pObj->Id << " "
  //      << bitset<32>(pObj->iData).to_string() << endl;
}
void collectConePo(Abc_Obj_t *pObj,
                   vector<Abc_Obj_t *> &pPos) {
  if (Abc_ObjIsPo(pObj))
    pPos.push_back(pObj);
  else {
    Abc_Obj_t *pFanout;
    int i;
    Abc_ObjForEachFanout(pObj, pFanout, i) {
      collectConePo(pFanout, pPos);
    }
  }
}

bool OneSubFind::sim(Abc_Obj_t *pOld, Abc_Obj_t *pNew) {
  // cerr << "sim " << pOld->Id << " " << pNew->Id << endl;
  Abc_Obj_t *pFanout, *pPo;
  int i;
  Abc_NtkForEachObj(_pNtk, pPo, i) {
    if (!Abc_ObjIsPi(pPo)) assert(!pPo->fMarkA);
  }

  vector<Abc_Obj_t *> oldFanouts, pPos;
  collectConePo(pOld, pPos);
  Abc_ObjForEachFanout(pOld, pFanout, i)
      oldFanouts.push_back(pFanout);
  for (i = 0; i < oldFanouts.size(); ++i) {
    Abc_ObjDeleteFanin(oldFanouts[i], pOld);
    Abc_ObjAddFanin(oldFanouts[i], pNew);
  }
  assert(_updateObjs.empty());
  bool rtn = true;
  // assert(Abc_NtkPo(_pNtk, 1)->iTemp);
  for (i = 0; i < pPos.size(); ++i) {
    // cerr << "pPos[i]->Id " << pPos[i]->Id << endl;
    if (pPos[i]->fMarkA) continue;
    propagate(pPos[i]);
    if (pPos[i]->iData != pPos[i]->iTemp) {
      rtn = false;
      break;
    }
  }
  // assert(Abc_NtkPo(_pNtk, 1)->iTemp);

  // if (pOld->Id == 17 && pNew->Id == 16)
  //   Abc_NtkForEachObj(_pNtk, pPo, i) {
  //     cerr << "i " << i << " pPo->fMarkA " << pPo->fMarkA
  //          << endl;
  //     if (!pPo->fMarkA) continue;
  //     if (Abc_ObjIsPi(pPo)) {
  //       // assert(pPo->fMarkA);
  //       cerr << "pPi " << i << " "
  //            << bitset<32>(pPo->iData).to_string() <<
  //            endl;

  //     } else if (Abc_ObjIsPo(pPo)) {
  //       // assert(!pPo->fMarkA);
  //       cerr << "pPo " << i << " "
  //            << bitset<32>(pPo->iData).to_string() <<
  //            endl;
  //       cerr << "pPo " << i << " "
  //            << bitset<32>(pPo->iTemp).to_string() <<
  //            endl;

  //       cerr << "Abc_ObjFanin0(pPo)->Id "
  //            << Abc_ObjFanin0(pPo)->Id << endl;

  //     } else if (Abc_ObjIsNode(pPo)) {
  //       // assert(!pPo->fMarkA);
  //       cerr << "pPNode " << i << endl
  //            << bitset<32>(pPo->iData).to_string() <<
  //            endl;
  //       cerr << bitset<32>(
  //                   Abc_NtkObj(_pNtk,
  //                              Abc_ObjFanin0(pPo)->Id)
  //                       ->iData)
  //                   .to_string()
  //            << endl;
  //       cerr << bitset<32>(
  //                   Abc_NtkObj(_pNtk,
  //                              Abc_ObjFanin1(pPo)->Id)
  //                       ->iData)
  //                   .to_string()
  //            << endl;

  //       cerr << "Abc_ObjFanin0(pPo)->Id "
  //            << Abc_ObjFanin0(pPo)->Id << endl;
  //       cerr << "Abc_ObjFanin1(pPo)->Id "
  //            << Abc_ObjFanin1(pPo)->Id << endl;
  //     }
  //   }

  for (i = 0; i < _updateObjs.size(); ++i)
    _updateObjs[i]->fMarkA = false;
  _updateObjs.clear();

  for (i = 0; i < oldFanouts.size(); ++i) {
    Abc_ObjDeleteFanin(oldFanouts[i], pNew);
    Abc_ObjAddFanin(oldFanouts[i], pOld);
  }

  // cerr << "sim: " << rtn << endl;
  return rtn;
}

bool OneSubFind::sat2(Abc_Obj_t *pOld, Abc_Obj_t *pNew) {
  // cerr << "sat2 "  << pOld->Id << " " << pNew->Id <<
  // endl;
  Abc_Ntk_t *pNtk1, *pNtk2;
  int i;
  Abc_Obj_t *pFanout, *pObj;
  pNtk1 = Abc_NtkDup(_pNewNtk);
  pNtk2 = Abc_NtkDup(_pNewNtk);
  Abc_NtkForEachObj(pNtk1, pObj, i) pObj->fMarkA = 0;
  Abc_NtkForEachObj(pNtk2, pObj, i) pObj->fMarkA = 0;

  pOld = Abc_NtkObj(pNtk2, pOld->Id);

  pNew = Abc_NtkObj(pNtk2, pNew->Id);

  vector<Abc_Obj_t *> pFanouts;
  Abc_ObjForEachFanout(pOld, pFanout, i)
      pFanouts.push_back(pFanout);
  for (i = 0; i < pFanouts.size(); ++i) {
    Abc_ObjDeleteFanin(pFanouts[i], pOld);
    Abc_ObjAddFanin(pFanouts[i], pNew);
  }
  pNtk2 = Abc_NtkStrash(pNtk2, 0, 1, 0);
  pNtk1 = Abc_NtkStrash(pNtk1, 0, 1, 0);
  // Abc_FrameReplaceCurrentNetwork(_pAbc, pNtk2);
  // Abc_FrameReplaceCurrentNetwork(_pAbc, pNtk1);
  // return 0;
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
    printf(
        "Networks are NOT EQUIVALENT after structural "
        "hashing.\n");
    // report the error
    pMiter->pModel = Abc_NtkVerifyGetCleanModel(pMiter, 1);
    Abc_NtkVerifyReportError(pNtk1, pNtk2, pMiter->pModel);
    ABC_FREE(pMiter->pModel);
    Abc_NtkDelete(pMiter);
    return false;
  }
  if (RetValue == 1) {
    Abc_NtkDelete(pMiter);
    printf(
        "Networks are equivalent after structural "
        "hashing.\n");
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
  RetValue = Abc_NtkMiterSat(pCnf, (ABC_INT64_T)0,
                             (ABC_INT64_T)0, 0, NULL, NULL);
  ABC_FREE(pCnf->pModel);
  Abc_NtkDelete(pCnf);
  ABC_FREE(pNtk2);
  ABC_FREE(pNtk1);
  if (RetValue == -1)
    printf(
        "Networks are undecided (SAT solver timed out).\n");
  else if (RetValue == 0) {
    printf("Networks are NOT EQUIVALENT after SAT.\n");
    return false;
  } else {
    printf("Networks are equivalent after SAT.\n");
    return true;
  }
  if (pCnf->pModel)
    Abc_NtkVerifyReportError(pNtk1, pNtk2, pCnf->pModel);
  return false;
}

// return true means SAT, diff outputs
bool OneSubFind::sat(Abc_Obj_t *pOld, Abc_Obj_t *pNew) {
  // cerr << "sat " << pOld->Id << " " << pNew->Id << endl;
  disableObj(pOld->Id);
  Abc_Obj_t *pFanout;
  int anotherFaninLit, lit = pNew->Id * 2;
  int i;
  vector<int> aux(2);
  int var = getVar();
  assert(Abc_ObjFanoutNum(pOld));
  Abc_ObjForEachFanout(pOld, pFanout, i) {
    assert(!Abc_ObjIsComplement(pFanout));
    if (Abc_ObjIsPo(pFanout)) {
      vector<vector<int>> clause(2);
      clause[0].push_back(var * 2);
      clause[0].push_back(lit + 1);
      clause[0].push_back(pFanout->Id * 2);

      clause[1].push_back(var * 2);
      clause[1].push_back(lit);
      clause[1].push_back(pFanout->Id * 2 + 1);

      if (!sat_solver_addclause(_pSat, &clause[0][0],
                                &clause[0][0] + 3))
        return false;
      if (!sat_solver_addclause(_pSat, &clause[1][0],
                                &clause[1][0] + 3))
        return false;

      continue;
    }
    if (Abc_ObjFanin0(pFanout) == pOld) {
      anotherFaninLit = objToLit(Abc_ObjChild1(pFanout));
      lit += Abc_ObjFaninC0(pFanout);
    } else {
      assert(Abc_ObjFanin1(pFanout) == pOld);
      anotherFaninLit = objToLit(Abc_ObjChild0(pFanout));
      lit += Abc_ObjFaninC1(pFanout);
    }
    aux[0] = var * 2;
    aux[1] = var * 2;
    if (!addAndGateClause(anotherFaninLit, lit,
                          pFanout->Id * 2, aux))
      return false;
  }
  _ctrlLits.push_back(var * 2 + 1);
  int status =
      sat_solver_solve(_pSat, &_ctrlLits[0],
                       &_ctrlLits[0] + _ctrlLits.size(),
                       10000000, 10000000, 0, 0);
  _ctrlLits.back() = lit_neg(_ctrlLits.back());
  bool ret = 0;
  if (status == l_False)
    ret = false;
  else if (status == l_True)
    ret = true;
  else {
    printf("undef\n");
    assert(0);
  }
  // cerr << "sat " << ret << endl;
  enableObj(pOld->Id);
  return ret;
}

void Lsv_Init(Abc_Frame_t *pAbc) {
  Cmd_CommandAdd(pAbc, "z LSV", "MAJ_find",
                 Abc_CommandMajFind, 0);
  Cmd_CommandAdd(pAbc, "z LSV", "1subfind",
                 Abc_Command1SubFind, 0);
}

void Lsv_End(Abc_Frame_t *pAbc) {}

int Abc_Command1SubFind(Abc_Frame_t *pAbc, int argc,
                        char **argv) {
  int fCheck = 1;
  int util_optind = 0;
  Abc_Ntk_t *pNtk =
      Io_Read(argv[1], Io_ReadFileType(argv[1]), fCheck, 0);
  if (!pNtk) {
    cerr << "pNtk is NULL\n";
    return 1;
  }
  if (!Abc_NtkIsStrash(pNtk))
    pNtk = Abc_NtkStrash(pNtk, 0, 1, 0);

  // Abc_Ntk_t *pNtk1 = Abc_NtkDup(pNtk);
  // Abc_Ntk_t *pNtk2 = Abc_NtkDup(pNtk);
  // Abc_Obj_t *pOld = Abc_NtkObj(pNtk2, 112);
  // Abc_Obj_t *pNew = Abc_NtkObj(pNtk2, 132);
  // Abc_ObjTransferFanout(pOld, pNew);
  // Abc_NtkMiter(pNtk1, pNtk2, 1, 0, 0, 0);

  // return 0;
  Abc_FrameReplaceCurrentNetwork(pAbc, pNtk);
  OneSubFind oneSubFind(pAbc, pNtk);

  oneSubFind.solve();
  return 0;
}

void OneSubFind::disableObj(int var) {
  assert(lit_sign(_ctrlLits[var]));
  _ctrlLits[var] = lit_neg(_ctrlLits[var]);
};

void OneSubFind::enableObj(int var) {
  assert(!lit_sign(_ctrlLits[var]));
  _ctrlLits[var] = lit_neg(_ctrlLits[var]);
};

bool OneSubFind::addAndGateClause(
    int input0, int input1, int output,
    vector<int> auxiliaryLits) {
  // input0 & input1 <=> output
  // =>(a'+b'+c + ctrl0 + ctrl1)(a+c'+ ctrl0)(b+c'+
  // ctrl1)
  vector<vector<int>> clauses(3);
  clauses[0].push_back(lit_neg(input0));
  clauses[0].push_back(lit_neg(input1));
  clauses[0].push_back(output);
  clauses[0].push_back(auxiliaryLits[0]);
  clauses[0].push_back(auxiliaryLits[1]);

  clauses[1].push_back(lit_neg(output));
  clauses[1].push_back(input0);
  clauses[1].push_back(auxiliaryLits[0]);

  clauses[2].push_back(lit_neg(output));
  clauses[2].push_back(input1);
  clauses[2].push_back(auxiliaryLits[1]);

  if (!sat_solver_addclause(
          _pSat, &clauses[0][0],
          &clauses[0][0] + clauses[0].size()) ||
      !sat_solver_addclause(
          _pSat, &clauses[1][0],
          &clauses[1][0] + clauses[1].size()) ||
      !sat_solver_addclause(
          _pSat, &clauses[2][0],
          &clauses[2][0] + clauses[2].size())) {
    return false;
  }
  return true;
}

void Lsv_Ntk1SubFind(Abc_Frame_t *pAbc, Abc_Ntk_t *pNtk) {
  // Abc_Ntk_t *_pNtkNew = Abc_NtkDup(pNtk);
  // Abc_Obj_t *pObj, *pFanin, *pPo, *pPoNew;
  // int i, j, k;
  // Abc_NtkObjNum(pNtk);
  // vector<int>
  // LitOfRmConctBtwFanouts(Abc_NtkObjNum(pNtk));
  // Abc_FrameReplaceCurrentNetwork(pAbc, pNtk);
  // sat_solver *_pSat = sat_solver_new();
  // var = Abc_NtkObjNum(pNtk);
  // int input0, input1, output0, output1, output;
  // Abc_NtkForEachObj(pNtk, pObj, i) {
  //   if (!i) continue;
  //   if (Abc_ObjIsPi(pObj) || Abc_ObjIsPo(pObj))
  //   continue;
  //   input0 = Abc_ObjFanin0(pObj)->Id * 2 +
  //   Abc_ObjFaninC0(pObj);
  //   input1 = Abc_ObjFanin1(pObj)->Id * 2 +
  //   Abc_ObjFaninC1(pObj);
  //   output = pObj->Id * 2;
  //   if (!addAndGateClause(_pSat, input0, input1,
  //   output,
  //   0)) {  // for
  //   pNtk
  //     cerr << "unsat\n";
  //     return;
  //   }

  //   output = (var + pObj->Id) * 2;  // for _pNtkNew
  //   if (!addAndGateClause(_pSat, input0, input1,
  //   output,
  //   0)) {
  //     cerr << "unsat\n";
  //     return;
  //   }
  // }
  // cerr << "inter node clause created.\n";

  // var = Abc_NtkObjNum(pNtk) * 2 + Abc_NtkPoNum(pNtk) *
  // 2;
  // vector<int> outputLits;
  // Abc_NtkForEachPo(pNtk, pPo, i) {
  //   pPoNew = Abc_NtkPo(_pNtkNew, i);
  //   input0 = Abc_ObjFanin0(pPo)->Id * 2;
  //   input1 = (Abc_ObjFanin0(pPo)->Id +
  //   Abc_NtkObjNum(pNtk)) * 2;
  //   output = var * 2 + i * 2;
  //   // output0 = Abc_NtkObjNum(pNtk) * 4 + i * 4;
  //   // output1 = Abc_NtkObjNum(pNtk) * 4 + i * 4 + 2;
  //   if (!addXorGateClause(input0, input1, output)) {
  //     cerr << "unsat\n";
  //     return;
  //   }
  //   outputLits.push_back(output);
  // }

  // input0 = outputLits[0];
  // var = Abc_NtkObjNum(pNtk) * 2 + Abc_NtkPoNum(pNtk) *
  // 3;
  // for (i = 1; i < outputLits.size(); ++i) {
  //   output = var++ * 2;
  //   input1 = outputLits[i];
  //   if (!addOrGateClause(_pSat, input0, input1,
  //   output))
  //   {
  //     cerr << "unsat\n";
  //     return;
  //   }
  //   input0 = output;
  // }

  // if (outputLits.size() == 1) output = outputLits[0];
  // if (!sat_solver_addclause(_pSat, &output, &output +
  // 1))
  // {
  //   cerr << "unsat\n";
  //   return;
  // }
  // int status =
  //     sat_solver_solve(_pSat,
  //     &_ctrlLits[0],&_ctrlLits[0]+&_ctrlLits[0] 10000000,
  //     10000000, 0, 0);

  // if (status == l_False) {
  //   printf("unsat\n");
  // } else if (status == l_True)
  //   printf("sat\n");
  // else {
  //   printf("undef\n");
  //   assert(0);
  // }
  // return;
}

int Abc_CommandMajFind(Abc_Frame_t *pAbc, int argc,
                       char **argv) {
  // TODO:
  // step.1: get the current network
  // step.2: check whether the current network is strashed
  // step.3: call Lsv_NtkMajFind() to report MAJ gates
  // MuxCommandMuxFind(pAbc, argc, argv);

  Abc_Ntk_t *pNtk = Abc_FrameReadNtk(pAbc);
  if (!pNtk) return 1;
  if (Abc_NtkIsStrash(pNtk))
    Lsv_NtkMajFind(pNtk);
  else {
    pNtk = Abc_NtkStrash(pNtk, 0, 1, 0);
    Lsv_NtkMajFind(pNtk);
    Abc_NtkDelete(pNtk);
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE ///
////////////////////////////////////////////////////////////////////////

ABC_NAMESPACE_IMPL_END
