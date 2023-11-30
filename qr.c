#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "qrcodegen.h"
#include <ctype.h>

void main(){
    int i = verifyByQrWithText("Your Code Is:", "heh");
    printf("\n\n%d", i);
}