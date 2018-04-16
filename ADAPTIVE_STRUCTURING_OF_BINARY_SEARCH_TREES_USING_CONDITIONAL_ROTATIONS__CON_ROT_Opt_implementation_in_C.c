#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define SIZE 3

struct node_{
       
  struct node_ *leftChild;
  struct node_ *rightChild;
  int number;
  int t;
  
};
struct stack_{
int size; 
int top;
int * value;
};


typedef struct node_ Node;
typedef struct stack_ stack;

stack * theStack=NULL;
Node * root=NULL;
Node *holder;
int a;
Node *tempRoot=NULL,*tempParent=NULL,*tempAccess=NULL;

int calculate_T(Node *parentNode);
Node * CON_ROT_Optimized(Node *record,int number);
void initStack();
int isOverFlow();
void push(Node *path);
int pop();
Node *  InsertTree(Node *node,int number);
void println_Infix(Node *node);
void println_Prefix(Node *node);
void println_Postfix(Node *node);
void instruction();
Node * entry_Con();
Node * CON_ROT_Optimized(Node *record,int number);
void rotation(Node **root,Node **parent,Node **access); 
void right_Rotation(Node **root,Node **parent,Node **access);
void left_Rotation(Node **root,Node **parent,Node **access);
void addNode();

void initStack()
{

 
 theStack=(stack *)calloc(1,sizeof(stack));
 theStack->value=(int *)calloc(3,sizeof(int));
 
 //theStack->top=-1;

}
int isOverFlow()
{

 if(theStack->top==-1)
  return 1;
 else
  return 0;

 
}

void push(Node *path)
{   
 
 theStack->top%=3;
 
 theStack->value[theStack->top++]=(int)path;        
 

}
int pop()
{
 int k;

 
 k=--theStack->top;
 if(isOverFlow())
 {
 theStack->top=2;
 k=theStack->top;
 }

  return theStack->value[k];

    
}


Node *  InsertTree(Node *node,int number)
{

 if(node==NULL)
 {
 node=(Node *)calloc(1,sizeof(Node));
 node->number=number; 
 return node;
 }
   else if(number<node->number)
   {
     node->leftChild=InsertTree(node->leftChild,number);                      
    }
   else if(number>node->number)
   {
   node->rightChild=InsertTree(node->rightChild,number);     
    }
    return node;
    
}
void println_Infix(Node *node)
{
     if(node!=NULL) 
     {
     println_Infix(node->leftChild);
     printf("number --> %d  t:  %d \n",node->number,node->t);
     println_Infix(node->rightChild);
     
     }
     
}
void println_Prefix(Node *node) 
{
     if(node!=NULL)
     {
     println_Prefix(node->leftChild);     
     println_Prefix(node->rightChild);
     printf("number --> %d  t:  %d \n",node->number,node->t);
     } 
     
}
void println_Postfix(Node *node)
{
     if(node!=NULL)
     {
 
 printf("number --> %d   t:  %d \n",node->number,node->t);
    println_Postfix(node->leftChild);     
    println_Postfix(node->rightChild);

     
     }
     
}

void initializeTree(){

 root=(Node *)InsertTree(root,54);
    root=(Node *)InsertTree(root,83);
    root=(Node *)InsertTree(root,92);
    root=(Node *)InsertTree(root,72);
    root=(Node *)InsertTree(root,81);
    root=(Node *)InsertTree(root,65); 
 root=(Node *)InsertTree(root,18);
 root=(Node *)InsertTree(root,12);
    root=(Node *)InsertTree(root,10);
    root=(Node *)InsertTree(root,14);
    root=(Node *)InsertTree(root,28);
    root=(Node *)InsertTree(root,20); 
    root=(Node *)InsertTree(root,37);
 

}
void println()
{
 int a;
 printf("1 :Infix\n2 :Prefix\n3 :Postfix");
 printf("\nEntry :");
 scanf("%d",&a);

 switch(a){

 case 1: printf("-----Infix-------\n"); 
    println_Infix(root); break;
 case 2 :printf("-----Prefix------\n");
 println_Prefix(root); break;
 case 3:printf("-----Postfix-----\n"); 
 println_Postfix(root); break;

 }
 
}
int result(Node * node){

 if(node==NULL){
  printf("\nThe search key %d is not found in the tree \n\n",a);
  //return -1; //Unsuccess key
 }
 else{
  return node->number;
 }

}
void instruction()
{
 int chose;
 
    
 printf("What do you want to do?\n1:Addition a node to the tree\n2:Conditional Rotation\n3:Print Tree\n4:Exit");
    printf("\nEntry :"); 
 scanf("%d",&chose);
    switch(chose){ 
        case 1:addNode(); break;
        case 2:result(entry_Con()); break;        
        case 3:println(); break;
  case 4:exit(1); break;
        } 
 instruction();
}
void addNode()
{

 int a;

    printf("Enter a integer : ");
    scanf("%d",&a);
    root=(Node *)InsertTree(root,a);
 
}
Node * entry_Con()
{
       
printf("Enter a search key in the tree :");     
scanf("%d",&a);
initStack();

  return CON_ROT_Optimized(root,a);  
     
    
    
}
Node * CON_ROT_Optimized(Node *record,int number)
{
    int w=0;
    
 record->t++;

 push(record);

 if(record->number==number)
 {
  tempAccess=(Node *)pop();
  tempParent=(Node *)pop();
  tempRoot=(Node *)pop();

  if(tempParent==NULL)
   w=0;

  else if(tempParent->leftChild==tempAccess)
  {
   w=2*calculate_T(tempAccess)-calculate_T(tempAccess->rightChild)-calculate_T(tempParent);
   
  }
  else if(tempParent->rightChild==tempAccess) 
  {
   w=2*calculate_T(tempAccess)-calculate_T(tempAccess->leftChild)-calculate_T(tempParent);
  }
   if(w>0)
   {
   
      if(tempRoot==NULL)
   {
   rotation(&tempRoot,&tempParent,&tempAccess); 
      root=tempRoot;
   }
   else
   {
   rotation(&tempRoot,&tempParent,&tempAccess);
   
   }

  tempAccess->t=calculate_T(tempAccess);
  tempRoot->t=calculate_T(tempRoot);
        
   }
  
  
  return tempAccess;
 }
 else
 {
  if(number<record->number)
  {
             if(record->leftChild!=NULL){                    
   CON_ROT_Optimized(record->leftChild,number);
        }
        else{
            
             return  NULL;
             }
  }else
  {
              if(record->rightChild!=NULL){
   CON_ROT_Optimized(record->rightChild,number); 
          }
          else{
               
               return NULL;              
               }
  }
 }
    

}//End method
//End Algorithm Oprimized_CON_ROT
int calculate_T(Node *parentNode)
{
 

 if(parentNode==NULL)
  return 0;
 else
    return parentNode->t;

    
}

void left_Rotation(Node **root,Node **parent,Node **access)
{
     
   
     holder=*parent;
     printf("\nLeft Rotation\n\n");
     
      if(*root==NULL)
     {
           *root=(*parent)->leftChild;            
          (*parent)->leftChild=(*access)->rightChild;    
    (*root)->rightChild=*parent;  
                
                    
     }
     else if((*root)->leftChild==*parent)
     {
     (*root)->leftChild=*access;
     (*parent)->leftChild=(*access)->rightChild;
     (*access)->rightChild=holder;
                              
                                 
     }
     else if((*root)->rightChild==*parent) 
     {
     (*root)->rightChild=*access;
     (*parent)->leftChild=(*access)->rightChild;
     (*access)->rightChild=holder;       
          
          
     }
}
void right_Rotation(Node **root,Node **parent,Node **access) 
{
     holder=*parent;
     printf("\nRight Rotation\n\n");
     
     if(*root==NULL)
     {
     *root=(*parent)->rightChild;
          (*parent)->rightChild=(*access)->leftChild;   
          (*root)->leftChild=*parent;
            
                          
     } 
     else if((*root)->leftChild==*parent)
     {
     (*root)->leftChild=*access;
     (*parent)->rightChild=(*access)->leftChild; 
     (*access)->leftChild=holder;                     
                                 
     }
     else if((*root)->rightChild==*parent)
     {
     (*root)->rightChild=*access;
     (*parent)->rightChild=(*access)->leftChild; 
     (*access)->leftChild=holder;     
     
     }
  
 
}
void rotation(Node **tempRoot,Node **tempParent,Node **tempAccess)
{


 if(*tempRoot==NULL)
  {
  

  if((*tempParent)->rightChild->number==(*tempAccess)->number)
  right_Rotation(tempRoot,tempParent,tempAccess);
  else
  left_Rotation(tempRoot,tempParent,tempAccess);
  
  
 }else
  if((*tempParent)->leftChild!=NULL){  
 if((*tempParent)->leftChild->number==(*tempAccess)->number)
 {

  left_Rotation(tempRoot,tempParent,tempAccess);

 }
 }
  if((*tempParent)->rightChild!=NULL){
 if((*tempParent)->rightChild->number==(*tempAccess)->number)
 {
  right_Rotation(tempRoot,tempParent,tempAccess);

 }
 }

}
void timer(){

 initStack();
root=(Node *)InsertTree(root,20);
    root=(Node *)InsertTree(root,12);
    root=(Node *)InsertTree(root,28);
    root=(Node *)InsertTree(root,7);
    root=(Node *)InsertTree(root,14);
    root=(Node *)InsertTree(root,18);
    root=(Node *)InsertTree(root,37);
    root=(Node *)InsertTree(root,92);
    root=(Node *)InsertTree(root,65);
    root=(Node *)InsertTree(root,67);
    root=(Node *)InsertTree(root,72); 
    root=(Node *)InsertTree(root,81);
    root=(Node *)InsertTree(root,99);
    root=(Node *)InsertTree(root,83);
    root=(Node *)InsertTree(root,27);
    root=(Node *)InsertTree(root,57);
    root=(Node *)InsertTree(root,35); 
    root=(Node *)InsertTree(root,48);
    root=(Node *)InsertTree(root,9);
    root=(Node *)InsertTree(root,5);
    root=(Node *)InsertTree(root,2);
    root=(Node *)InsertTree(root,78);
    root=(Node *)InsertTree(root,24); 
    root=(Node *)InsertTree(root,56);
    root=(Node *)InsertTree(root,100);
    root=(Node *)InsertTree(root,3);
    root=(Node *)InsertTree(root,8);
    root=(Node *)InsertTree(root,17);
    root=(Node *)InsertTree(root,23); 
    root=(Node *)InsertTree(root,29);
    root=(Node *)InsertTree(root,21);
    root=(Node *)InsertTree(root,77);
    root=(Node *)InsertTree(root,66);
    root=(Node *)InsertTree(root,88);
    root=(Node *)InsertTree(root,55); 
    root=(Node *)InsertTree(root,44);
    root=(Node *)InsertTree(root,33);
    root=(Node *)InsertTree(root,22);
    root=(Node *)InsertTree(root,11);
    root=(Node *)InsertTree(root,0);
    root=(Node *)InsertTree(root,87); 
    root=(Node *)InsertTree(root,90);
    root=(Node *)InsertTree(root,79);
    root=(Node *)InsertTree(root,14);
    root=(Node *)InsertTree(root,79);
    root=(Node *)InsertTree(root,53);
    root=(Node *)InsertTree(root,28); 
    root=(Node *)InsertTree(root,6);
    
    root=(Node *)InsertTree(root,201);
    root=(Node *)InsertTree(root,122);
    root=(Node *)InsertTree(root,283);
    root=(Node *)InsertTree(root,74);
    root=(Node *)InsertTree(root,145); 
    root=(Node *)InsertTree(root,186);
    root=(Node *)InsertTree(root,377);
    root=(Node *)InsertTree(root,928);
    root=(Node *)InsertTree(root,659);
    root=(Node *)InsertTree(root,671);
    root=(Node *)InsertTree(root,721); 
    root=(Node *)InsertTree(root,811);
    root=(Node *)InsertTree(root,991);
    root=(Node *)InsertTree(root,831);
    root=(Node *)InsertTree(root,271);
    root=(Node *)InsertTree(root,571);
    root=(Node *)InsertTree(root,351); 
    root=(Node *)InsertTree(root,481);
    root=(Node *)InsertTree(root,91);
    root=(Node *)InsertTree(root,51);
    root=(Node *)InsertTree(root,21);
    root=(Node *)InsertTree(root,718);
    root=(Node *)InsertTree(root,214); 
    root=(Node *)InsertTree(root,516);
    root=(Node *)InsertTree(root,110);
    root=(Node *)InsertTree(root,31);
    root=(Node *)InsertTree(root,81);
    root=(Node *)InsertTree(root,171);
    root=(Node *)InsertTree(root,213); 
    root=(Node *)InsertTree(root,219);
    root=(Node *)InsertTree(root,211);
    root=(Node *)InsertTree(root,717);
    root=(Node *)InsertTree(root,616);
    root=(Node *)InsertTree(root,818);
    root=(Node *)InsertTree(root,515); 
    root=(Node *)InsertTree(root,414);
    root=(Node *)InsertTree(root,313);
    root=(Node *)InsertTree(root,212);
    root=(Node *)InsertTree(root,111);
    root=(Node *)InsertTree(root,101);
    root=(Node *)InsertTree(root,817); 
    root=(Node *)InsertTree(root,910);
    root=(Node *)InsertTree(root,719);
    root=(Node *)InsertTree(root,114);
    root=(Node *)InsertTree(root,719);
    root=(Node *)InsertTree(root,513);
    root=(Node *)InsertTree(root,218); 
    root=(Node *)InsertTree(root,163);
 println_Postfix(root);
 CON_ROT_Optimized(root,219);
 println_Postfix(root);
}

int main()
{  

 
  initializeTree();  
  println_Postfix(root);  
  instruction();
  
/*
    clock_t start = clock();  
     timer();
    printf("Time elapsed: %f\n", ((double)clock() - (double)start)); 
  */
  
  system("PAUSE"); 
  return 0;
}