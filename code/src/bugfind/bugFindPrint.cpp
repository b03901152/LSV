/**CppFile**************************************************************
 
  FileName    [bugFindPrint.cpp] 

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName []
  
  Synopsis    [Print Ntk Stats]

  Author      [Danny Wang]
   
  Affiliation [NTUEE]

  Date        []

  Revision    []

***********************************************************************/

#include "bugFind.h"

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

void BugFind_PrintNtkStats       ( Abc_Ntk_t * pNtk );
void BugFind_PrintIO             ( Abc_Ntk_t * pNtk );
void BugFind_PrintObj            ( Abc_Ntk_t * pNtk );
void BugFind_Print2Circuit       ( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2 );

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Bmatch_Resync function]

  Description [resync the input ntk]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void BugFind_PrintNtkStats( Abc_Ntk_t * pNtk )
{
    printf("Network name :    %s \n", Abc_NtkName(pNtk));
    printf("number of PIs   = %d \n", Abc_NtkPiNum( pNtk ));
    printf("number of POs   = %d \n", Abc_NtkPoNum( pNtk ));
    printf("number of Nodes = %d \n", Abc_NtkNodeNum( pNtk ));
}

void BugFind_PrintObj( Abc_Ntk_t * pNtk )
{
    Abc_Obj_t * pObj;
    int i;

    Abc_NtkForEachObj( pNtk, pObj, i){
        printf("Name %15s : ", Abc_ObjName( pObj ));
        Abc_ObjPrint( stdout, pObj );
    }
}

void BugFind_PrintIO( Abc_Ntk_t * pNtk )
{
    int i;
    Abc_Obj_t * pObj;

    Abc_NtkForEachPi( pNtk, pObj, i){
        printf("PI %15s : ", Abc_ObjName( pObj) );
        Abc_ObjPrint( stdout, pObj );
    }

    Abc_NtkForEachPo( pNtk, pObj, i){
        printf("PO %15s : ", Abc_ObjName( pObj) );
        Abc_ObjPrint( stdout, pObj );
    }
    printf("Total           PI : %7d   PO : %7d \n", Abc_NtkPiNum( pNtk ), Abc_NtkPoNum( pNtk ) );
}

void BugFind_Print2Circuit       ( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2 )
{
    printf("Total   Cir1    PI : %7d   PO : %7d \n", Abc_NtkPiNum( pNtk1 ), Abc_NtkPoNum( pNtk1 ) );
    printf("Total   Cir2    PI : %7d   PO : %7d \n", Abc_NtkPiNum( pNtk2 ), Abc_NtkPoNum( pNtk2 ) );
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

ABC_NAMESPACE_IMPL_END
