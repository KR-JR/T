#include <stdio.h>

typedef struct list {
    char data;
    struct list *link;
}list;


void printList(list *ptr) {
    while(1) {
        
        printf("%c\n", ptr->data);
        
        if(ptr->link == NULL)
            break;

        ptr = ptr->link;
    }
    printf("Address Contents\n");
}




main () {
    list item1, item2, item3;
    
    item1.data = 'a';
    item2.data = 'b';
    item3.data = 'c';

    item1.link = &item2;
    item2.link = &item3;

    list *listPtr = &item1;
    printList(listPtr);

}