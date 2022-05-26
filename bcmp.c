#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//TASK 3: 
typedef struct diff {
    long offset; /* offset of the difference in file starting from zero*/
    unsigned char orig_value;     /* value of the byte in ORIG */
    unsigned char new_value;     /* value of the byte in NEW */
} diff;
typedef struct node node;
 
struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};
void list_print(node *diff_list,FILE* output){
     /* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE. 
Each item followed by a newline character. */
	if(diff_list != NULL){
		struct node* tempPTR=diff_list;
			while(diff_list != NULL){
				/*
				curr_1 =(unsigned char*)malloc (sizeof(unsigned char*));
				*curr_1=diff_list->diff_data->orig_value;
				curr_2 =(unsigned char*)malloc (sizeof(unsigned char*));
				*curr_2=diff_list->diff_data->new_value;
				offset=(long*) malloc(sizeof(long*));
				*offset=diff_list->diff_data->offset;
				free(curr_1);
				free(curr_2);
				free(offset);	
				*/

				fprintf(output,"byte %ld %02x %02x ", diff_list->diff_data->offset,diff_list->diff_data->orig_value,diff_list->diff_data->new_value);
				fprintf(output,"\n");
				diff_list=diff_list->next;
					
			}
			diff_list=tempPTR;
	}
 
}
 
node* list_append(node* diff_list, diff* data){ 
     /* Add a new node with the given data to the list,
        and return a pointer to the list (i.e., the first node in the list).
        If the list is null - create a new entry and return a pointer to the entry.*/
		
	if(diff_list==NULL){
		 struct node* next= malloc(sizeof(node));
		 struct diff *data1 =malloc(sizeof(diff));
		 data1->new_value=data->new_value;
		 data1->orig_value=data->orig_value;
		 data1->offset=data->offset;
		 next->diff_data=data1;
		 next->next=NULL;
		 return next;
	}
	else
	{
		struct node* ptr=diff_list;
		while(ptr->next!=NULL){ 
			ptr=ptr->next;
		}
		 struct node* nextN= malloc(sizeof(node));
		 struct diff *data1 =malloc(sizeof(diff));
		 data1->new_value=data->new_value;
		 data1->orig_value=data->orig_value;
		 data1->offset=data->offset;
		 nextN->diff_data=data1;
		 nextN->next=NULL;
		 ptr->next=nextN;
		
	}
	return diff_list;
	
}

void list_free(node *diff_list){ /* Free the memory allocated by and for the list. */
	struct node* iter;
	while(diff_list!=NULL){ 
		iter=diff_list;
		diff_list=diff_list->next;
		free(iter->diff_data);
		free(iter);
	}
}
int main(int argc,char**argv){
FILE *output=stdout;
FILE *fileInput_ORIG; 
FILE *fileInput_NEW; 
unsigned char buffer_orig[100];
unsigned char buffer_new[100];
int i,temp, length_orig, length_new, n, oversize_flag=0;
int output_f=0;
int indic_tf=0;
int counter_t=0;
int to_restore=0;
struct node* diff_list=NULL;

for(i=0;i<argc;i++){
		if(strcmp(argv[i],"-o")==0){
	output=fopen(argv[++i],"w");
	output_f=1;
 	}
	 if(strcmp(argv[i],"-t")==0)	
	  indic_tf=1;
	if(strcmp(argv[i],"-k")==0){	
	  oversize_flag=4;
	  temp = atoi(argv[++i]);
    }
	if(strcmp(argv[i],"-r")==0){	
	  to_restore = atoi(argv[++i]);
    }
}
 if(output_f==1){
	 fileInput_ORIG= fopen(argv[argc-4],"r");
	 fileInput_NEW =fopen(argv[argc-3],"r") ;
 }
else{
	fileInput_ORIG= fopen(argv[argc-2],"r");
	fileInput_NEW =fopen(argv[argc-1],"r") ;
}
length_orig = fread(buffer_orig,sizeof(unsigned char),100,fileInput_ORIG);
length_new = fread(buffer_new,sizeof(unsigned char),100,fileInput_NEW);


if(length_orig <= length_new)
	n=length_orig;
else
	n=length_new;

if(oversize_flag==4){ 
	if(temp<n)
		n=temp;
}
	int j;
	long pos=0;
	for(j=0;j<n;j++){
		if(buffer_orig[j]!=buffer_new[j]){
		 struct diff nextD={pos,buffer_orig[j],buffer_new[j]};
		 diff_list = list_append(diff_list,&nextD);
		 counter_t++;
		}
		pos += 1;
	}


list_print(diff_list,output);
printf("\n");
if(indic_tf==1)
	fprintf(output,"total changes : %d \n",counter_t);

if(to_restore!=0 && (to_restore <= n)){
	    unsigned char *buffer2=malloc(length_new*sizeof(char));
		fread(buffer2,length_new,sizeof(char),fileInput_NEW);
		struct node* iterator=diff_list;
		long tempPos;
		for(int q=0;i<to_restore;q++){
		tempPos = iterator->diff_data->offset;
		buffer2[(int)tempPos]= iterator->diff_data->orig_value;
		iterator =iterator->next;
		}
		FILE* restoreInput;
		if(output_f==1){
		restoreInput=fopen(argv[argc-3],"w");
		}
	else{
		restoreInput=fopen(argv[argc-1],"w");
	}
		fprintf(restoreInput,"%s ",buffer2);
		free(iterator);
}
fclose(fileInput_ORIG);
fclose(fileInput_NEW);
list_free(diff_list);
return 0;  
}