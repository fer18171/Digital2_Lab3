#include "EUSART.h"

void EUSART_conf(void){
    TXSTAbits.TXEN=1;
    TXSTAbits.SYNC=0;
    RCSTAbits.SPEN=1;
    TXSTAbits.TX9=0;
    RCSTAbits.RC9=0;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = _XTAL_FREQ/(16*(9600+1));
    TXSTAbits.BRGH=1;
    SPBRGH = 0;
    RCSTAbits.CREN=1;
}
