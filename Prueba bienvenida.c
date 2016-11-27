# include <xc.h>

//const unsigned char myROMarray[11] @ 0x100 = "Hello World"; // ROM data
//void main(void){
//unsigned char *table_ptr=0xAF1; 
//unsigned char x;
 //for(x=0;x<11;x++){
 //*table_ptr = myROMarray [x];
 //table_ptr++;
// }
//}

unsigned char myRAMarray[11] @ 0xA00; // RAM data.
const unsigned char myROMarray[11] @ 0x900 = "Hello World"; // ROM data
void main(void){
 unsigned char x;
 for(x=0;x<11;x++)
 myRAMarray [x] = myROMarray [x];
}