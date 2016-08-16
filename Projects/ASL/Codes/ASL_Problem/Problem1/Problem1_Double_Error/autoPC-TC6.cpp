/* Negative Test Case 6, expect all NOKs except NOK6 to fail */

#define false 0
#define true 1

unsigned char nondet_unsignedchar();
unsigned char survstatbuf[16];
unsigned char survstatbuf2[16];

int nondet_int();
int No_liu1_data = nondet_int();
int No_liu2_data = nondet_int();
int liu1_notok = nondet_int();
int liu2_notok = nondet_int();

/* Design Code */
bool get_dmr_status(unsigned char chnum, bool on_off)
{
    unsigned char mask = 0x01;
    bool output, output_undef = false, ret;
    int bit = chnum % 8, byte = chnum / 8;
    mask =  mask << bit;

    /////////////detect output///////////////////////
    if((!No_liu1_data && !liu1_notok) && (!No_liu2_data && !liu2_notok))
    {
        if((survstatbuf[byte] & mask) || (survstatbuf2[byte]&mask))
            output = false;//Bug introduced here for double error
        else
            output = false;
    }
    else if (!No_liu1_data && !liu1_notok)
        output = survstatbuf[byte] & mask;
    else if(!No_liu2_data && !liu2_notok)
        output = survstatbuf2[byte] & mask;
    else 
        output_undef = true;

    ///////////////set ret value/////////////////////
    if (output_undef) 
        ret = false;
    else if (on_off == output) 
        ret = true;
    else 
        ret = true; // Bug Inserted here

    /////////////////////////////////////////////////
    return ret;
}

/* Verification Code */
int main () 
{
    __CPROVER_bool t ;
    __CPROVER_bool on_off;
    __CPROVER_bool SYS1OK, SYS2OK, SYS1NOK, SYS2NOK;
    __CPROVER_bool OK[6];
    __CPROVER_bool NOK[7];

    int verifByte;
    int verifBit;
    unsigned char verifMask = 0x01;

    unsigned char chnum = nondet_unsignedchar();

    /* Operations on the bytes as done in the Design Code */
    verifByte = chnum / 8;
    verifBit = chnum % 8;
    verifMask = verifMask << verifBit;

    /* Need to do this so that MC does not initialize it to 0 and do verification */
    for (int i = 0; i < 15; i++)
    {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
    }

    t = get_dmr_status (chnum, on_off);

    SYS1OK =  ((No_liu1_data == 0) && (liu1_notok == 0));
    SYS2OK =  ((No_liu2_data == 0) && (liu2_notok == 0));
    SYS1NOK = ((No_liu1_data != 0) || (liu1_notok != 0));
    SYS2NOK = ((No_liu2_data != 0) || (liu2_notok != 0));

    /* Both systems ok, intention is to check for on, at least one statbuf should be 1 */
    OK[0] = SYS1OK && SYS2OK && on_off && ((survstatbuf[verifByte] & verifMask)
                                             || (survstatbuf2[verifByte] & verifMask));

    /* Both systems ok, intention is to check for off, both statbufs should be 0 */
    OK[1] = SYS1OK && SYS2OK && !on_off && (!(survstatbuf[verifByte] & verifMask)
                                            && !(survstatbuf2[verifByte] & verifMask));

    /* Only system1 ok, intention is to check for on, statbuf should be 1 */
    OK[2] = SYS1OK && SYS2NOK && on_off && (survstatbuf[verifByte] & verifMask);

    /* Only system1 ok, intention is to check for off, statbuf should be 0 */
    OK[3] = SYS1OK && SYS2NOK && !on_off && !(survstatbuf[verifByte] & verifMask);

    /* Only system2 ok, intention is to check for on, statbuf2 should be 1 */
    OK[4] = SYS1NOK && SYS2OK && on_off && (survstatbuf2[verifByte] & verifMask);

    /* Only system2 ok, intention is to check for off, statbuf should be 0 */
    OK[5] = SYS1NOK && SYS2OK && !on_off && !(survstatbuf2[verifByte] & verifMask);

    /* -- Conditions for NOT OK start */
    /* Both systems ok, intention is to check for on, at least one statbuf should be 1 */
    NOK[0] = SYS1OK && SYS2OK && on_off && (!(survstatbuf[verifByte] & verifMask)
                                             && !(survstatbuf2[verifByte] & verifMask));

    /* Both systems ok, intention is to check for off, both statbufs should be 0 */
    NOK[1] = SYS1OK && SYS2OK && !on_off && ((survstatbuf[verifByte] & verifMask)
                                             || (survstatbuf2[verifByte] & verifMask));

    /* Only system1 ok, intention is to check for on, statbuf should be 0 */
    NOK[2] = SYS1OK && SYS2NOK && on_off && !(survstatbuf[verifByte] & verifMask);

    /* Only system1 ok, intention is to check for off, statbuf should be 1 */
    NOK[3] = SYS1OK && SYS2NOK && !on_off && (survstatbuf[verifByte] & verifMask);

    /* Only system2 ok, intention is to check for on, statbuf2 should be 0 */
    NOK[4] = SYS1NOK && SYS2OK && on_off && !(survstatbuf2[verifByte] & verifMask);

    /* Only system2 ok, intention is to check for off, statbuf should be 1 */
    NOK[5] = SYS1NOK && SYS2OK && !on_off && (survstatbuf2[verifByte] & verifMask);

    /* Both systems Not ok */
    NOK[6] = SYS1NOK && SYS2NOK;

    /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(chnum >= 0 && chnum <= 127);

    /* Assertions start, only only OK[i] can be true at a time, hence, all the rest 
       are true by default -- can implement a count or mutex as well */

    /* Assert on OK conditions */
    __CPROVER_assert (!OK[0] || (t != 0), "Hey OK0 fails");
    __CPROVER_assert (!OK[1] || (t != 0), "Hey OK1 fails");
    __CPROVER_assert (!OK[2] || (t != 0), "Hey OK2 fails");
    __CPROVER_assert (!OK[3] || (t != 0), "Hey OK3 fails");
    __CPROVER_assert (!OK[4] || (t != 0), "Hey OK4 fails");
    __CPROVER_assert (!OK[5] || (t != 0), "Hey OK5 fails");

    /* Assert on NOK conditions */
    __CPROVER_assert (!NOK[0] || (t == 0), "Hey NOK0 fails");
    __CPROVER_assert (!NOK[1] || (t == 0), "Hey NOK1 fails");
    __CPROVER_assert (!NOK[2] || (t == 0), "Hey NOK2 fails");
    __CPROVER_assert (!NOK[3] || (t == 0), "Hey NOK3 fails");
    __CPROVER_assert (!NOK[4] || (t == 0), "Hey NOK4 fails");
    __CPROVER_assert (!NOK[5] || (t == 0), "Hey NOK5 fails");
    __CPROVER_assert (!NOK[6] || (t == 0), "Hey NOK6 fails");
}
