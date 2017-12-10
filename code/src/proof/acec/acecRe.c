/**CFile****************************************************************

  FileName    [acecRe.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [CEC for arithmetic circuits.]

  Synopsis    [Core procedures.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: acecRe.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "acecInt.h"
#include "misc/vec/vecHash.h"
#include "misc/util/utilTruth.h"

ABC_NAMESPACE_IMPL_START


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

#define Ree_ForEachCut( pList, pCut, i ) for ( i = 0, pCut = pList + 1; i < pList[0]; i++, pCut += pCut[0] + 2 )

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Detecting FADDs in the AIG.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Ree_TruthPrecompute()
{
    word Truths[8] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };
    word Truth;
    int i;
    for ( i = 0; i < 8; i++ )
    {
        Truth = Truths[i];
        Truth = Abc_Tt6SwapAdjacent( Truth, 1 );
        Abc_TtPrintHexRev( stdout, &Truth, 3 );
        printf( "\n" );
    }
    printf( "\n" );
    for ( i = 0; i < 8; i++ )
    {
        Truth = Truths[i];
        Truth = Abc_Tt6SwapAdjacent( Truth, 1 );
        Truth = Abc_Tt6SwapAdjacent( Truth, 0 );
        Abc_TtPrintHexRev( stdout, &Truth, 3 );
        printf( "\n" );
    }
    printf( "\n" );
}

/**Function*************************************************************

  Synopsis    [Detecting FADDs in the AIG.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
static inline int Ree_ManCutMergeOne( int * pCut0, int * pCut1, int * pCut )
{
    int i, k;
    for ( k = 0; k <= pCut1[0]; k++ )
        pCut[k] = pCut1[k];
    for ( i = 1; i <= pCut0[0]; i++ )
    {
        for ( k = 1; k <= pCut1[0]; k++ )
            if ( pCut0[i] == pCut1[k] )
                break;
        if ( k <= pCut1[0] )
            continue;
        if ( pCut[0] == 3 )
            return 0;
        pCut[1+pCut[0]++] = pCut0[i];
    }
    assert( pCut[0] == 2 || pCut[0] == 3 );
    if ( pCut[1] > pCut[2] )
        ABC_SWAP( int, pCut[1], pCut[2] );
    assert( pCut[1] < pCut[2] );
    if ( pCut[0] == 2 )
        return 1;
    if ( pCut[2] > pCut[3] )
        ABC_SWAP( int, pCut[2], pCut[3] );
    if ( pCut[1] > pCut[2] )
        ABC_SWAP( int, pCut[1], pCut[2] );
    assert( pCut[1] < pCut[2] );
    assert( pCut[2] < pCut[3] );
    return 1;
}
static inline int Ree_ManCutCheckEqual( Vec_Int_t * vCuts, int * pCutNew )
{
    int * pList = Vec_IntArray( vCuts );
    int i, k, * pCut;
    Ree_ForEachCut( pList, pCut, i )
    {
        for ( k = 0; k <= pCut[0]; k++ )
            if ( pCut[k] != pCutNew[k] )
                break;
        if ( k > pCut[0] )
            return 1;
    }
    return 0;
}
static inline int Ree_ManCutFind( int iObj, int * pCut )
{
    if ( pCut[1] == iObj ) return 0;
    if ( pCut[2] == iObj ) return 1;
    if ( pCut[3] == iObj ) return 2;
    assert( 0 );
    return -1;
}
static inline int Ree_ManCutNotFind( int iObj1, int iObj2, int * pCut )
{
    if ( pCut[3] != iObj1 && pCut[3] != iObj2 ) return 0;
    if ( pCut[2] != iObj1 && pCut[2] != iObj2 ) return 1;
    if ( pCut[1] != iObj1 && pCut[1] != iObj2 ) return 2;
    assert( 0 );
    return -1;
}
static inline int Ree_ManCutTruthOne( int * pCut0, int * pCut )
{
    int Truth0 = pCut0[pCut0[0]+1];
    int fComp0 = (Truth0 >> 7) & 1;
    if ( pCut0[0] == 3 )
        return Truth0;
    Truth0 = fComp0 ? ~Truth0 : Truth0;
    if ( pCut0[0] == 2 )
    {
        int Truths[3][8] = {
            { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 }, // {0,1,-}
            { 0x00, 0x05, 0x0A, 0x0F, 0x50, 0x55, 0x5A, 0x5F }, // {0,-,1}
            { 0x00, 0x03, 0x0C, 0x0F, 0x30, 0x33, 0x3C, 0x3F }  // {-,0,1}
        };
        int Truth = Truths[Ree_ManCutNotFind(pCut0[1], pCut0[2], pCut)][Truth0 & 0x7];
        return 0xFF & (fComp0 ? ~Truth : Truth);
    }
    if ( pCut0[0] == 1 )
    {
        int Truths[3] = { 0x55, 0x33, 0x0F };
        int Truth = Truths[Ree_ManCutFind(pCut0[1], pCut)];
        return 0xFF & (fComp0 ? ~Truth : Truth);
    }
    assert( 0 );
    return -1;
}
static inline int Ree_ManCutTruth( Gia_Obj_t * pObj, int * pCut0, int * pCut1, int * pCut )
{
    int Truth0 = Ree_ManCutTruthOne( pCut0, pCut );
    int Truth1 = Ree_ManCutTruthOne( pCut1, pCut );
    Truth0 = Gia_ObjFaninC0(pObj) ? ~Truth0 : Truth0;
    Truth1 = Gia_ObjFaninC1(pObj) ? ~Truth1 : Truth1;
    return 0xFF & (Gia_ObjIsXor(pObj) ? Truth0 ^ Truth1 : Truth0 & Truth1); 
}

#if 0

int Ree_ObjComputeTruth_rec( Gia_Obj_t * pObj )
{
    int Truth0, Truth1;
    if ( pObj->Value )
        return pObj->Value;
    assert( Gia_ObjIsAnd(pObj) );
    Truth0 = Ree_ObjComputeTruth_rec( Gia_ObjFanin0(pObj) );
    Truth1 = Ree_ObjComputeTruth_rec( Gia_ObjFanin1(pObj) );
    if ( Gia_ObjIsXor(pObj) )
        return (pObj->Value = (Gia_ObjFaninC0(pObj) ? ~Truth0 : Truth0) ^ (Gia_ObjFaninC1(pObj) ? ~Truth1 : Truth1));
    else
        return (pObj->Value = (Gia_ObjFaninC0(pObj) ? ~Truth0 : Truth0) & (Gia_ObjFaninC1(pObj) ? ~Truth1 : Truth1));
}
void Ree_ObjCleanTruth_rec( Gia_Obj_t * pObj )
{
    if ( !pObj->Value )
        return;
    pObj->Value = 0;
    if ( !Gia_ObjIsAnd(pObj) )
        return;
    Ree_ObjCleanTruth_rec( Gia_ObjFanin0(pObj) );
    Ree_ObjCleanTruth_rec( Gia_ObjFanin1(pObj) );
}
int Ree_ObjComputeTruth( Gia_Man_t * p, int iObj, int * pCut )
{
    unsigned Truth, Truths[3] = { 0xAA, 0xCC, 0xF0 }; int i;
    for ( i = 1; i <= pCut[0]; i++ )
        Gia_ManObj(p, pCut[i])->Value = Truths[i-1];
    Truth = 0xFF & Ree_ObjComputeTruth_rec( Gia_ManObj(p, iObj) );
    Ree_ObjCleanTruth_rec( Gia_ManObj(p, iObj) );
    return Truth;
}

#endif

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Ree_ManCutPrint( int * pCut, int Count, word Truth )
{
    int c;
    printf( "%d : ", Count );
    for ( c = 1; c <= pCut[0]; c++ )
        printf( "%3d ", pCut[c] );
    for (      ; c <= 4; c++ )
        printf( "    " );
    printf( "0x" );
    Abc_TtPrintHexRev( stdout, &Truth, 3 );
    printf( "\n" );
}
void Ree_ManCutMerge( Gia_Man_t * p, int iObj, int * pList0, int * pList1, Vec_Int_t * vCuts, Hash_IntMan_t * pHash, Vec_Int_t * vData )
{
    int fVerbose = 0;
    int i, k, c, Value, Truth, TruthC, * pCut0, * pCut1, pCut[5], Count = 0;
    if ( fVerbose )
        printf( "Object %d\n", iObj );
    Vec_IntFill( vCuts, 2, 1 );
    Vec_IntPush( vCuts, iObj );
    Vec_IntPush( vCuts, 0xAA );
    Ree_ForEachCut( pList0, pCut0, i )
    Ree_ForEachCut( pList1, pCut1, k )
    {
        if ( !Ree_ManCutMergeOne(pCut0, pCut1, pCut) )
            continue;
        if ( Ree_ManCutCheckEqual(vCuts, pCut) )
            continue;
        Truth = TruthC = Ree_ManCutTruth(Gia_ManObj(p, iObj), pCut0, pCut1, pCut);
        //assert( Truth == Ree_ObjComputeTruth(p, iObj, pCut) );
        Vec_IntAddToEntry( vCuts, 0, 1 );  
        for ( c = 0; c <= pCut[0]; c++ )
            Vec_IntPush( vCuts, pCut[c] );
        Vec_IntPush( vCuts, Truth );
        if ( Truth & 0x80 )
            Truth = 0xFF & ~Truth;
        if ( (Truth == 0x66 || Truth == 0x11 || Truth == 0x22 || Truth == 0x44 || Truth == 0x77) && pCut[0] == 2 )
        {
            assert( pCut[0] == 2 );
            Value = Hsh_Int3ManInsert( pHash, pCut[1], pCut[2], 0 );
            Vec_IntPushThree( vData, iObj, Value, TruthC );
        }
        else if ( Truth == 0x69 || Truth == 0x17 || Truth == 0x2B || Truth == 0x4D || Truth == 0x71 )
        {
            assert( pCut[0] == 3 );
            Value = Hsh_Int3ManInsert( pHash, pCut[1], pCut[2], pCut[3] );
            Vec_IntPushThree( vData, iObj, Value, TruthC );
        }
        if ( fVerbose )
            Ree_ManCutPrint( pCut, ++Count, TruthC );
    }
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Vec_Int_t * Ree_ManDeriveAdds( Hash_IntMan_t * p, Vec_Int_t * vData )
{
    int i, j, k, iObj, iObj2, Value, Truth, CountX, CountM, Index = 0;
    int nEntries = Hash_IntManEntryNum(p);
    Vec_Int_t * vAdds = Vec_IntAlloc( 1000 );
    Vec_Int_t * vXors = Vec_IntStart( nEntries + 1 );
    Vec_Int_t * vMajs = Vec_IntStart( nEntries + 1 );
    Vec_Int_t * vIndex = Vec_IntStartFull( nEntries + 1 );
    Vec_Int_t * vIndexRev = Vec_IntAlloc( 1000 );
    Vec_Wec_t * vXorMap, * vMajMap;
    Vec_IntForEachEntryTriple( vData, iObj, Value, Truth, i )
    {
        assert( Value <= nEntries );
        if ( Truth == 0x66 || Truth == 0x99 || Truth == 0x69 || Truth == 0x96 )
            Vec_IntAddToEntry( vXors, Value, 1 );
        else
            Vec_IntAddToEntry( vMajs, Value, 1 );
    }
    // remap these into indexes
    Vec_IntForEachEntryTwo( vXors, vMajs, CountX, CountM, i )
        if ( CountX && CountM )
        {
            Vec_IntPush( vIndexRev, i );
            Vec_IntWriteEntry( vIndex, i, Index++ );
        }
    Vec_IntFree( vXors );
    Vec_IntFree( vMajs );
    printf( "Detected %d shared cuts among %d hashed cuts.\n", Index, nEntries );
    // collect nodes
    vXorMap = Vec_WecStart( Index );
    vMajMap = Vec_WecStart( Index );
    Vec_IntForEachEntryTriple( vData, iObj, Value, Truth, i )
    {
        Index = Vec_IntEntry( vIndex, Value );
        if ( Index == -1 )
            continue;
        if ( Truth == 0x66 || Truth == 0x99 || Truth == 0x69 || Truth == 0x96 )
            Vec_WecPush( vXorMap, Index, iObj );
        else
            Vec_WecPush( vMajMap, Index, iObj );
    }
    Vec_IntFree( vIndex );
    // create pairs
    Vec_IntForEachEntry( vIndexRev, Value, i )
    {
        Vec_Int_t * vXorOne = Vec_WecEntry( vXorMap, i );
        Vec_Int_t * vMajOne = Vec_WecEntry( vMajMap, i );
        Hash_IntObj_t * pObj = Hash_IntObj( p, Value );
        Vec_IntForEachEntry( vXorOne, iObj, j )
        Vec_IntForEachEntry( vMajOne, iObj2, k )
        {
            Vec_IntPushThree( vAdds, pObj->iData0, pObj->iData1, pObj->iData2 ); 
            Vec_IntPushTwo( vAdds, iObj, iObj2 );
        }
    }
    Vec_IntFree( vIndexRev );
    Vec_WecFree( vXorMap );
    Vec_WecFree( vMajMap );
    return vAdds;
}
Vec_Int_t * Ree_ManComputeCuts( Gia_Man_t * p, int fVerbose )
{
    Gia_Obj_t * pObj; 
    int * pList0, * pList1, i, nCuts = 0;
    Hash_IntMan_t * pHash = Hash_IntManStart( 1000 );
    Vec_Int_t * vAdds;
    Vec_Int_t * vTemp = Vec_IntAlloc( 1000 );
    Vec_Int_t * vData = Vec_IntAlloc( 1000 );
    Vec_Int_t * vCuts = Vec_IntAlloc( 30 * Gia_ManAndNum(p) );
    Vec_IntFill( vCuts, Gia_ManObjNum(p), 0 );
    Gia_ManCleanValue( p );
    Gia_ManForEachCi( p, pObj, i )
    {
        Vec_IntWriteEntry( vCuts, Gia_ObjId(p, pObj), Vec_IntSize(vCuts) );
        Vec_IntPush( vCuts, 1 );
        Vec_IntPush( vCuts, 1 );
        Vec_IntPush( vCuts, Gia_ObjId(p, pObj) );
        Vec_IntPush( vCuts, 0xAA );
    }
    Gia_ManForEachAnd( p, pObj, i )
    {
        pList0 = Vec_IntEntryP( vCuts, Vec_IntEntry(vCuts, Gia_ObjFaninId0(pObj, i)) );
        pList1 = Vec_IntEntryP( vCuts, Vec_IntEntry(vCuts, Gia_ObjFaninId1(pObj, i)) );
        Ree_ManCutMerge( p, i, pList0, pList1, vTemp, pHash, vData );
        Vec_IntWriteEntry( vCuts, i, Vec_IntSize(vCuts) );
        Vec_IntAppend( vCuts, vTemp );
        nCuts += Vec_IntEntry( vTemp, 0 );
    }
    if ( fVerbose )
        printf( "Nodes = %d.  Cuts = %d.  Cuts/Node = %.2f.  Ints/Node = %.2f.\n", 
            Gia_ManAndNum(p), nCuts, 1.0*nCuts/Gia_ManAndNum(p), 1.0*Vec_IntSize(vCuts)/Gia_ManAndNum(p) );
    Vec_IntFree( vTemp );
    Vec_IntFree( vCuts );
    vAdds = Ree_ManDeriveAdds( pHash, vData );
    if ( fVerbose )
        printf( "Adds = %d.  Total = %d.  Hashed = %d.  Hashed/Adds = %.2f.\n", 
            Vec_IntSize(vAdds)/5, Vec_IntSize(vData)/3, Hash_IntManEntryNum(pHash), 5.0*Hash_IntManEntryNum(pHash)/Vec_IntSize(vAdds) );
    Vec_IntFree( vData );
    Hash_IntManStop( pHash );
    return vAdds;
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Ree_ManComputeCutsTest( Gia_Man_t * p )
{
    abctime clk = Abc_Clock();
    Vec_Int_t * vAdds = Ree_ManComputeCuts( p, 1 );
    int i, Count = 0;
    for ( i = 0; 5*i < Vec_IntSize(vAdds); i++ )
    {
        if ( Vec_IntEntry(vAdds, 5*i+2) == 0 )
            continue;
        Count++;
        continue;
        printf( "%6d : ", i );
        printf( "%6d ", Vec_IntEntry(vAdds, 5*i+0) );
        printf( "%6d ", Vec_IntEntry(vAdds, 5*i+1) );
        printf( "%6d ", Vec_IntEntry(vAdds, 5*i+2) );
        printf( "   ->  " );
        printf( "%6d ", Vec_IntEntry(vAdds, 5*i+3) );
        printf( "%6d ", Vec_IntEntry(vAdds, 5*i+4) );
        printf( "\n" );
    }
    Vec_IntFree( vAdds );
    printf( "Detected %d FAs and %d HAs.  ", Count, Vec_IntSize(vAdds)/5-Count );
    Abc_PrintTime( 1, "Time", Abc_Clock() - clk );
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

