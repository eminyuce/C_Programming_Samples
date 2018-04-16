#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 3 
//Each node has got following values.Two Pointer and four int type values.
struct node_{
       
  struct node_ *leftChild; 
  struct node_ *rightChild;
  int number;
  int alfa;
  int t;
  int wpl;
  
};
//Our stack to hold address of three last nodes
struct stack_{
int size; 
int top;
int * value;
};

 

typedef struct node_ Node;
typedef struct stack_ stack;

int isOverFlow();
void instruction();
void left_Rotation(Node **root,Node **parent,Node **access);
void right_Rotation(Node **root,Node **parent,Node **access);
int calculate_T(Node *parentNode);
void pathFound(Node *path,Node **parent,Node **root,int); 
int calculate_Wpl(Node *);
Node * entry_Con();
Node * CON_ROT_Naive(Node *record,int number);
void push(Node *path);
int pop();
int criterion(Node *parent,int num);
void initStack();
int isOverFlow(); 
void println_Infix(Node *node);
void println_Prefix(Node *node);
void println_Postfix(Node *node);
void addNode();
void rotation();
void rotation(Node **tempRoot,Node **tempParent,Node **tempRecord);


stack *theStack=NULL;
Node * root=NULL;
Node *holder=NULL;
int a;

 

//Insert a node to the tree but we don't put same numbers in the tree
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
//Infix Printer
void println_Infix(Node *node) 
{
     if(node!=NULL)
     {
     println_Infix(node->leftChild);
     printf("number --> %d   alfa:  %d  t:  %d  wpl:  %d \n",node->number,node->alfa,node->t,node->wpl);
     println_Infix(node->rightChild); 
     
     }
     
}
//Prefix Printer
void println_Prefix(Node *node)
{
     if(node!=NULL)
     {
     println_Prefix(node->leftChild);     
     println_Prefix(node->rightChild);
     printf("number --> %d   alfa:  %d  t:  %d  wpl:  %d \n",node->number,node->alfa,node->t,node->wpl);
     }
     
}
//Postfix Printer
void println_Postfix(Node *node)
{
     if(node!=NULL) 
     {
 
 printf("number --> %d    alfa:  %d  t:  %d  wpl:  %d \n",node->number,node->alfa,node->t,node->wpl);
    println_Postfix(node->leftChild);     
    println_Postfix(node->rightChild);

     
     }
     
}
//We have got prepared tree first and we initalize it.
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
//Choice of println.The User can make a choice to print tree on console(Infix,Prefix,Postfix) 
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
//return Result
int result(Node * node){

  return node->number;
 

}
//Entry of CON_ROT program, Choose one of them
void instruction()
{
 int chose;
 
 printf("\nWhat do you want to do?\n1:Addition a node to the tree\n2:Conditional Rotation\n3:Print Tree\n4:Exit"); 
    printf("\nEntry :");
 scanf("%d",&chose);
    switch(chose){ 
        case 1:addNode(); break;
        case 2:printf("Result   %d\n", result(entry_Con())); break;        
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
//We want a search key from the user..
Node * entry_Con()
{
    
printf("Enter a search key in the tree :");     
scanf("%d",&a);
initStack();
  return CON_ROT_Naive(root,a);  
     
    
    
}

Node * CON_ROT_Naive(Node *record,int number)
{
     int wplParent=0,wplChild=0;
     Node *tempRoot=NULL,*tempParent=NULL,*tempRecord=NULL;
     
     //Increment T value for each node in the tree      
     record->t++;
     //Push the last three address of the path to use them rotations
    push(record);
 //ıf search key exists then increment alfa value.
     if(record->number==number)
     {
  
     record->alfa++;
     /*Then take three address from stack because we will use them rotation.Last push is address of our search key.
     After that address is address of parent of search key.And the last one is address of grandParent of search key. 
     In this way we take them our circle stack.
     */
      tempRecord=(Node *)pop();           
      tempParent=(Node *)pop();      
      tempRoot=(Node *)pop();
     

      //Criterion funciton calculate that WPL of parent of search key - WPL of search key after rotation. 
  if(criterion(tempParent,number))
  {
   //If tempRoot is NULL then we must rotate between root and it's children.If not then make a normal rotation
   if(tempRoot==NULL)
   {
   rotation(&tempRoot,&tempParent,&tempRecord);
   root=tempRoot;
   }
   else
   {
   rotation(&tempRoot,&tempParent,&tempRecord); 
   
   }
         //Calculate WPL  of parent of search key, WPL of search key,
         //T value of parent of search key,T value of search key
   tempParent->wpl=calculate_Wpl(tempParent);
   tempRecord->wpl=calculate_Wpl(tempRecord); 
   tempParent->t=calculate_T(tempParent);
   tempRecord->t=calculate_T(tempRecord);

         
  }
  
    
   }//EndIf (tempRecord->number==number)
     
     else
     { //Search the subtrees to find our search key
         if(number<record->number)
   {  
            if(record->leftChild!=NULL){   
          CON_ROT_Naive(record->leftChild,number);
          }
          else{
               //If program couldn't found then make a explanation
               printf("\nThe search key %d is not found in the tree \n\n",a); 
               
               }
         }
   else
   {
             if(record->rightChild!=NULL){
          CON_ROT_Naive(record->rightChild,number);
          }
           else{
              printf("\nThe search key %d is not found in the tree \n\n",a); 
               
               }    
         } 
  //Recalculate Kj    
     record->wpl=calculate_Wpl(record);
     }
     
     //return the node;
      return record;
          
     }//End Methods 
//End Algorithm CON_ROT_Naive
////Criterion funciton calculate that WPL of parent of search key - WPL of search key after rotation according to the parent's children
int criterion(Node *parent,int num)
{
  if(parent==NULL)
   return 0;
  else if(parent->rightChild!=NULL)
  {
   if(parent->rightChild->number==num)
                                      
      return  parent->rightChild->alfa+calculate_T(parent->rightChild->leftChild)-parent->alfa-calculate_T(parent->leftChild); 
        
  }if(parent->leftChild!=NULL)
  {
   if(parent->leftChild->number==num)
      
    return parent->leftChild->alfa+calculate_T(parent->leftChild->leftChild)-parent->alfa-calculate_T(parent->rightChild); 
 }
    
}

//Rotation function,We decide which rotation will perform in the tree, left rotation or right rotation.....
void rotation(Node **tempRoot,Node **tempParent,Node **tempRecord)
{
 
  /*We evaulate tempRoot and tempParent to perform rotation.. 
    If tempRoot is null then we rotate root's children to be root but 
    if not then make a rotation according to the tempParent->children    
 */
  if(*tempRoot==NULL)
  {
  

  if((*tempParent)->rightChild->number==(*tempRecord)->number)
  right_Rotation(tempRoot,tempParent,tempRecord);
  else if((*tempParent)->leftChild->number==(*tempRecord)->number)
  left_Rotation(tempRoot,tempParent,tempRecord); 
  
  
  }
  else{
             //Decide which rotations will be performed
   if((*tempParent)->leftChild!=NULL)
   {
    if((*tempParent)->leftChild->number==(*tempRecord)->number){         
  left_Rotation(tempRoot,tempParent,tempRecord);
   
    }

   }
     if((*tempParent)->rightChild!=NULL)
   {
    if((*tempParent)->rightChild->number==(*tempRecord)->number){
   right_Rotation(tempRoot,tempParent,tempRecord);
  
   }
   }
  }
     
}
/*We define a stack to hold address of node in the tree.Our stack is circle stack. 
It can take only three address of path on the search key.Adress of search key,address of parent of search key and address of grandparent 
Initialize the Stack
*/
void initStack(){

 
 theStack=(stack *)calloc(1,sizeof(stack));
 theStack->value=(int *)calloc(3,sizeof(int));
 
}
//Make a circle if is OverFlow.
int isOverFlow()
{

 if(theStack->top==-1)
  return 1;
 else
  return 0;

 
}
//Push the address of nodes
void push(Node *path)
{   
 
 theStack->top%=3;
 
 theStack->value[theStack->top++]=(int)path;        
 

}
//Take address of node from stack.
int pop()
{
 int k;

 
 k=--theStack->top;
 if(isOverFlow()){
 theStack->top=2;
 k=theStack->top;
 }

  return theStack->value[k];

    
}
//Calculate WPL.
int calculate_Wpl(Node *parentNode)
{
    int t=0,wpl_left=0,wpl_right=0,t_left=0,t_right=0;
    
    if(parentNode!=NULL)
    {
    t_left=calculate_T(parentNode->leftChild); 
    t_right=calculate_T(parentNode->rightChild);
    t=parentNode->alfa+t_left+t_right;   
    
    }
    if(parentNode->leftChild!=NULL)
    wpl_left=parentNode->leftChild->wpl;
    if(parentNode->rightChild!=NULL) 
    wpl_right=parentNode->rightChild->wpl;
    
    
    return t+wpl_left+wpl_right;
    
}
//Calculate T value.
int calculate_T(Node *parentNode)
{
    int alfa=0,left=0,right=0;
    
    if(parentNode!=NULL)
    alfa=parentNode->alfa;
    else
  return 0;
    if(parentNode->rightChild!=NULL)
       right=parentNode->rightChild->t;
 else
  right=0;
                                    
    if(parentNode->leftChild!=NULL)
    left=parentNode->leftChild->t;                         
    else
  left=0;
    
    return alfa+left+right;
    
}
//Make a left Rotation if it is necessary.. 
void left_Rotation(Node **root,Node **parent,Node **access)
{
     
   
     printf("\nLeft Rotation\n\n");
  if(*root==NULL)
     {
           *root=(*parent)->leftChild;     
          (*parent)->leftChild=(*access)->rightChild;    
    (*root)->rightChild=*parent;        
                          
     }
  else if((*root)->leftChild==*parent)
     {
 holder=*parent;
     (*root)->leftChild=*access;
     (*parent)->leftChild=(*access)->rightChild; 
     (*access)->rightChild=holder;
                              
                                 
     }
     else if((*root)->rightChild==*parent)
     {
   holder=*parent;
     (*root)->rightChild=*access; 
     (*parent)->leftChild=(*access)->rightChild;
     (*access)->rightChild=holder;       
          
          
     }
     
     
     
}
//Make a right rotation if it is necessary 
void right_Rotation(Node **root,Node **parent,Node **access)
{
     
     printf("\nRight Rotation\n\n");
  if(*root==NULL)
     {
     *root=(*parent)->rightChild;
          (*parent)->rightChild=(*access)->leftChild;   
          (*root)->leftChild=*parent;
            
                          
     } 
     else if((*root)->leftChild==*parent)
     {
   holder=*parent;
     (*root)->leftChild=*access;
     (*parent)->rightChild=(*access)->leftChild;
     (*access)->leftChild=holder;                     
                                 
     }
     else if((*root)->rightChild==*parent)
     { 
   holder=*parent;
     (*root)->rightChild=*access;
     (*parent)->rightChild=(*access)->leftChild;
     (*access)->leftChild=holder;     
     
     }
  
 
}
/*That funciton is calculating  time with a big tree and a search key in the tree. 
İnitalize a big tree,print it on console,make a search and print it, again..We start time in the main funciton and end of that function
we take difference of between start and end.We calculate it in the main function.. 
*/
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
    
    
    
    
    //printf("------Postfix-------\n");
 println_Postfix(root);
 CON_ROT_Naive(root,219);
 //printf("\n\n------Postfix-------\n\n"); 
 println_Postfix(root);
}

int main()
{
    
 initializeTree();
 printf("------Postfix-------\n");
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

 