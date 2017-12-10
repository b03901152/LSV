/**CppFile**************************************************************
 
  FileName    [muxFind.cpp] 

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [mux finder]
  
  Synopsis    [Command file]

  Author      [Danny Wang]
   
  Affiliation [NTUEE]

  Date        [9, October, 2016]

  Revision    []

***********************************************************************/

#include "base/main/mainInt.h"
#include "base/abc/abc.h"
#include "mux.h"
#include <iostream>
#include <map>

using namespace std;

#ifndef _WIN32
#include <unistd.h>
#endif

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

void Mux_Init( Abc_Frame_t * pAbc );
void Mux_End( Abc_Frame_t * pAbc );

static int MuxCommandMuxFind( Abc_Frame_t * pAbc, int argc, char **argv );
void MuxFind( Abc_Ntk_t * pNtk );
void MuxFindRecursive( Abc_Obj_t * pObj, int & count, map < Abc_Obj_t *, bool > & );
bool ReportMUX( Abc_Obj_t * pObj );

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


void Mux_Init( Abc_Frame_t * pAbc )
{
    Cmd_CommandAdd( pAbc , "z Mux" , "MUX_find" , MuxCommandMuxFind , 0 );
}

void Mux_End( Abc_Frame_t * pAbc )
{
}

/**Function*************************************************************

  Synopsis    [mux command]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

int MuxCommandMuxFind( Abc_Frame_t * pAbc, int argc, char **argv )
{
    int c;
    int nArgcNew;
    char ** pArgvNew;
    Abc_Ntk_t * pNtk, * pNtkRes;
    Abc_Obj_t * pObj;
    pNtk = Abc_FrameReadNtk(pAbc);

    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "" ) ) != EOF )
    {
        switch ( c )
        {
        default:
            goto usage;
        }
    }
    
    pArgvNew = argv + globalUtilOptind;
    nArgcNew = argc - globalUtilOptind;
    // printf("%d \n", nArgcNew);
    // printf("%s \n", pArgvNew[nArgcNew - 1]);

    if( nArgcNew != 0 ) goto usage;

    cout << "start MUX_find" << endl;
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    Abc_Ntk_t * pNtkTemp;

    // get the new network
    pNtkRes = Abc_NtkStrash( pNtk, 0, 1, 0 );

    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "Strashing has failed.\n" );
        return 1;
    }

    cout << "Before MUX_find" << endl;
    MuxFind( pNtkRes );

    return 0;

usage:
    Abc_Print( -2, "usage: MUX_find \n" );
    return 1;
}

void MuxFind( Abc_Ntk_t * pNtk )
{
  Abc_Obj_t * pPo;
  int i, count = 0;
  map < Abc_Obj_t *, bool > traversed;

/*
cout << "--- Information ---" << endl;
  Abc_NtkForEachObj( pNtk, pPo, i ) {
    Abc_AigPrintNode( pPo );
  }
cout << "-------------------" << endl;
*/
  Abc_NtkForEachPo( pNtk, pPo, i ) {
    MuxFindRecursive( pPo, count, traversed );
  }
  // count = Abc_NtkCountMuxes( pNtk );
  cout << "Total number of MUXes is " << count << endl;
}

void MuxFindRecursive( Abc_Obj_t * pObj, int & count, 
  map < Abc_Obj_t *, bool > & map )
{
  pObj = Abc_ObjRegular( pObj );
  Abc_Obj_t * pCurrent, * pTemp, * pOne, * pTwo;
  if( !Abc_ObjIsPi(pObj) && !Abc_AigNodeIsConst(pObj) ) {
    pCurrent = Abc_ObjChild0( pObj );
// Abc_AigPrintNode( pCurrent );
// cout << "Child0" << endl;
    pCurrent = Abc_ObjRegular( pCurrent );
    if( map.find(pCurrent) == map.end() ) {
      //if( Abc_NodeIsMuxType( pCurrent ) ) ++count;
      if( ReportMUX( pCurrent ) ) ++count;
    }
    map[pCurrent] = true;
    MuxFindRecursive( pCurrent, count, map );
    if( !Abc_ObjIsPo(pObj) ) {
      pCurrent = Abc_ObjChild1( pObj );
// Abc_AigPrintNode( pCurrent );
// cout << "Child1" << endl;
      pCurrent = Abc_ObjRegular( pCurrent );
      if( map.find(pCurrent) == map.end() ) {
        // if( Abc_NodeIsMuxType( pCurrent ) ) ++count;
        if( ReportMUX( pCurrent ) ) ++count;
      }
      map[pCurrent] = true;
      MuxFindRecursive( pCurrent, count, map );
    }
  }
}

bool ReportMUX( Abc_Obj_t * pObj )
{
  pObj = Abc_ObjRegular( pObj );
// Abc_AigPrintNode( pObj );
  Abc_Obj_t * pOne, * pTwo;
  Abc_Obj_t * pC = NULL, * pA, * pB;
  if( Abc_ObjIsPi(pObj) || Abc_AigNodeIsConst(pObj) ) return false;
  
// cout << "Before IF" << endl;
  if( Abc_ObjIsComplement(Abc_ObjChild0( pObj )) && Abc_ObjIsComplement(Abc_ObjChild1( pObj )) ) {
// cout << "In IF" << endl;
        pOne = Abc_ObjFanin0( pObj );
        pTwo = Abc_ObjFanin1( pObj );
// Abc_AigPrintNode( pOne );
// cout << "One" << endl;
// Abc_AigPrintNode( pTwo );
// cout << "Two" << endl;
        if( Abc_ObjIsPi(pOne) || Abc_ObjIsPi(pTwo) || Abc_AigNodeIsConst(pOne) || Abc_AigNodeIsConst(pTwo) ) return false;

        if( Abc_ObjFanin0(pOne) == Abc_ObjFanin0(pTwo) ) {
          if( Abc_ObjIsComplement(Abc_ObjChild0(pOne)) && (!Abc_ObjIsComplement(Abc_ObjChild0(pTwo))) ) {
            pC = Abc_ObjFanin0(pOne);
            pA = Abc_ObjNot(Abc_ObjChild1(pOne));
            pB = Abc_ObjNot(Abc_ObjChild1(pTwo));
          }
          else if( (!Abc_ObjIsComplement(Abc_ObjChild0(pOne))) && Abc_ObjIsComplement(Abc_ObjChild0(pTwo)) ) {
            pC = Abc_ObjFanin0(pTwo);
            pA = Abc_ObjNot(Abc_ObjChild1(pTwo));
            pB = Abc_ObjNot(Abc_ObjChild1(pOne));
          }
        }
        else if( Abc_ObjFanin1(pOne) == Abc_ObjFanin0(pTwo)) {
          if( Abc_ObjIsComplement(Abc_ObjChild1(pOne)) && (!Abc_ObjIsComplement(Abc_ObjChild0(pTwo))) ) {
            pC = Abc_ObjFanin1(pOne);
            pA = Abc_ObjNot(Abc_ObjChild0(pOne));
            pB = Abc_ObjNot(Abc_ObjChild1(pTwo));
          }
          else if( (!Abc_ObjIsComplement(Abc_ObjChild1(pOne))) && Abc_ObjIsComplement(Abc_ObjChild0(pTwo)) ) {
            pC = Abc_ObjFanin0(pTwo);
            pA = Abc_ObjNot(Abc_ObjChild1(pTwo));
            pB = Abc_ObjNot(Abc_ObjChild0(pOne));
          }
        }
        else if( Abc_ObjFanin0(pOne) == Abc_ObjFanin1(pTwo)) {
          if( Abc_ObjIsComplement(Abc_ObjChild0(pOne)) && (!Abc_ObjIsComplement(Abc_ObjChild1(pTwo))) ) {
            pC = Abc_ObjFanin0(pOne);
            pA = Abc_ObjNot(Abc_ObjChild1(pOne));
            pB = Abc_ObjNot(Abc_ObjChild0(pTwo));
          }
          else if( (!Abc_ObjIsComplement(Abc_ObjChild0(pOne))) && Abc_ObjIsComplement(Abc_ObjChild1(pTwo))) {
            pC = Abc_ObjFanin1(pTwo);
            pA = Abc_ObjNot(Abc_ObjChild0(pTwo));
            pB = Abc_ObjNot(Abc_ObjChild1(pOne));
          }
        }
        else if( Abc_ObjFanin1(pOne) == Abc_ObjFanin1(pTwo)) {
          if( Abc_ObjIsComplement(Abc_ObjChild1(pOne)) && (!Abc_ObjIsComplement(Abc_ObjChild1(pTwo))) ) {
            pC = Abc_ObjFanin1(pOne);
            pA = Abc_ObjNot(Abc_ObjChild0(pOne));
            pB = Abc_ObjNot(Abc_ObjChild0(pTwo));
          }
          else if( (!Abc_ObjIsComplement(Abc_ObjChild1(pOne))) && Abc_ObjIsComplement(Abc_ObjChild1(pTwo)) ) {
            pC = Abc_ObjFanin1(pTwo);
            pA = Abc_ObjNot(Abc_ObjChild0(pTwo));
            pB = Abc_ObjNot(Abc_ObjChild0(pOne));
          }
        }
  }
  if( pC != NULL ) {
    bool a = false, b = false;
    a = Abc_ObjIsComplement(pA);
    b = Abc_ObjIsComplement(pB);
    pA = Abc_ObjRegular( pA );
    pB = Abc_ObjRegular( pB );
    pC = Abc_ObjRegular( pC );
    if( pA == pB ) return false;
    cout << Abc_ObjId(pObj) << " = MUX ( " << Abc_ObjId(pC) << ", ";
    if(a) cout << "-";
    cout << Abc_ObjId(pA)   << ", ";
    if(b) cout << "-";
    cout << Abc_ObjId(pB) << " )" << endl;
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

ABC_NAMESPACE_IMPL_END
