#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 30

typedef struct {
    char data[MAX_SIZE]; 
    int front, back;     
} Queue;

void initQueue(Queue *q){
    q->front = -1;
    q->back = -1;
}

int isEmpty(Queue *q){
 return(q->front ==-1);
}


void addtoQueue(char c, Queue *q){
  if (isEmpty(q)){
    q->front = q->back =0;

  } else {
    q->back = (q->back + 1) % MAX_SIZE;
  }
q-data[q->back] =c;
}


int QueueToString(Queue *q){
 int size =(q->back - q->front + MAX_SIZE) % MAX_SIZE +1;
 char *string =(char*)malloc(size);
 int i = q->front
 int index =0;

 while(1){
  string[index++] = q->data[i];
  if (i == q->back) break;
  i =(i + 1)
 }
return string;
}




initQueue;
addtoQueue(decodedChar ,q);


void lcd_scroll_queue(Queue *q)
{
  char *text QueueToString(q);
  int length = strlen(text);
  for(int i =0;, i<length; i++){

    lcd_clear();
    lcd_set_cursor(0,0);

    for(int j=0; j<16 && (i+j)<length;j++){
      lcd_put_char(text[i+j]);
    
    }
    delay_ms(5);
  }

}