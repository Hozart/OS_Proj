#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

#define NULL (void*)0

char MyMemory[8192];
long NowMemory;

void* my_malloc(long MemoryLength) {
	NowMemory = NowMemory + MemoryLength + 1;
	return (void*)(&(MyMemory[NowMemory]));
}

struct DirTreeNode
{
	char node[MAX_PATH];
	struct DirTreeNode *parent;
	struct DirTreeNode *brother;
	struct DirTreeNode *child;
}DirTreeNode;

struct DirTreeNode* NewDirTreeNode(char x[MAX_PATH]) {
	struct DirTreeNode* temp = (struct DirTreeNode*)my_malloc(sizeof(struct DirTreeNode));
	strcpy(temp->node, x);
	temp->parent = NULL;
	temp->brother = NULL;
	temp->child = NULL;
	return temp;
}

struct DirTreeNode *DirRootNode;

/********************************************************************************
*
*初始链表
*
********************************************************************************/
struct OriginLinkNode {
	char node[MAX_PATH];
	char parent[MAX_PATH];
	struct OriginLinkNode *next;
};

struct OriginLinkNode* NewOriginLinkNode(char x[MAX_PATH], char y[MAX_PATH]) {
	struct OriginLinkNode* temp = (struct OriginLinkNode*)my_malloc(sizeof(struct OriginLinkNode));
	for(int i = 0;i < MAX_PATH;i++) {
		temp->node[i] = 0;
		temp->parent[i] = 0;
	}
	strcpy(temp->node, x);
	strcpy(temp->parent, y);
	temp->next = NULL;
	return temp;
}

struct OriginLinkNode* OriginLinkStart;
struct OriginLinkNode* OriginLinkEnd;
int LinkLength;

void AddOriginLinkNode(char x[MAX_PATH], char y[MAX_PATH]) {
	OriginLinkEnd->next = NewOriginLinkNode(x, y);
	OriginLinkEnd = OriginLinkEnd->next;
}

/********************************************************************************
*
*栈
*
********************************************************************************/
struct NodeStack
{
	struct DirTreeNode* data;
	struct NodeStack* next;
};

struct NodeStack* NewNodeStack(struct DirTreeNode* x) {
	struct NodeStack* temp = (struct NodeStack*)my_malloc(sizeof(struct NodeStack));
	temp->data = x;
	temp->next = NULL;
	return temp;
}

struct NodeStack* NodeStackTop;

void NodeStackPush(struct DirTreeNode* x) {
	struct NodeStack* temp = NewNodeStack(x);
	temp->next = NodeStackTop;
	NodeStackTop = temp;
}

struct DirTreeNode* NodeStackPop() {
	if (strcmp(NodeStackTop->data->node, "root") == 0) {
		return NULL;
	}
	else {
		struct DirTreeNode* temp = NodeStackTop->data;
		NodeStackTop = NodeStackTop->next;
		return temp;
	}
}


PUBLIC struct DirTreeNode* CurrentNode;

/*
*切换目录
*/
PUBLIC void ChangeCurrentNode(char x[MAX_PATH]) {
	if(strcmp("..",x) == 0) {
		CurrentNode = CurrentNode->parent;
		return;
	}
	struct DirTreeNode* temp = CurrentNode->child;
	while(temp != NULL) {
		if(strcmp(temp->node,x) == 0) {
			CurrentNode = temp;
			return;
		}
		else{
			temp = temp->brother;
		}
	}
}

/*
*展示当初目录下文件及子目录
*/
PUBLIC void ListChildNode() {
	struct DirTreeNode* temp = CurrentNode->child;
	while(temp != NULL) {
		printf("%s ",temp->node);
		temp = temp->brother;
	}
	printf("\n",temp->node);
}

/*
*创建目录
*/
PUBLIC void CreateDirectory(char x[MAX_PATH]) {
	struct DirTreeNode* temp = NewDirTreeNode(x);
	temp->parent=CurrentNode;
	temp->brother=CurrentNode->child;
	CurrentNode->child=temp;
	
	AddOriginLinkNode(x,CurrentNode->node);
}

/*
*创建文件
*/
PUBLIC void CreateFile(char x[MAX_PATH]) {
	int fd = -1;
	fd = open(x, O_CREAT | O_RDWR);
	
	if(fd == -1) {
		return;
	}
	
	int length;
	char buffer[129];
	int n;
	int endwhile = 1;
	
	while(endwhile) {
		length = 0;
		for(int i = 0;i < 129;i++) {
			buffer[i] = 0;
		}
		printf("|");
		scan(buffer);
		for(int i = 0;i < 128;i++) {
			if(buffer[i] == 0) {
				break;
			}
			else if(buffer[i] == '^') {
				length++;
				length++;
				buffer[i + 1] = 0;
				endwhile = 0;
				break;
			}
			length++;
		}
		if(buffer[length - 1] != '\\') {
			buffer[length] = '\n';
			length++;
		}
		else {
			buffer[length - 1] = 0;
			length--;
		}
		n = write(fd, buffer, length);
		assert(n == length);
	}
	
	close(fd);
	
	struct DirTreeNode* temp = NewDirTreeNode(x);
	temp->parent=CurrentNode;
	temp->brother=CurrentNode->child;
	CurrentNode->child=temp;
	
	AddOriginLinkNode(x,CurrentNode->node);
}

/*
*删除文件夹
*/
PUBLIC void RemoveDirectory(char x[MAX_PATH]) {
	struct DirTreeNode* temp = CurrentNode->child;
	struct DirTreeNode* deletenode;
	if(temp == NULL) {
		printf("No Such Directory.\n");
		return;
	}
	if(strcmp(temp->node, x) == 0) {
		CurrentNode->child = temp->brother;
		return;
	}
	while(temp->brother != NULL) {
		if(strcmp(temp->brother->node, x) == 0) {
			deletenode = temp->brother;
			temp->brother = temp->brother->brother;
			//free(deletenode);
			return;
		}
		else{
			temp = temp->brother;
		}
	}
	
	struct OriginLinkNode* tempnode = OriginLinkStart;
	if(strcmp(tempnode->node,x) == 0) {
		tempnode->next = tempnode->next->next;
		return;
	}
	while(tempnode->next!=NULL) {
		if(strcmp(tempnode->next->node,x) == 0) {
			tempnode->next = tempnode->next->next;
		}
	}
}

/*
*删除文件
*/
PUBLIC void RemoveFile(char x[MAX_PATH]) {
	struct DirTreeNode* temp = CurrentNode->child;
	struct DirTreeNode* deletenode;
	if(temp == NULL) {
		printf("No Such File.\n");
		return;
	}
	if(strcmp(temp->node, x) == 0) {
		if (unlink(x) == 0) {
			CurrentNode->child = temp->brother;
			//free(temp);
		}
		else {
			printf("%s is a directory.\n", x);
		}
		CurrentNode->child = temp->brother;
		return;
	}
	while(temp->brother != NULL) {
		if(strcmp(temp->brother->node, x) == 0) {
			if (unlink(x) == 0) {
				deletenode = temp->brother;
				temp->brother = temp->brother->brother;
				//free(deletenode);
			}
			else {
				printf("%s is a directory.\n", x);
			}
			return;
		}
		else{
			temp = temp->brother;
		}
	}
	struct OriginLinkNode* tempnode = OriginLinkStart;
	if(strcmp(tempnode->node,x) == 0) {
		tempnode->next = tempnode->next->next;
		return;
	}
	while(tempnode->next!=NULL) {
		if(strcmp(tempnode->next->node,x) == 0) {
			tempnode->next = tempnode->next->next;
		}
	}
}

/*
*读取并展示文件内容
*/
PUBLIC void ReadFile(char x[MAX_PATH]) {
	struct DirTreeNode* temp = CurrentNode->child;
	if(temp == NULL) {
		printf("No Such File.\n");
		return;
	}
	
	char buffer[2];
	buffer[0] = 0;
	buffer[1] = 0;
	int fd = -1;
	int n;
	int flags = 0;
	while(temp != NULL) {
		if(strcmp(temp->node, x) == 0) {
			
			fd = open(x, O_RDWR);
			if(fd == -1) {
				printf("%s is a directory.\n", x);
				return;
			}
			
			while(1) {
				buffer[0] = 0;
				buffer[1] = 0;
				n = read(fd, buffer, 1);
				if(buffer[0] == 0&&n == 1) {
					flags++;
					if(flags >= 2) {
						break;
					}
				}
				else {
					flags = 0;
				}
				if(buffer[0] == '^') {
					break;
				}
				buffer[1] = 0;
				printf("%s", buffer);
			}
			
			printf("\n");
			
			close(fd);
			
			return;
		}
		else{
			temp = temp->brother;
		}
	}
}

PUBLIC void ReadDirectory() {
	int fd = -1;
	int n;
	fd = open("directorytree", O_RDWR);
	if(fd == -1) {
		char dirfile[2][10][MAX_PATH] = {
			{"a","b","c","d","e","f","g","h","i","j" },
			{"/","/","/","a","a","b","c","c","c","c" }
		};
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				for (int k = 1; k < MAX_PATH; k++) {
					dirfile[i][j][k] = 0;
				}
			}
		}
		for (int i = 0; i < 10; i++) {
			AddOriginLinkNode(dirfile[0][i], dirfile[1][i]);
			LinkLength++;
		}
		return;
	}
	char temp1[MAX_PATH];
	char temp2[MAX_PATH];
	while(1) {
		for(int i = 0;i < MAX_PATH;i++) {
			temp1[i] = 0;
			temp2[i] = 0;
		}
		n = read(fd, temp1, MAX_PATH);
		if(temp1[0] == 0) {
			break;
		}
		n = read(fd, temp2, MAX_PATH);
		if(temp2[0] == 0) {
			break;
		}
		AddOriginLinkNode(temp1, temp2);
		LinkLength++;
	}
	close(fd);
}

PUBLIC void WriteDirectory() {
	int fd = -1;
	int n;
	fd = open("directorytree", O_RDWR);
	if (fd == -1) {
		fd = open("directorytree", O_CREAT | O_RDWR);
	}
	struct OriginLinkNode* temp;
	temp = OriginLinkStart -> next;
	while(temp != NULL) {
		n = write(fd, temp->node , MAX_PATH);
		n = write(fd, temp->parent , MAX_PATH);
		temp = temp->next;
	}
}

int main()
{
	for(int i = 0;i < 8192; i++) {
		MyMemory[i] = 0;
	}
	NowMemory=0;
	
	LinkLength = 0;
	DirRootNode = NewDirTreeNode("/");
	DirRootNode->parent=DirRootNode;
	OriginLinkStart = NewOriginLinkNode("root", "root");
	OriginLinkEnd = OriginLinkStart;
	NodeStackTop = NewNodeStack(NewDirTreeNode("root"));
	CurrentNode=DirRootNode;
	
	ReadDirectory();
	
	struct DirTreeNode* TempTop;
	struct OriginLinkNode* temp1 = OriginLinkStart;
	struct DirTreeNode* temp2;
	struct DirTreeNode* LocalNode;
	NodeStackPush(DirRootNode);
	while (strcmp(NodeStackTop->data->node, "root") != 0) {
		TempTop = NodeStackPop();
		for (int i = 0; i < LinkLength; i++) {
			if (strcmp(temp1->parent, TempTop->node) == 0) {
				temp2 = NewDirTreeNode(temp1->node);
				temp2->parent = TempTop;
				temp2->brother = TempTop->child;
				TempTop->child = temp2;

				NodeStackPush(temp2);
			}
			temp1 = temp1->next;
		}
		temp1 = OriginLinkStart;
	}
	
	char command[8 + MAX_PATH];
	int pos = 0;
	char x[MAX_PATH];
	
	while(1) {
		for(int i = 0;i < 8 + MAX_PATH;i++) {
			command[i] = 0;
		}
		for(int i = 0;i < MAX_PATH;i++) {
			x[i] = 0;
		}
		printf(">> ");
		scan(command);
		pos = 0;
		while(command[pos]!='\n'||command[pos]!=0) {
			if(command[pos] == ' ') {
				command[pos] = 0;
				break;
			}
			if(pos >= 8 + MAX_PATH) {
				break;
			}
			pos++;
		}
		command[pos] = 0;
		if(strcmp(command,"ls") == 0) {
			ListChildNode();
		}
		else if(strcmp(command,"cd") == 0) {
			ChangeCurrentNode(&(command[pos+1]));
		}
		else if(strcmp(command,"mkdir") == 0) {
			CreateDirectory(&(command[pos+1]));
		}
		else if(strcmp(command,"rmdir") == 0) {
			RemoveDirectory(&(command[pos+1]));
		}
		else if(strcmp(command,"vi") == 0) {
			CreateFile(&(command[pos+1]));
		}
		else if(strcmp(command,"rm") == 0) {
			RemoveFile(&(command[pos+1]));
		}
		else if(strcmp(command,"open") == 0) {
			ReadFile(&(command[pos+1]));
		}
		else if(strcmp(command,"exit") == 0) {
			WriteDirectory();
			break;
		}
	}
	return 0;
}