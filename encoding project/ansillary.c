/*
 * ansillary.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dell
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "encoder.h"

#define MIN_OCCURENCES_OF_WORD 10

void iterate_test(struct tnode* x)
{
	struct tnode* something;
	something = x;
	if(!something)
		return;
	else
	{
		iterate_test(something->left);
		iterate_test(something->right);
		printf("here we go\n");
	}
}

void print_hash_table()
{
	char filename[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\hash_table.txt";
	FILE *fp;
	struct tnode * sl;
	struct tnode* next;
	char Holmes[] = "Holmes";
	struct word * word = NULL, *temp;
	int i, count = 0;
	fp = fopen(filename,"w");
	qhead = NULL;

	for(i = 0; i < HASH_BUCKETS; i++)
		{
			word = hash_table[i];
			while(word != NULL)
			{
				sl = slalloc(word);
				//printf("word= %s\nsymbol = %c\ncount = %dfreq = %f\nnext = %p\nparent = %p\nleft = %p\nright=%p\n",sl->word,sl->symbol,sl->word_count,sl->freq,sl->next,sl->parent,sl->left,sl->right);
				pq_insert(sl);
				//free(sl);
				//temp = word->next;
				//clear_word(word,i);
				//printf("word->word = %s sl->word = %s\n",word->word,sl->word);
				word = word->next;
				//fprintf(fp,"%s %d\n",word->word, word->count);


				count++;
			}
		}

	//deconstruct_tree(root);
	next = qhead;
	while(next)
	{
		if(next->word)
			printf("word = %s\n",next->word);
		else
			printf("char = %c\n",next->symbol);
		next = next->next;
	}

	//generate_tree();
	//deconstruct_tree(root);
	fclose(fp);
	printf("true count = %d\n",count);
}
//
//useful debug functions
//
//removes words that only appear once in hash text
void clean_hash_table()
{
	struct word * word = NULL, * temp = NULL;
	int i, count_cleared = 0, count_total = 0;
	for(i = 0; i < HASH_BUCKETS; i++)
		{
			word = hash_table[i];
			while(word != NULL)
			{
				temp = word->next;
				if (word->count < MIN_OCCURENCES_OF_WORD)
				{
					//printf("%s\n",word->word);
					clear_word(word,i);
					count_cleared++;

				}
				word = temp;
				count_total++;
			}
		}
	printf("total words = %d  words cleared = %d\n",count_total,count_cleared);
}
//dumps code[] array and hash table to a key file
//which can be read in.
void dump_code_words(char * keyfile)
{
	FILE * kf = fopen(keyfile,"w");
	int i=0;
	struct word * temp;
	for(i=0;i<MAX_SYMBOLS;i++)
	{
		if(code[i][0]) /*non empty*/
			fprintf(kf,"%c %s\n",i,code[i]);
	}
	for (i = 0; i<HASH_BUCKETS; i++)
	{
		temp = hash_table[i];
		while(temp)
		{
			fprintf(kf,"%s %s\n",temp->word,temp->code);
			temp = temp->next;
		}
	}
	fclose(kf);
}

//Priority Queue Insert Function//
void pq_insert(struct tnode* p)
{
   struct tnode* curr=NULL;
   struct tnode* prev=NULL;
   //printf("inserting:%c,%f\n",p->symbol,p->freq);
   if(qhead==NULL) /*qhead is null*/
   {
	   /*TODO: write code to insert when queue is empty*/
	   qhead = p;
	   return;
   }
   /*TODO: write code to find correct position to insert*/
   curr = qhead;
   //printf("curr->freq = %f\np->freq = %f\n",curr->freq,p->freq);
   while(curr && ((curr->freq) < (p->freq)))
   {
	   prev = curr;
	   curr = curr->next;
   }
   if(curr==qhead)
   {
   	 	/*TODO: write code to insert before the current start*/
	   p->next = qhead;
	   qhead = p;
   }
   else /*insert between prev and next*/
   {
	 	/*TODO: write code to insert in between*/
	   prev->next = p;
	   p->next = curr;
   }
}

struct tnode* pq_pop()
{
    struct tnode* p=NULL;
    /*TODO: write code to remove front of the queue*/
    p = qhead;
    if (!qhead)
    	return p;
    qhead = qhead->next;
    p->next = NULL;
    return p;
}

void decoder_two_letters()
{
	FILE * fin;
	FILE * fout;
	char readFile[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\encoding_two_letters.txt";
	char writeFile[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\decodebook_two_letters.txt";
	char c;
	struct tnode* curr=root;
	fin = fopen(readFile, "r");
	fout = fopen(writeFile, "w");
	while((c=getc(fin))!=EOF)
	{
		/*TODO:
			traverse the tree
			print the symbols only if you encounter a leaf node
		*/
		if (c == '1')
		{
			if ((curr->right)->word)
			{
				fprintf(fout,"%s",(curr->right)->word);
				curr = root;
			}
			else if (((curr->right)->isleaf) == 1)
			{
				fputc((curr->right)->symbol,fout);
				curr = root;
			}
			else
				curr = curr->right;
		}
		else if (c == '0')
		{
			if ((curr->left)->word)
			{
				fprintf(fout,"%s",(curr->left)->word);
				curr = root;
			}
			else if (((curr->left)->isleaf) == 1)
			{
				fputc((curr->left)->symbol,fout);
				curr = root;
			}
			else
				curr = curr->left;
		}
	}
	fclose(fin);
	fclose(fout);
}
/*void func_save()
{
    for(i=0;i<num_unique_char-1;i++)
    {
        lc=pq_pop();
        rc=pq_pop();
        create parent
        p=talloc(0,lc->freq+rc->freq);
        set parent link
        lc->parent=rc->parent=p;
        set child link
        p->right=rc; p->left=lc;
		make it non-leaf
		p->isleaf=0;
        add the new node to the queue
        pq_insert(p);
    }
    get root
    root=pq_pop();
	build code
    generate_code(root,0);
}*/

void read_key_file(char * keyfile)
{
	FILE * fp = fopen(keyfile,"r");
	char	symbol;
	char	strcode[MAX_LEN], line[MAX_LEN * 2], word[MAX_LEN];
	int		len;
	struct	tnode* curr=NULL;
	int word_flag = 0, symbol_flag = 0;
	root = talloc(0,0);
	while(fgets(line,(MAX_LEN * 2),fp))
	{
		if (line[0] == '\n')
		{
			symbol = line[0];
			fgets(line,MAX_LEN+3,fp);
			strcpy(strcode,&line[1]);
			len = strlen(strcode);
			strcode[len-1] = '\0';
			len--;
//			printf("symbol: %c strcode: %s\n",symbol, strcode);
			symbol_flag = 1;
			word_flag = 0;
		}
		else if(line[1] == ' ')
		{
			symbol = line[0];
			strcpy(strcode,&line[2]);
			len = strlen(strcode);
			strcode[len-1] = '\0';
			len--;
//			printf("symbol: %c strcode: %s\n",symbol, strcode);
			symbol_flag = 1;
			word_flag = 0;
		}
		else
		{
			strcpy(word,strtok(line, " "));
			strcpy(strcode, strtok(NULL," "));
			len = strlen(strcode);
			strcode[len-1] = '\0';
			len--;
//			printf("word: %s strcode: %s len = %d\n",word, strcode,len);
			symbol_flag = 0;
			word_flag = 1;
		}

		curr=root;
		for(int i=0; i<len; i++)
		{
			if (strcode[i] == '1')
			{
				if (!(curr->right))
				{
					curr->isleaf = 0;
					curr->right = talloc(0,0);
					(curr->right)->parent = curr;
					curr = curr->right;
				}
				else
					curr = curr->right;
			}
			else if (strcode[i] == '0')
			{
				if(!(curr->left))
				{
					curr->isleaf = 0;
					curr->left = talloc(0,0);
					(curr->left)->parent = curr;
					curr = curr->left;
				}
				else
					curr = curr->left;
			}
			else
			{
				printf("error encoding strcode.\n");
				return;
			}
		}
		//assign code

		if (symbol_flag)
		{
//			curr = talloc(0,0);
			curr->isleaf=1;
			curr->symbol=symbol;
//			printf("inserted symbol:%c\n",symbol);
		}
		else if (word_flag)
		{
//			curr = talloc(0,0);
			curr->word = (char * )malloc(sizeof(word));
			strcpy(curr->word,word);
//			printf("inserted word:%s\n",curr->word);
		}
		else
		{
			printf("error occured with symbol/word flag\n");
			return;
		}
		//fflush(stdout);
	}
	fclose(fp);
}

void freetree(struct tnode* r)
{
	if(r==NULL)
		return;
	freetree(r->right);
	freetree(r->left);
	free(r->word);
	if (r->source)
	{
		free(r->source->code);
		free(r->source);
	}
	free(r);
	r = NULL;
}
