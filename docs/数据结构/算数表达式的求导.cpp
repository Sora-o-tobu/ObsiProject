#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* letter[100];
int index=0;
typedef struct Node{
    char data[100];
    int isOperator; //0 for number or variable, 1 for operator
    struct Node* left;
    struct Node* right;
}Node;

int isOperator(char* c);
Node* newNode(char* data, int oper);
Node* differentiate(Node* node,char* letter);
int getPrecedence(char* oper);
Node* infixToExpressionTree(char* infix,int length);
void inorderprint(Node* root,int deep);
Node* simplify(Node* root);





int main(void){
    char infix[100];
    scanf("%[^\n]",infix);
    for(int i=0;i<strlen(infix)-2;i++){ 
        //To process the negative number, add a '0' before the negative number
        if(infix[i]=='(' && infix[i+1]=='-'){
            for(int j=strlen(infix);j>i;j--){
                infix[j]=infix[j-1];
            }
            infix[i+1]='0';
        }
    }
    Node* root = infixToExpressionTree(infix,strlen(infix));
    puts("");
    for(int x=0;x<index;x++){
        //Sort the variables in the expression in lexicographical order
        for(int i=0;i<index-1;i++){
            if(strcmp(letter[i],letter[i+1])>0){ //O(n^2)
                char* temp=letter[i];
                letter[i]=letter[i+1];
                letter[i+1]=temp;
            }
        }
    }
    root = simplify(root);
    for(int x=0;x<index;x++){
        printf("%s: ",letter[x]);
        Node* diff = differentiate(root,letter[x]);
        diff = simplify(diff);
        inorderprint(diff,1);
        puts("");
    }
}







int isOperator(char* c) {
    return strcmp(c, "+") == 0 || strcmp(c, "-") == 0 || strcmp(c, "*") == 0 || strcmp(c, "/") == 0 || strcmp(c,"^")==0;
}

Node* newNode(char* data, int oper){
    //Use dynamic memory allocation to initialize a new node
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->data, data);
    node->isOperator = oper;
    node->left = node->right = NULL;
    return node;
}

Node* differentiate(Node* node,char* letter){
    if (node == NULL) {
        return NULL;
    }
    // This part I explaint in the report,please read it.
    if (node->isOperator) {
        if (strcmp(node->data, "+") == 0 || strcmp(node->data, "-") == 0) {
            Node* newNodePtr = newNode(node->data, 1);
            newNodePtr->left = differentiate(node->left, letter);
            newNodePtr->right = differentiate(node->right, letter);
            return newNodePtr;
        } else if (strcmp(node->data, "*") == 0) {
            Node* newNodePtr = newNode("+", 1);
            newNodePtr->left = newNode("*", 1);
            newNodePtr->left->left = differentiate(node->left,letter);
            newNodePtr->left->right = node->right;
            newNodePtr->right = newNode("*", 1);
            newNodePtr->right->left = node->left;
            newNodePtr->right->right = differentiate(node->right,letter);
            return newNodePtr;
        } else if (strcmp(node->data, "/") == 0) {
            Node* newNodePtr = newNode("/", 1);
            newNodePtr->left = newNode("-", 1);
            newNodePtr->left->left = newNode("*", 1);
            newNodePtr->left->left->left = differentiate(node->left,letter);
            newNodePtr->left->left->right = node->right;
            newNodePtr->left->right = newNode("*", 1);
            newNodePtr->left->right->left = node->left;
            newNodePtr->left->right->right = differentiate(node->right,letter);
            newNodePtr->right = newNode("*", 1);
            newNodePtr->right->left = node->right;
            newNodePtr->right->right = node->right;
            return newNodePtr;
        } else if (strcmp(node->data, "sin") == 0) {
            Node* newNodePtr = newNode("*", 1);  //We restrict that the value is in right child.
            newNodePtr->left = newNode("cos", 1);
            newNodePtr->left->right = node->right;
            newNodePtr->right = differentiate(node->right,letter);
            return newNodePtr;
        } else if (strcmp(node->data, "cos") == 0) {
            Node* newNodePtr = newNode("*", 1);
            newNodePtr->left = newNode("-", 1);
            newNodePtr->left->left = newNode("0", 0);
            newNodePtr->left->right = newNode("sin", 1);
            newNodePtr->left->right->right = node->right;
            newNodePtr->right = differentiate(node->right,letter);
            return newNodePtr;
        }else if (strcmp(node->data,"tan")==0){
            Node* newNodePtr = newNode("*", 1);
            newNodePtr->left = newNode("/", 1);
            newNodePtr->left->left = newNode("1", 0);
            newNodePtr->left->right = newNode("^", 1);
            newNodePtr->left->right->left = newNode("cos", 1);
            newNodePtr->left->right->left->right = node->right;
            newNodePtr->left->right->right = newNode("2", 0);
            newNodePtr->right = differentiate(node->right,letter);
            return newNodePtr;
        }else if (strcmp(node->data, "ln") == 0){
            Node* newNodePtr = newNode("/", 1);
            newNodePtr->left = differentiate(node->right,letter);
            newNodePtr->right = node->right;
            return newNodePtr;
        }else if (strcmp(node->data,"exp")==0){
            Node* newNodePtr = newNode("*", 1);
            newNodePtr->left = differentiate(node->right,letter);
            newNodePtr->right = newNode("^",1);
            newNodePtr->right->left = newNode("exp",1);
            newNodePtr->right->right = node->right;
            return newNodePtr;
        }else if(strcmp(node->data,"^")==0){
            Node* newNodePtr = newNode("*", 1);
            newNodePtr->left = node;
            newNodePtr->right= newNode("+",1);
            newNodePtr->right->left = newNode("*",1);
            newNodePtr->right->left->left = differentiate(node->right,letter);
            newNodePtr->right->left->right = newNode("ln",1);
            newNodePtr->right->left->right->right = node->left;
            newNodePtr->right->right = newNode("*",1);
            newNodePtr->right->right->left = node->right;
            newNodePtr->right->right->right = newNode("/",1);
            newNodePtr->right->right->right->left = differentiate(node->left,letter);
            newNodePtr->right->right->right->right = node->left;
            return newNodePtr;
        }else if(strcmp(node->data,"log")==0){
            Node* newNodePtr = newNode("*", 1);
            newNodePtr->left = newNode("/",1);
            newNodePtr->left->left=newNode("1",0);
            newNodePtr->left->right = newNode("ln",1);
            newNodePtr->left->right->right = node->left;
            newNodePtr->right = newNode("-",1);
            newNodePtr->right->left = newNode("/",1);
            newNodePtr->right->left->left = differentiate(node->right,letter);
            newNodePtr->right->left->right = node->right;
            newNodePtr->right->right = newNode("*",1);
            newNodePtr->right->right->left = newNode("/",1);
            newNodePtr->right->right->left->left = differentiate(node->left,letter);
            newNodePtr->right->right->left->right = node->left;
            newNodePtr->right->right->right = newNode("/",1);
            newNodePtr->right->right->right->left = newNode("ln",1);
            newNodePtr->right->right->right->left->right = node->right;
            newNodePtr->right->right->right->right = newNode("ln",1);
            newNodePtr->right->right->right->right->right = node->left;
            return newNodePtr;
        }
    }else{//Recursion termination condition
        if(strcmp(node->data, letter)==0){ 
            return newNode("1", 0);
        }else{
            return newNode("0", 0);
        }
    }
    return NULL;
}

int getPrecedence(char* oper){
    //The precedence '^'> '*' or '/' > '+' or '-'
    if(strcmp(oper, "+") == 0 || strcmp(oper, "-") == 0){
        return 1;
    }else if(strcmp(oper, "*") == 0 || strcmp(oper, "/") == 0){
        return 2;
    }else if(strcmp(oper,"^")==0){
        return 3;
    }else{
        return 0;
    }
}

Node* infixToExpressionTree(char* infix,int length) {
    Node* nodeStack[100];
    //To store the node
    int top = -1;
    char operatorStack[100][20];
    //To store the operator
    int topOp = -1;
    char* token = (char*)malloc(10*sizeof(char));
    int i=0;
    while(i<length){
        sprintf(token,"%c",infix[i]);
        if (isOperator(token)) {
            while (topOp != -1 && getPrecedence(operatorStack[topOp]) >= getPrecedence(token)) {
                /*
                *If the precedence of the operator in the stack is greater 
                *than or equal to the precedence of the current operator, 
                *then pop the operator in the stack and construct the expression tree
                */
                Node* right = nodeStack[top--];
                Node* left = nodeStack[top--];
                Node* node = newNode(operatorStack[topOp--], 1);
                node->left = left;
                node->right = right;
                nodeStack[++top] = node;
            }
            strcpy(operatorStack[++topOp], token);
        } else if (strcmp(token, "(") == 0) {
            strcpy(operatorStack[++topOp], token);
        } else if (strcmp(token, ")") == 0) {
            while (topOp != -1 && strcmp(operatorStack[topOp], "(") != 0) {
                Node* right = nodeStack[top--];
                Node* left = nodeStack[top--];
                Node* node = newNode(operatorStack[topOp--], 1);
                node->left = left;
                node->right = right;
                nodeStack[++top] = node;
            }
            topOp--;
        }else if(infix[i]=='l' && infix[i+1]=='o' && infix[i+2]=='g'){
            i+=3;
            strcpy(token,"log");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,',')-(long long)infix)-i-1;
            /*
            *cast from 'char*' to 'int' loses precision,
            *so I must use long long to store the address of the character.
            */
            nodeStack[top]->left=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
            temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1;
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if(infix[i]=='p' && infix[i+1]=='o' && infix[i+2]=='w'){
            i+=3;
            strcpy(token,"^");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,',')-(long long)infix)-i-1;
            nodeStack[top]->left=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
            temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1;
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if(infix[i]=='c' && infix[i+1]=='o' && infix[i+2]=='s'){
            i+=3;
            strcpy(token,"cos");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1; 
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if(infix[i]=='s' && infix[i+1]=='i' && infix[i+2]=='n'){
            i+=3;
            strcpy(token,"sin");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1;
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if (infix[i] == 't' && infix[i+1] == 'a' && infix[i+2] == 'n') {
            i+=3;
            strcpy(token,"tan");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1;
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if(infix[i]=='l' && infix[i+1]=='n'){
            i+=2;
            strcpy(token,"ln");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1;
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if(infix[i]=='e' && infix[i+1]=='x' && infix[i+2]=='p'){
            i+=3;
            strcpy(token,"exp");
            nodeStack[++top] = newNode(token, 1);
            int temp=((long long)strchr(infix+i+1,')')-(long long)infix)-i-1;
            nodeStack[top]->right=infixToExpressionTree(infix+i+1,temp);
            i+=temp+1;
        }else if(infix[i]>='0' && infix[i]<='9'){
            //Process the number
            sscanf(infix + i, "%[0-9]", token);
            nodeStack[++top] = newNode(token, 0);
            i+=strlen(token)-1;
        }else{
            //This case is for the variable
            int flag=1;
            sscanf(infix+i,"%[a-zA-Z]",token); //Read the maybe long name variable
            nodeStack[++top] = newNode(token, 0);
            for(int j=0;j<index;j++){
                //If the variable has been stored, then don't store it again
                if(strcmp(letter[j],token)==0){
                    flag=0;
                    break;
                }
            }if(flag){
            letter[index]=(char*)malloc((strlen(token)+1)*sizeof(char));
            sprintf(letter[index],"%s",token);
            index++;
            }i+=strlen(token)-1;
        }i++;
        free(token);
        token = (char*)malloc(10*sizeof(char));
    }

    while (topOp != -1) {
        //Pop the remaining operators in the stack
        Node* right = nodeStack[top--];
        Node* left = nodeStack[top--];
        Node* node = newNode(operatorStack[topOp--], 1);
        node->left = left;
        node->right = right;
        nodeStack[++top] = node;
    }

    return nodeStack[top];
}

void inorderprint(Node* root,int deep){
    if(root==NULL)return;
    else if(root->left==NULL && root->right==NULL)printf("%s",root->data);
    else{
        //To correctly print the expression tree, we need to consider the precedence of the operator
        if(strcmp(root->data,"log")==0){
            printf("log(");
            inorderprint(root->left,deep+1);
            printf(",");
            inorderprint(root->right,deep+1);
            printf(")");
        }else{
            /*
            *Before traverse the left child, add a left bracket, 
            *after traverse the right child, add a right bracket,
            *except the root node and the leaf node.
            */
            if(deep>1)printf("(");
            inorderprint(root->left,deep+1);  
            printf("%s",root->data);
            inorderprint(root->right,deep+1);
            if(deep>1)printf(")");
        }
    }
}

Node* simplify(Node* root){
    // This part I explaint in the report,please read it.
    if(root==NULL)return NULL;
    if(root->left==NULL && root->right==NULL)return root;
    root->left=simplify(root->left);
    root->right=simplify(root->right);
    if(root->isOperator){
        if(strcmp(root->data,"+")==0){
            if(strcmp(root->left->data,"0")==0)return root->right;
            if(strcmp(root->right->data,"0")==0)return root->left;
        }else if(strcmp(root->data,"-")==0){
            if(strcmp(root->right->data,"0")==0)return root->left;
            if(strcmp(root->left->data,root->right->data)==0 && root->left->isOperator!=1)return newNode("0",0);
        }else if(strcmp(root->data,"*")==0){
            if(strcmp(root->left->data,"0")==0 || strcmp(root->right->data,"0")==0)return newNode("0",0);
            if(strcmp(root->left->data,"1")==0)return root->right;
            if(strcmp(root->right->data,"1")==0)return root->left;
        }else if(strcmp(root->data,"/")==0){
            if(strcmp(root->left->data,"0")==0)return newNode("0",0);
            if(strcmp(root->right->data,"1")==0)return root->left;
            if(strcmp(root->left->data,root->right->data)==0 && root->left->isOperator!=1)return newNode("1",0);
        }else if(strcmp(root->data,"^")==0){
            if(strcmp(root->right->data,"0")==0)return newNode("1",0);
            if(strcmp(root->right->data,"1")==0)return root->left;
            if(strcmp(root->left->data,"1")==0)return newNode("1",0);
        }else if(strcmp(root->data,"ln")==0){
            if(strcmp(root->right->data,"1")==0)return newNode("0",0);
        }else if(strcmp(root->data,"log")==0){
            if(strcmp(root->right->data,"1")==0)return newNode("0",0);
            if(strcmp(root->left->data,root->right->data)==0 && root->left->isOperator!=1)return newNode("1",0);
        }
    }
    return root;
}
