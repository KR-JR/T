void path(void) {
/* 미로를 통과하는 경로가 있으면 그 경로를 출력한다. */
        int i, row, col, next_row, next_col, dir, found = FALSE;
        element position;
        mark[1][1] = 1; top = 0;
        stack[0].row = 1;stack[0].col=1;stack[0].dir = 1;
        
        while(top > -1 && !found)
        {
                position = delete(&top);
                row = position.row; col=position.col;
                dir = position.dir;
                while(dir < 8 && !found)
                {
                        /*dir 방향으로 이동*/
                        next_row = row+move[dir].vert;
                        next_col = col +move[dir].horiz;
                        if(next_row==EXIT_ROW && next_col==EXIT_COL)
                                found = TRUE;
                        else if(!maze[next_row][next_col] &&
                                !mark[next_row][next_col])
                        {
                                mark[next_row][next_col] = 1;
                                position.row = row; position.col = col;
                                position.dir = ++dir;
                                add(&top,position);
                                row = next.row; col = next; dir = 0;
                        }
                        else ++dir;
                }
        }
        if(found)
        {
                printf("The path is \n");
                printf("row col\n");
                for(i = 0; i <= top; i++)
                        printf("%2d%5d",stack[i].row,stack[i].col);
                printf("%2d%5d\n",row,col);
                printf("%2d%5d\n",EXIT_ROW,EXIT_COL);
        }
        else printf("The maze does not have a path\n");
}