#include "drivers.h"
#include "dac.h"
#include "pwm.h"
#include "adc.h"
void run(int stuff[], int *item, int row[],int target,int *end);
int main(void){
  
  int arr[40] = {     /// this for map
    1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 
    7, 7, 7, 7, 1, 1, 64, 54, 64, 64,
    5, 1, 1, 1, 1, 1, 1, 125, 1, 200,
    1, 40, 60, 1, 120, 1, 7, 2, 4, 7};
  int ms = 0, end = 1, item = 0,sum = 0,target=0;
  int lookUpTbl[16] = {15, 11, 0, 14, 10, 9, 8, 7, 10, 6, 5, 4, 10, 3, 2, 1}; // alla tangentbord i ordning
  int path[] = {1,2,4,8,16,32,64,128,256};    // 8x8 data 
  int row[8] = {0};    // will make the flow uppdate
  
  t5omsi();  // Initialize timer5 1kHz
  colinit(); // Initialize column toolbox
  l88init(); // Initialize 8*8 led toolbox
  keyinit(); // Initialize keyboard toolbox
  ADC3powerUpInit(1);
 
  while (1)
  {
    int potMeter = (read_adc()*7)/4059; // 0... 4059 I will make it 0 till 7
    ///read_adc()///  Den function kommer att omvandla analog till digital genom adc.c

    if (t5expq())
    {                   
      l88row(colset());
      
       if(end)
       {   
         ms++;
         target = path[potMeter];
         l88mem(7,path[potMeter]);    // the target                      
            if (ms==500){  // this will make the map flow
               run(arr,&item,row,target,&end);
              ms=0;  
            }        
       }
      else
      {

        // Sad face in end
         for (int i = 0; i < 8; i++)
      {
        if(i==0 || i == 7){ l88mem(i,255);}
        else if (i == 2)
        {
           l88mem(i,165);
        }
        else if (i == 4)
        {
          l88mem(i,153);
        }
         else if (i == 5)
        {
          l88mem(i,165);
        }
        else{l88mem(i,129);};
      }
      }
    }
  }
}
void run(int stuff[], int *item, int row[],int target,int *end) {
    if (*item == 40) {     /// make array looper på sig själv 
        *item = 0;     
    }
    for (int i = 7; i > 0; i--) {  // den kommer att uppdetera row array
        row[i] = row[i - 1];  
    }
    row[0] = stuff[*item];    // nästa värder för row 
    for (int i = 0; i < 8; i++) {   // uppdeterar map 
        l88mem(i, row[i]);
        if (target & row[7]) {*(end) =0;};     // Om spelare kollidera med hindrar sluta spelet 
    }
    (*item)++;     // uppdetera map att den kommer att loop igen
}