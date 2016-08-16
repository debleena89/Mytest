#include <stdio.h>

#define false 0
#define true 1

/********** Global flags and buffers ******/
unsigned char nondet_unsignedchar();
unsigned char survstatbuf[16];
unsigned char survstatbuf2[16];
unsigned char zero_array[16];
int nondet_int();
__CPROVER_bool MainDC_DC;

/****** End of global declarations ******/

void bzero(void *s1, unsigned n)
{
    register char *t = (char *) s1;

    while (n != 0) {
        *t++ = 0;
         n--;
    }
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    unsigned char u1, u2;

    for (; n-- ; s1++, s2++) {
        u1 = * (unsigned char *) s1;
        u2 = * (unsigned char *) s2;
        if ( u1 != u2) {
            return (u1-u2);
        }
    }
    return 0;
}

void check_DC_DC(int dip)
{
     bzero(zero_array, 16); 

     if(dip < 96)
     { 
         if (memcmp (zero_array, survstatbuf, 12) == 0)
            MainDC_DC = false; 
         else 
            MainDC_DC = true; 
     } 
}

int main()
{
    __CPROVER_bool x;
    unsigned char  chno = nondet_unsignedchar();

    for (int i = 0; i < 15; i++)
    {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
    }

    check_DC_DC(chno);

    x = (chno < 96) && (memcmp(zero_array, survstatbuf, 12) == 0);  
                                     
   /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(chno >= 0 && chno <= 127);

    __CPROVER_assert (!x || (MainDC_DC == 0), "Fails");
}
