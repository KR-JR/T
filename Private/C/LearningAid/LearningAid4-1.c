




void insert_front(list_ptr *pptr, list_ptr node) {
    if (IS_EMPTY(*pptr)) {
        *pptr = node;
        node -> link = NULL;
    }
    else {
        node ->link = *pptr;
        *pptr = node;
    }
}


void insert_rear(list_ptr *pptr, list_ptr node, list_ptr rear) {
    if (IS_EMPTY(*pptr)) {
        *pptr = node = rear;
    }
    else {
        node = (list_ptr)malloc(sizeof(list_ptr));
        node -> data = rear -> data
        node -> link = NULL;
        rear -> link = node;
        rear = rear ->link;
    }
}