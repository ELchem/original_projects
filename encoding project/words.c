/*
 * words.c
 *
 *  Created on: Apr 16, 2020
 *      Author: Dell
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "encoder.h"

#define MAX_WL 40
#define DEFAULT_BUFFER_SIZE 50
#define HASH_BUCKETS 101
#define KEY 21
#define BUFFER_SIZE 3


struct word * hash_table[HASH_BUCKETS];
//diagnostic for hash table building

//allocates a new word
struct word * walloc(char * word, int sum)
{
	struct word * new_word;
	new_word = (struct word *)malloc(sizeof(struct word));
	new_word->next = NULL;
	new_word->word = word;
	new_word->count = 1;
	new_word->sum = sum;
	new_word->code = NULL;
	new_word->code_len_letters = 0;
	return new_word;
}

void hash_distribution()
{
	int bucket_count[101];
	for(int h = 0; h<HASH_BUCKETS; h++)
	{
		bucket_count[h] = 0;
	}
	struct word* word;
	for (int i = 0; i < HASH_BUCKETS; i++)
	{
		word = hash_table[i];
		while(word)
		{
			bucket_count[i]++;
			word = word->next;
		}
	}
//	for (int j = 0; j<HASH_BUCKETS;j++)
//	{
//	printf("%d\n",bucket_count[j]);
//	}
}
//same as strcmp more or less
int compare_words(char * a, const char * b)
{
	int i = 0;
	while(a[i])
	{
		if(a[i] != b[i])
			break;
		i++;
	}
	return (a[i]-b[i]);
}

//Given a character string (word)
//determine_index returns the hash
//bucket that a word will be found in
//or be added to. Pointer to a intiger
//variable is optionally passed and will store
//the sum value calculated in this function.
int determine_index(char * str, int * psum)
{
	int sum = 0, i = 0;
	while(str[i] != '\0')
	{
		sum = sum + str[i];
		i++;
	}
	if (psum)
		*psum = sum;
	if (sum/KEY > 100 || sum/KEY < 0)
	{
		printf("Error: index out of bounds (%d,%s)\n",sum/KEY,str);
		return 0;
	}
	return (sum/KEY);
}
//returns pointer to word structure if it exists
//given the words associated char string and index
//of its hash bucket.
struct word * find_word (char * str, int index)
{
	struct word * temp = NULL;
	temp = hash_table[index];
	while (temp)
	{
		if(!strcmp(temp->word,str))
			return temp;
		temp = temp->next;
	}
	return NULL;
}
//inserts a new word into the linked lists
//in the hash table, ordered by sum.
void insert_word (struct word * s_word, int index)
{
	struct word * temp;
	struct word * prev;
	if (hash_table[index])
	{
		temp = hash_table[index];
		if (temp->sum >= s_word->sum)
		{
			s_word->next = temp;
			hash_table[index] = s_word;
			return;
		}
		while (temp &&temp->sum < s_word->sum)
		{
			prev = temp;
			temp = temp->next;
		}
		prev->next = s_word;
		s_word->next = temp;
	}
	else
	{
		hash_table[index] = s_word;
		s_word->next = NULL;
	}
}
//Adds a word to the hash table or
//increments its count if it exists
void add_word(char * a)
{
	char * pass_str;
	struct word * s_word;
	int sum, index = determine_index(a,&sum);
	if (s_word = find_word(a, index))
	{
		(s_word->count)++;
	}
	else
	{
		pass_str = strdup(a);
		s_word = walloc(pass_str,sum);
		insert_word(s_word, index);
	}
	return;
}
//frees word structures when not needed,
//aswell as code and word char arrays
void clear_word(struct word * pword, int index)
{
	//always used to clear root word for particular hash index
	//set the root for the particular hash index to the next word
	//from the word that you are clearing
	struct word *temp = hash_table[index], *prev = NULL;
	if (!temp)
	{
		printf("Error: no words at that index\n");
		return;
	}
	if (temp == pword)
	{
		hash_table[index] = temp->next;

		free(pword->code);
		free(pword->word);
		free(pword);
		return;
	}
	while(temp)
	{
		if (temp == pword)
			break;
		prev = temp;
		temp = temp->next;
	}
	if (!temp)
	{
		printf("Error: no words at that index\n");
		return;
	}
	prev->next = temp->next;
	free(pword->code);
	free(pword->word);
	free(pword);
}
//
//
//testing strdup()
//
//
struct test_dup
{
	char * str;
	struct test_dup * next;
};
struct test_dup* test_head;

struct test_dup* test_alloc(char * string)
{
	struct test_dup * return_value;
	return_value = (struct test_dup *)malloc(sizeof(struct test_dup));
	return_value->str = string;
	return_value->next = NULL;
	return return_value;
}
void test_insert (struct test_dup * new_head)
{
	if (test_head == NULL)
		test_head = new_head;
	else
	{
		new_head->next = test_head;
		test_head = new_head;
	}
}
void test_strdup(char * a)
{
	char * string;
	struct test_dup * new_link=NULL;
	string = strdup(a);
	new_link = test_alloc(string);
	test_insert(new_link);
}
//Initializes hash table buckets to NULL.
//Reads words from text file into new_word char array
//stopping whenever a non letter is read.
//The char array is then sent to add_word function
//which creates a structure around that word and
//inserts it into hash table.
void read_words(char * filename){
	struct  word* temp;
	FILE *fp;
	FILE *fp2;
	int index;
	int num_runs = 0;
	int holmes_count = 0;
	int count = 0;
	int flag1 = 0;
	char c;
	char new_word[MAX_WL];
	memset(new_word,0,MAX_WL);
	for (int i = 0; i < HASH_BUCKETS; i++)
	{
		hash_table[i] = NULL;
	}
	fp = fopen(filename,"r");
	if(!fp)
	{
		printf("file failed to open\n");
		return;
	}
	while(feof(fp) == 0)
	{
		c = fgetc(fp);
		if (isalpha(c))
		{
			new_word[count] = c;
			count++;
		}
		else
		{
			new_word[count+1] = '\0';
			if (count > 1)
				add_word(new_word);
			count = 0;
			memset(new_word,0,MAX_WL);
		}
		num_runs++;
	}
	fclose(fp);
	hash_distribution();
}
//reads two letter blocks starting at first block into hash table
//hash key is changed to account for smaller sum of characters.
void read_two_letters()
{
	int count = 0,i, buffer_size = BUFFER_SIZE;
	char c,buffer[buffer_size];
	char filename[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\book.txt";
	FILE* fp1;
	fp1 = fopen(filename,"r");
	memset(buffer,0,buffer_size);
	for (i = 0; i<HASH_BUCKETS; i++)
	{
		hash_table[i] = NULL;
	}
	while(feof(fp1) == 0)
	{
		c = fgetc(fp1);
		if (isalnum(c))
		{
			buffer[count] = c;
			count++;
		}
		else
			memset(buffer,0,buffer_size);

		if(count > (buffer_size-1))
		{
			add_word(buffer);
			memset(buffer,0,buffer_size);
			count = 0;
		}
	}
	fclose(fp1);
	//hash_distribution();
}
void read_all_consecutive()
{
	int count = 0,i, buffer_size = BUFFER_SIZE;
	char c,buffer[buffer_size];
	char filename[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\book.txt";
	FILE* fp1;
	fp1 = fopen(filename,"r");
	memset(buffer,0,buffer_size);
	for (i = 0; i<HASH_BUCKETS; i++)
	{
		hash_table[i] = NULL;
	}
	while(feof(fp1) == 0)
	{
		c = fgetc(fp1);
	}
}
//writes words to file, uses fgetc() to fill char array and then
//searches hash table to find word. If found, word->code is written to
//destination file. If not found, word is written in letter by letter.
//essentially opposite process of read_words()
void write_words(char * file, char * dest){
	struct word * temp;
	char c;
	char word[MAX_WL];
	int i, count = 0, chars_read = 0,trad_count = 0, words_count = 0;
	FILE *fin;
	FILE *fout;
	fin = fopen(file,"r");
	fout = fopen(dest,"w");
	memset(word,0,MAX_WL);
	while(feof(fin) == 0)
	{
		c = fgetc(fin);
		if (isalpha(c))
		{
			word[count] = c;
			count++;
			word[count] = '\0';
		}
		else if ((temp = find_word(word,determine_index(word,0))) != NULL)
		{
			//printf("word added = %s\n",temp->word);
			fprintf(fout,"%s%s",temp->code,code[c]);
			count = 0;
			words_count++;
			memset(word,0,MAX_WL);
		}
		else
		{
			//printf("word not added = %s\n",word);
			//fseek(fin,-count,SEEK_CUR);
			for (i = 0; i < count; i++)
			{
				fprintf(fout,"%s",code[word[i]]);
			}
			fprintf(fout,"%s",code[c]);
			count = 0;
			trad_count++;
			memset(word,0,MAX_WL);
		}
		chars_read++;
	}
	fclose(fin);
	fclose(fout);
//	printf("words = %d\tas chars = %d\n",words_count,trad_count);
}
//Similair to write_words(), however, instead of needing to
//write words, it writes two char blocks as binary code.
//If the two char block is not found in the hash table, it
//looks to code[] array.
void write_two_letters()
{
	struct word* temp;
	int i, buffer_size = BUFFER_SIZE,count1 = 0, count2 = 0;//count2 for 2letter count1 for 1letter
	char str[buffer_size];
	FILE *fin;
	FILE *fout;
	char filename1[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\book.txt";
	char filename2[] = "C:\\Users\\Dell\\Desktop\\MIT_OCW\\practical_programming_in_c_6087\\LAB_2\\encoding_two_letters.txt";
	fin = fopen(filename1,"r");
	fout = fopen(filename2,"w");
	while(!feof(fin))
	{
		for(int i = 0; i < buffer_size - 1; i++)
			str[i] = fgetc(fin);
		str[buffer_size - 1] = '\0';
		if((temp = find_word(str,determine_index(str,0))))
		{
			if (temp->code)
			{
			fprintf(fout,"%s",temp->code);
			count2++;
			}
		}
		else
		{
			for (i = 0; i<buffer_size;i++)
			{
				fprintf(fout,"%s",code[str[i]]);
				count1++;
			}
		}
	}
//	printf("2letter count = %d\n1lettercount = %d\n",count2,count1);
	fclose(fin);
	fclose(fout);
}
