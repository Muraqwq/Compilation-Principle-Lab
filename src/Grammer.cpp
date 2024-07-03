#include "Grammer.h"

Grammer::Grammer(TransMethod& method) : methods(method)
{
    grammer_list = {

        /** 0 S`-> S*/
        Production('V', {'S'}, [this](Symbol* s)
                   { methods.S2V((S*)s); }),
        /**1 S -> begin L;   end*/
        Production('S', {1, 'L', 20,2}, [this](Symbol* s)
                   { methods.BeginLend2S((S*)s); }),
        /**2 S -> integer I  整型变量定义*/
        Production('S', {4, 'I'}, [this](Symbol* s)
                   { methods.IntDefine((S*)s); }),
        /**3 S-> if E then M  { S1; } N else M  {S2;} */
        Production('S', {5, 'E', 6, 'M', 27, 'S',20, 28, 'N', 7, 'M', 27, 'S', 20,28}, [this](Symbol* s)
                   { methods.IfElse2S((S*)s); }),
        /**4 S -> if E then M { S1; }*/
        Production('S', {5, 'E', 6, 'M', 27, 'S', 20, 28}, [this](Symbol* s)
                   { methods.If2S((S*)s); }),
        /**5 S->While M E do M{ S }*/
        Production('S', {9, 'M', 'E', 8, 'M', 27, 'S',20,  28}, [this](Symbol* s)
                   { methods.Whiledo2S((S*)s); }),

        ///**6 S -> A  赋值语句*/
        //Production('S', {'A'}, [this](Symbol* s)
        //           { methods.A2S((S*)s); }),
        /**6 L -> L; M S*/
        Production('L', {'L', 20, 'M', 'S'}, [this](Symbol* s)
                   { methods.LMStoL((L*)s); }),
        /**7 L -> S;*/
        Production('L', {'S'}, [this](Symbol* s)
                   { methods.S2L((L*)s); }),
        /*8 S->id:= E*/
        Production('S', {'I', 23, 'E'}, [this](Symbol* s)
                   { methods.IequalE2S((S*)s); }),
        /**9 E->E+E*/
        /**E->E*E*/
        Production('E', {'E', 'C', 'E'}, [this](Symbol* s)
                   { methods.ECE2E((E*)s); }),

        // Production('E', {'E', 14, 'E'}, [this](Symbol *s)
        //            { methods.EtimesE2E((E *)s); }),
        /**10 E->I*/
        Production('E', {'I'}, [this](Symbol* s)
                   { methods.ID2E((E*)s); }),
        /**11 E->E or E*/
        /** E-> E and E*/
        Production('E', {'E', 'B', 'M', 'E'}, [this](Symbol* s)
                   { methods.EBME2E((E*)s); }),

        // Production('E', {'E', 24, 'M', 'E'}, [this](Symbol *s)
        //            { methods.EandME2E((E *)s); }),
        /**12 E -> not E*/
        Production('E', {26, 'E'}, [this](Symbol* s)
                   { methods.notE2E((E*)s); }),
        /**13 E-> I relop I*/
        Production('E', {'I', 'R', 'I'}, [this](Symbol* s)
                   { methods.IrelopI2E((E*)s); }),

        /**M -> ε*/
        Production('M', {10}, [this](Symbol* s)
                   { methods.Null2M((M*)s); }),

        /**N -> ε*/
        Production('N', {10}, [this](Symbol* s)
                   { methods.Null2N((N*)s); }),

    };
}
const Production&
Grammer::getProduction(int target)
{
    return grammer_list[target];
}
