// 주석은 참고용



// 소켓 정보 관리 함수 (밑에 추가)
void StoreMessage(const CHAT_MSG* msg);
void SendStoredMessages(SOCKET sock);

                // 소켓 정보 추가: 실패 시 소켓 닫음(각 소켓 밑에 추가)
				else {
					SendStoredMessages(client_sock);
				}
















				case TYPE_CHAT: {
					retval = recv(ptr->sock, (char*)&(ptr->msg) + sizeof(int), sizeof(CHAT_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					StoreMessage((CHAT_MSG*)&(ptr->msg)); // 요거 추가
					CHAT_MSG* chatMsg = (CHAT_MSG*)&(ptr->msg);
					printf("[%s]: %s\n", chatMsg->clientID,chatMsg->msg);

					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { 
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)chatMsg, sizeof(CHAT_MSG), 0);
						}
					}

					
					send(ptr->sock, (char*)chatMsg, SIZE_TOT, 0);
					break;
				}










	
	WSACleanup(); // 무시

	void FreeMessageList(); // main 마지막에 요거 추가

	return 0; // 무시












// 마지막에 요 함수들 추가
void StoreMessage(const CHAT_MSG* msg) {
	MessageNode* newNode = (MessageNode*)malloc(sizeof(MessageNode));
	newNode->message = *msg;
	newNode->next = NULL;

	if (tail == NULL) {
		head = newNode;
		tail = newNode;
	}
	else {
		tail->next = newNode;
		tail = newNode;
	}
}

void SendStoredMessages(SOCKET sock) {
	MessageNode* current = head;
	while (current != NULL) {
		send(sock, (char*)&(current->message), sizeof(CHAT_MSG), 0);
		current = current->next;
	}
}

void FreeMessageList() {
	MessageNode* current = head;
	while (current != NULL) {
		MessageNode* temp = current;
		current = current->next;
		free(temp);
	}
	head = NULL;
	tail = NULL;
}