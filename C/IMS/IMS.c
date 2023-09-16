#include <stdio.h>
#include <string.h>
#include <malloc.h>

FILE *fp1, *fp2;
typedef struct node *node_ptr;

typedef struct node {
	int component;
	int inventory;
	int reorder;
	char name[20];
	node_ptr left;
	node_ptr right;
} node;

void list(node_ptr root, FILE *fp2);
void insertnode(node_ptr *root, node_ptr newItem, FILE *fp2);
node* search(node_ptr root, int component, FILE *fp2);
void check(node_ptr item, int inventory, FILE *fp2);
void delete_node(node_ptr *root, int component, FILE *fp2); //삭제


int main() {
	node_ptr item = NULL;
	node_ptr newnode;
	int *num;
	int i, j, t, n, a, c, d;  //i는 반복 a는 component b는 inventory c는 reorder
	int temp1[10] = { 0, }; //component일시저장
	int temp2[3] = { 0, }; //inventory
	int temp3[3] = { 0, }; //reorder	
	char data; //알파벳 저장용
	char b[20] = { 0 }; //name저장
	char buffer[50] = { 0, };  //한줄씩읽음
	fp1 = fopen("inventory.txt", "r");
	fp2 = fopen("write.txt", "w");

	while (!feof(fp1)) {
		newnode = malloc(sizeof(node));  //newnode 동적할당
		newnode->left = NULL;  //초기화
		newnode->right = NULL;
		fgets(buffer, 50, fp1);
		data = buffer[0];
		a = 0, c = 0, d = 0;  //저장하기전 초기화

		switch (data){
		case 'N':  //컴포넌트 추가 

			for (i = 2; i < 8; i++) {  //componenet
				if (buffer[i] == ' ')
					i++;
				else {
					j = i;
					temp1[i] = buffer[i] - '0';

					for (j = i - 2; j < 5; j++)
					{
						temp1[i] = temp1[i] * 10;
					}
					a += temp1[i];
				}
			}
			j = 0;
			for (i = 9; i < 28; i++) { //name
				b[j] = buffer[i];
				j++;
			}
			j = 0;
			for (i = 30; i < 35; i++) { //inventory
				if (buffer[i] == ' ') {
					i = i + 1;
				}
				if (buffer[i] > '0') {
					temp2[j] = buffer[i] - '0';
					j++;
				}
			}
			n = j;
			for (t = 0; t < n; t++)
			{
				for (i = 1; i < j; i++)
				{
					temp2[t] = temp2[t] * 10;
					j--;
				}
				c += temp2[t];
			}
			for (i = 40; i < 45; i++)  //reorder
			{
				if (buffer[i] == ' ')
				{
					i = i + 1;
				}
				if (buffer[i] > '0')
				{
					temp3[j] = buffer[i] - '0';
					j++;
				}
			}
			n = j;
			for (t = 0; t < n; t++)
			{
				for (i = 1; i < j; i++)
				{
					temp3[t] = temp3[t] * 10;
					j--;
				}
				d += temp3[t];
			}

			newnode->component = a;
			strcpy_s(newnode->name, 20, b);
			newnode->inventory = c;
			newnode->reorder = d;
			insertnode(&item, newnode, fp2);

			break;

		case 'D':  //컴포넌트 삭제
			for (i = 2; i < 8; i++) {  //componenet
				if (buffer[i] == ' ')
					i++;
				else {
					j = i;
					temp1[i] = buffer[i] - '0';

					for (j = i - 2; j < 5; j++) {
						temp1[i] = temp1[i] * 10;
					}
					a += temp1[i];

				}
			}
			newnode->component = a;
			delete_node(&item, a, fp2);
			break;

		case 'A':  //재고량 추가
			for (i = 2; i < 9; i++) {  //componenet
				if (buffer[i] == ' ')
					i++;
				else {
					j = i;
					temp1[i] = buffer[i] - '0';

					for (j = i - 2; j < 5; j++) {
						temp1[i] = temp1[i] * 10;
					}
					a += temp1[i];
				}
			}
			j = 0;

			for (i = 30; i < 35; i++) { //inventory
				if (buffer[i] == ' ') {
					i = i + 1;
				}
                
				if (buffer[i] > '0') {
					temp2[j] = buffer[i] - '0';
					j++;
				}
			}
			n = j;

			for (t = 0; t < n; t++) {
				for (i = 1; i < j; i++) {
					temp2[t] = temp2[t] * 10;
					j--;
				}
				c += temp2[t];
			}
			newnode->component = a;
			check(search(item, a, fp2), c, fp2);
			break;

		case 'R': //재고량 제거
			for (i = 2; i < 8; i++) {  //componenet
				if (buffer[i] == ' ')
					i++;
				else {
					j = i;
					temp1[i] = buffer[i] - '0';
					for (j = i - 2; j < 5; j++) {
						temp1[i] = temp1[i] * 10;
					}
					a += temp1[i];
				}
			}
			j = 0;

			for (i = 30; i < 35; i++) { //inventory
				if (buffer[i] == ' ') {
					i = i + 1;
				}
				if (buffer[i] > '0') {
					temp2[j] = buffer[i] - '0';
					j++;
				}
			}
			n = j;

			for (t = 0; t < n; t++) {
				for (i = 1; i < j; i++) {
					temp2[t] = temp2[t] * 10;
					j--;
				}
				c += temp2[t];
			}
			newnode->component = a;
			check(search(item, a, fp2), c*-1, fp2);
			break;

		case 'P':  //목록 시스템 출력
			list(item, fp2);
			break;

		case 'X':  //프로그램 끝냄
			exit(1);
			break;
		}
	}
}

void list(node_ptr root, FILE *fp2) {   //LVR출력
	if (root != NULL) {
		list(root->left, fp2);
		fprintf_s(fp2, "%d  %s  %d  %d ", root->component, root->name, root->inventory, root->reorder);

		if (root->inventory < root->reorder)
			fprintf_s(fp2, "*재주문요*");
		fprintf_s(fp2, "\n");
		list(root->right, fp2);
	}
}

node_ptr search(node_ptr root, int component, FILE *fp2)
{

	while (root != NULL) { // root가 NULL이 아니라면 일단 탐색을 계속한다.

		// 원하는 키 값을 발견했을 경우
		if (component == root->component)
			return root; // 해당 노드의 주소 반환
						 // 찾고자 하는 값보다 루트키값이 더 큰 경우
		else if (component < root->component)
			root = root->left; // 왼쪽 서브트리로 이동
							   // 찾고자 하는 값보다 루트키값이 더 작은 경우
		else if ((component > root->component))
			root = root->right; // 오른쪽 서브트리로 이동
	}
	fprintf_s(fp2, "%d  컴포넌트 번호 에러\n", component);
	return NULL;
}

void insertnode(node_ptr *root, node_ptr newItem, FILE *fp2) {
	node_ptr parent, temp;    // parent는 부모노드, temp는 현재노드 
	int t; //컴포넌트 값 받는 변수
	parent = NULL;
	temp = *root;


	while (temp != NULL) {  //값이 이미 있는지 확인

		if (newItem->component == temp->component) {
			t = temp->component;
			fprintf_s(fp2, "%d  컴포넌트 번호 중복\n", t);
			return; // 값이 이미 존재한다.
		}
        
		parent = temp;
		if (newItem->component < temp->component)     //temp가 루트이므로 새로운 컴포넌트가 루트보다 크면 왼쪽 작으면 오른쪽
			temp = temp->left;
		else if (newItem->component > temp->component)
			temp = temp->right;
	}

	// 부모 노드와 링크 연결
	if (parent == NULL)
		*root = newItem;
	else if (newItem->component < parent->component)
		parent->left = newItem;
	else
		parent->right = newItem;

}

void delete_node(node_ptr *root, int component, FILE *fp2) {
	node_ptr parent, child, suc, suc_p, t;

	// component를 갖는 노드 t를 탐색  parent는 t의 부모노드
	parent = NULL; //  root의 부모노드x.
	t = *root;

	// component를 갖는 노드 t를 탐색.
	while (t != NULL && t->component != component) {
		parent = t;
		// t는 왼쪽이나 componenet값에 따라 왼쪽이나 오른쪽으로 내려간다.
		if (component < parent->component)
			t = parent->left;
		else if (component > parent->component)
			t = parent->right;
	}

	// 탐색이 끝난 시점에서 만약 키 값이 트리에 없었다면 t는 NULL일 것이다.
	if (!t) {
		fprintf_s(fp2, "%d  컴포넌트 번호 없음\n", component);
		return; // 원하는 값이 없으므로 return 한다.
	}

	// 단말노드일때
	if (t->left == NULL && t->right == NULL) {
		if (parent) { // 부모 노드가 NULL이 아닐 때
			if (parent->left == t)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else // 부모 노드가 NULL 이면, 삭제하려는 노드가 루트 노드
			*root = NULL;
	}

	// 하나의 서브트리만 가지는 경우    삭제하고 서브트리를 부모노드에 연결해야함
	else if ((t->left == NULL) || (t->right == NULL)) {
		child = (t->left != NULL) ? t->left : t->right;
		if (parent) {
			if (parent->left == t)
				parent->left = child;
			else
				parent->right = child;
		}
		else // 부모 노드가 NULL이면 삭제되는 노드가 루트노드다.
			*root = child;
	}
	//두개의 서브트리를 모두 가지는 경우
	else {
		// 오른쪽 서브트리에서 가장 작은 값을 찾는다.
		suc_p = t;
		suc = t->right;
		while (suc->left != NULL) {
			suc_p = suc;
			suc = suc->left;
		}

		// 후속자의 부모와 자식을 연결
		if (suc_p->left == suc)
			suc_p->left = suc->right;

		// 왼쪽 끝까지 내려왔지만 그 끝에 오른쪽 서브트리가 있을 경우가 있어서 suc->right 값을 대입
		else
			suc_p->right = suc->right;

		t->component = suc->component;
		t = suc;
	}
	free(t);
}

void check(node_ptr item, int inventory, FILE *fp2) {  //재고량 검사
	if (item) {
		if (item->inventory + inventory < 0) {
			fprintf_s(fp2, "%d 재고량이 충분하지 않음\n", item);
			return;
		}
		item->inventory = item->inventory + inventory;

	}
}