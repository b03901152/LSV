#include <iostream>
#include <map>
#include <cassert>

using namespace std;
/**CFile****************************************************************

  FileName    [lsvMajFind.cpp]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [lsv: Logic Synthesis and Verification PA.]

  Synopsis    [procedure for finding MAJ gates.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [17, Sep., 2017.]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "base/main/mainInt.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////
 
/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
bool equalToOne(Abc_Obj_t * pA, Abc_Obj_t * pB, Abc_Obj_t * pC) {
  if (pA == pB)
    return pA != pC;
  else
    return pA == pC; 
}

Abc_Obj_t *isSameFanin(Abc_Obj_t * pA, Abc_Obj_t * pB, Abc_Obj_t * pC, Abc_Obj_t * pD) {
  bool pC_same = pC == pA || pC == pB;
  bool pD_same = pD == pA || pD == pB;
  if (pC_same == pD_same)
    return (Abc_Obj_t*)NULL;
  if (pC_same) {
    if (pC == pA)
      return pA;
    else
      return pB;
  }
  else if (pD_same) {
    if (pD == pA)
      return pA;
    else
      return pB;
  }
  assert(0);
  return (Abc_Obj_t*)NULL;
}

bool isConstOrPi(Abc_Obj_t* p) {
  return Abc_ObjIsPi(p) || Abc_AigNodeIsConst(p);
}

Abc_Obj_t* commonInput(Abc_Obj_t *a, Abc_Obj_t *b) {
  if (isConstOrPi(a) || isConstOrPi(b)) return NULL;
  Abc_Obj_t *input_a0 = Abc_ObjChild0(a);
  Abc_Obj_t *input_a1 = Abc_ObjChild1(a);
  Abc_Obj_t *input_b0 = Abc_ObjChild0(b);
  Abc_Obj_t *input_b1 = Abc_ObjChild1(b);

  if (input_a0 == input_b0 || input_a0 == input_b1)
    return input_a0;

  if (input_a1 == input_b0 || input_a1 == input_b1)
    return input_a1;

  return NULL;
}

bool checkCase1(Abc_Obj_t *G9) {
  assert(G9 ==  Abc_ObjRegular(G9));
  Abc_Obj_t *G8 = Abc_ObjChild0(G9);
  Abc_Obj_t *G5 = Abc_ObjChild1(G9);
  if (Abc_ObjIsComplement(G8) == Abc_ObjIsComplement(G5))
    return false;
  if (isConstOrPi(G8) || isConstOrPi(G5))
    return false;
  if (Abc_ObjIsComplement(G8))
    swap(G8,G5);


  G8 = Abc_ObjRegular(G8);
  G5 = Abc_ObjRegular(G5);
  Abc_Obj_t *G6 = Abc_ObjChild0(G8);
  Abc_Obj_t *G7 = Abc_ObjChild1(G8);
  if (!Abc_ObjIsComplement(G6) || !Abc_ObjIsComplement(G7))
    return false;
  G6 = Abc_ObjRegular(G6);
  G7 = Abc_ObjRegular(G7);
  Abc_Obj_t *a = commonInput(G5, G6);
  Abc_Obj_t *b = commonInput(G5, G7);
  Abc_Obj_t *c = commonInput(G6, G7);

  bool succ = false;
  if (a != b && b != c && c != a)
    succ = true;
  if (!a || !b || !c)
    succ = false;
  if (succ) {
    Abc_Obj_t *aR = Abc_ObjRegular(a);
    Abc_Obj_t *bR = Abc_ObjRegular(b);
    Abc_Obj_t *cR = Abc_ObjRegular(c);
  
    cout << Abc_ObjId(G9) << " = MAJ("
    << (Abc_ObjIsComplement(a) ? string("-") : string("")) << Abc_ObjId(aR) << ", "
    << (Abc_ObjIsComplement(b) ? string("-") : string("")) << Abc_ObjId(bR) << ", "
    << (Abc_ObjIsComplement(c) ? string("-") : string("")) << Abc_ObjId(cR) << ")\n";
    // 25 = MAJ(5, −14, 22)
  }
  return succ;
}

bool same2InputButInverse(Abc_Obj_t *i, Abc_Obj_t *_i) {
  if (isConstOrPi(i) || isConstOrPi(_i))
    return false;
  Abc_Obj_t *in0, *in1, *_in0, *_in1;
  in0 = Abc_ObjChild0(i);
  in1 = Abc_ObjChild1(i);
  _in0 = Abc_ObjChild0(_i);
  _in1 = Abc_ObjChild1(_i);
  Abc_Obj_t *r0,*r1, *_r0, *_r1;
  r0 = Abc_ObjRegular(in0);
  r1 = Abc_ObjRegular(in1);
  _r0 = Abc_ObjRegular(_in0);
  _r1 = Abc_ObjRegular(_in1);
  if (r0 != _r0)
    swap(in0, in1);

  r0 = Abc_ObjRegular(in0);
  r1 = Abc_ObjRegular(in1);
  _r0 = Abc_ObjRegular(_in0);
  _r1 = Abc_ObjRegular(_in1);
  bool bb0;
  bool bb1;
  bool _b0;
  bool _b1;
  bb0 = Abc_ObjIsComplement(in0);
  bb1 = Abc_ObjIsComplement(in1);
  _b0 = Abc_ObjIsComplement(_in0);
  _b1 = Abc_ObjIsComplement(_in1);
  return (bb0 != _b0 && bb1 != _b1 && r0 == _r0 && r1 == _r1);
}

bool checkCase2_G6G7(Abc_Obj_t *G6, Abc_Obj_t *G7, Abc_Obj_t *G8) {
  if (isConstOrPi(G6) || isConstOrPi(G6))
    return false;
  Abc_Obj_t *in0, *in1;
  in0 = Abc_ObjChild0(G6);
  in1 = Abc_ObjChild1(G6);
  int r = 0;

  Abc_Obj_t *a, *b, *c;
  if (Abc_ObjIsComplement(in0)) {
    r += same2InputButInverse(Abc_ObjRegular(in0), G7);
    a = in1;
  }
  if (Abc_ObjIsComplement(in1)) {
    r += same2InputButInverse(Abc_ObjRegular(in1), G7);
    a = in0;
  }
  b = Abc_ObjChild0(G7);
  c = Abc_ObjChild1(G7);
  assert(r <= 1);

  if (r) {
    Abc_Obj_t *aR = Abc_ObjRegular(a);
    Abc_Obj_t *bR = Abc_ObjRegular(b);
    Abc_Obj_t *cR = Abc_ObjRegular(c);
  
    cout << Abc_ObjId(G8) << " = MAJ("
    << (Abc_ObjIsComplement(a) ? string("-") : string("")) << Abc_ObjId(aR) << ", "
    << (Abc_ObjIsComplement(b) ? string("-") : string("")) << Abc_ObjId(bR) << ", "
    << (Abc_ObjIsComplement(c) ? string("-") : string("")) << Abc_ObjId(cR) << ")\n";
    // 25 = MAJ(5, −14, 22)
  }
  return r;
}

bool checkCase2(Abc_Obj_t *G8) {
  assert(G8 ==  Abc_ObjRegular(G8));
  Abc_Obj_t *G7, *G6;
  G7 = Abc_ObjChild0(G8);
  G6 = Abc_ObjChild1(G8);
  
  if (!Abc_ObjIsComplement(G7) || !Abc_ObjIsComplement(G6))
  return false;
  
  G7 = Abc_ObjRegular(G7);
  G6 = Abc_ObjRegular(G6);
  
  if (isConstOrPi(G7) || isConstOrPi(G6))
    return false;

  int r = 0;
  if (Abc_ObjIsComplement(Abc_ObjChild0(G6)) || Abc_ObjIsComplement(Abc_ObjChild1(G6)))
    r += checkCase2_G6G7(G6, G7, G8);
  
  if (Abc_ObjIsComplement(Abc_ObjChild0(G7)) || Abc_ObjIsComplement(Abc_ObjChild1(G7)))
    r += checkCase2_G6G7(G7, G6, G8);
  
  assert(r <= 1);
  return r;
  
}
    
      

void
Lsv_NtkMajFind( Abc_Ntk_t * pNtk )
{
  int counter1 = 0;
  int counter2 = 0;
  Abc_Obj_t *pObj;
  int i;
  Abc_NtkForEachNode(pNtk, pObj, i) {
    counter1 += checkCase1(pObj);
    counter2 += checkCase2(pObj);
  }
  cout << "MAJ counter1: " << counter1 << endl;
  cout << "MAJ counter2: " << counter2 << endl;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

