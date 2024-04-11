#include<stdio.h>
#include<stdlib.h>
typedef struct TreeNode *Tree;
struct TreeNode {
    int element;
    Tree  left;
    Tree  right;
};
struct Node {
    struct Node* next;
    Tree rep;
};

struct Stack {
    int size;
    struct Node* top;
};
int Find(int n,int a[],int front,int rear);
Tree newnode(int data);
Tree createTree(Tree head,int inorder[],int postorder[],int front,int rear,int N);
void Print(Tree head);
struct Node* nnewnode(Tree rep);
int main(void){
    int N;
    scanf("%d",&N);
    int inorder[N];
    int postorder[N];
    for(int i=0;i<N;i++){
        scanf("%d",inorder+i);
    }
    for(int i=0;i<N;i++){
        scanf("%d",postorder+i);
    }Tree head=newnode(postorder[N-1]);
    createTree(head,inorder,postorder,0,N,N);
    Print(head);
}
Tree newnode(int data){
    Tree temp=(Tree)malloc(sizeof(struct TreeNode));
    temp->element=data;
    temp->left=NULL;
    temp->right=NULL;
    return temp;
}
struct Node* nnewnode(Tree rep){
    struct Node* temp=(struct Node*)malloc(sizeof(struct Node));
    temp->next=NULL;
    temp->rep=rep;
    return temp;
}
int Find(int n,int a[],int front,int rear){
    for(int i=front;i<rear;i++){
        if(a[i]==n){
            return i;
        }
    }return rear+1;
}
Tree createTree(Tree head,int inorder[],int postorder[],int front,int rear,int N){
    Tree temp;
    int z;
    if(front>=rear){
        return NULL;
    }
    int indexh=Find(postorder[rear-1],inorder,0,N); //在中序遍历中找到后序遍历的最后一个元素的位置
    int index=front-1;
    //找到左子树的根节点
    for(int i=rear-2;i>=front;i--){
        z=Find(postorder[i],inorder,0,N);
        if(z<indexh){
            index=i;
            break;
        }
    }if(rear-2<front){
        return NULL;
    }
    if(index==front-1){
        head->left=NULL;
        temp=newnode(postorder[rear-2]);
        head->right=temp;
    }else if(index==rear-2){
        head->right=NULL;
        temp=newnode(postorder[index]);
        head->left=temp;
    }else{
        temp=newnode(postorder[rear-2]);
        head->right=temp;
        temp=newnode(postorder[index]);
        head->left=temp;
    }
    if(head->left!=NULL)
    createTree(head->left,inorder,postorder, front,index+1,N);
    if(head->right!=NULL)
    createTree(head->right,inorder,postorder,index+1,rear-1,N);
}
void Print(Tree head){
    //按照zigzag顺序输出，方法是建立两个栈，一个栈存放奇数层的节点，一个栈存放偶数层的节点，两个栈的运作方式不同
    struct Stack stack1,stack2;
    stack1.top=NULL;stack2.top=NULL;
    struct Node* temp1;
    struct Node* temp2;
    printf("%d",head->element);//空格小技巧：先打印一个不带空格的第一个数，之后每个数打印前加一个空格
    if(head->right!=NULL){
    temp1=nnewnode(head->right);
    stack1.top = temp1;
    }if(head->left!=NULL){
    temp1=nnewnode(head->left);
    temp1->next= stack1.top;
    stack1.top = temp1;
    }
    while(stack1.top!=NULL || stack2.top!=NULL){
    while(stack1.top!=NULL){
        printf(" %d",stack1.top->rep->element);
        if(stack1.top->rep->left!=NULL){
        temp2 = nnewnode(stack1.top->rep->left);
        temp2->next=stack2.top;
        stack2.top = temp2;}
        if(stack1.top->rep->right!=NULL){
        temp2=nnewnode(stack1.top->rep->right);
        temp2->next=stack2.top;
        stack2.top=temp2;}
        stack1.top=stack1.top->next;
    }
    while(stack2.top!=NULL){
        printf(" %d",stack2.top->rep->element);
        if(stack2.top->rep->right!=NULL){
        temp1 = nnewnode(stack2.top->rep->right);
        temp1->next=stack1.top;
        stack1.top = temp1;}
        if(stack2.top->rep->left!=NULL){
        temp1=nnewnode(stack2.top->rep->left);
        temp1->next=stack1.top;
        stack1.top=temp1;}
        stack2.top=stack2.top->next;
    }
    }
}