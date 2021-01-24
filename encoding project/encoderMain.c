/*
 * AndYetHePersisted.c
 *
 *  Created on: Apr 6, 2020
 *      Author: Dell
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoder.h"


/*global variables*/
float TOTAL_CHARS = 0;
char word_code[MAX_LEN]; //temp str for storing word code before its fully written
char code[MAX_SYMBOLS][MAX_LEN];
struct tnode* root=NULL; /*tree of symbols*/
struct tnode* qhead=NULL; /*list of current symbols*/
struct cnode* chead;/*list of code*/
int foo;
int char_counter[ASCII_EXT_CHARS];
int num_unique_char = 0;

struct tnode* talloc(int symbol,float freq)
{
    struct tnode* p=(struct tnode*)malloc(sizeof(struct tnode));
    if(p!=NULL)
    {
        p->left=p->right=p->parent=p->next=NULL;
        p->symbol=symbol;
        p->freq=freq;
		p->isleaf=1;
		p->word = NULL;
		p->word_count = 0;
		p->source = NULL;
    }
    return p;
}

/*
	@function build_code
	@desc     generates the string codes given the tree
	NOTE: makes use of the global variable root
*/
void generate_code(struct tnode* root,int depth, int * total_code)
{
	int symbol;
	int len; /*length of code*/
	struct tnode* temp;
	if(root->word)
	{
		temp = root;
		len = depth;
		/*start backwards*/
		word_code[len]=0;
		/*
			TODO: follow parent pointer to the top
			to generate the code string
		*/
		for (int x = 1; x<=len; x++)
		{
			if((root->parent)->right == root)
			{
				word_code[len-x] = '1';
				*total_code += root->word_count;
				root = root->parent;
				//free(root->right);
			}
			else if((root->parent)->left == root)
			{
				word_code[len-x] = '0';
				*total_code += root->word_count;
				root = root->parent;
				//free(root->left);
			}
			else
			{
				printf("generate_code encountered error with pointer comparison.\n");
				break;
			}
		}
		//printf("built code:%c,%s\n",symbol,code[symbol]);
		(temp->source)->code = strdup(word_code);
		return;
	}
	else if(root->isleaf)
	{
		symbol=root->symbol;
		len = depth;
		/*start backwards*/
		code[symbol][len]=0;
		/*
			TODO: follow parent pointer to the top
			to generate the code string
		*/
		for (int x = 1; x<=len; x++)
		{
			if((root->parent)->right == root)
			{
				code[symbol][len-x] = '1';
				*total_code += char_counter[root->symbol];
				root = root->parent;
				//free(root->right);
			}
			else if((root->parent)->left == root)
			{
				code[symbol][len-x] = '0';
				*total_code += char_counter[root->symbol];
				root = root->parent;
				//free(root->left);
			}
			else
			{
				printf("generate_code encountered error with pointer comparison.\n");
				break;
			}
		}
		//printf("built code:%c,%s\n",symbol,code[symbol]);
		return;
	}
	else
	{
		generate_code(root->left,depth+1, total_code);
		generate_code(root->right,depth+1, total_code);
		return;
	}

}
void encode(char str,FILE* fout)
{
	fprintf(fout,"%s",code[str]);
}

void dump_code(FILE* fp)
{
	int i=0;
	for(i=0;i<MAX_SYMBOLS;i++)
	{
		if(code[i][0]) /*non empty*/
			fprintf(fp,"%c %s\n",i,code[i]);
	}
}

void decode(FILE* fin,FILE* fout)
{
	char c;
	struct tnode* curr=root;
	while((c=getc(fin))!=EOF)
	{
		/*TODO:
			traverse the tree
			print the symbols only if you encounter a leaf node
		*/
		if (c == '1')
		{
			if (((curr->right)->isleaf) == 1)
			{
				fputc((curr->right)->symbol,fout);
				curr = root;
			}
			else
				curr = curr->right;
		}
		else if (c == '0')
		{
			if (((curr->left)->isleaf) == 1)
			{
				fputc((curr->left)->symbol,fout);
				curr = root;
			}
			else
				curr = curr->left;
		}
	}
}


//
//
//super letters are now tnode
//
//


void deconstruct_tree(struct tnode * rootb)
{
	if (!rootb)
		return;
	//If the current root pointer is not set to NULL (ie it is a word)
	//Recalculate its frequency, add to prior queue.
	else if (rootb->word)
	{
		if(TOTAL_CHARS)
		{
			//if there are characters left in the tree
			//set the frequency of the root (recursively).
			//(freq = (Length of word * Occurence of Word)/(Total Characters in Book)
			root->freq = (rootb->word_count*strlen(rootb->word))/TOTAL_CHARS;
		}
		pq_insert(rootb);
	}
	//If rootb is a non-word character this statement should trigger.
	else if(rootb->isleaf)
	{
		if (rootb->symbol >=ASCII_EXT_CHARS || rootb->symbol < 0)
		{
			printf("error in char counter index\n");
			return;
		}
		else if (TOTAL_CHARS)
		{
		rootb->freq = char_counter[rootb->symbol]/TOTAL_CHARS;
		}
		rootb->left = rootb->right = NULL;
		rootb->next = NULL;
		pq_insert(rootb);

	}
	else if (!(rootb->isleaf))
	{
		deconstruct_tree(rootb->left);
		deconstruct_tree(rootb->right);
		free(rootb);

	}
}
int generate_tree()
{
    struct tnode* p=NULL;
    struct tnode* lc,*rc;
	int i, total_code = 0;
	/*build tree*/
	for(i=0;i<num_unique_char-1;i++)
	{
		lc=pq_pop();
		rc=pq_pop();
		/*create parent*/
		p=talloc(0,lc->freq+rc->freq);
		/*set parent link*/
		lc->parent=rc->parent=p;
		/*set child link*/
		p->right=rc; p->left=lc;
		/*make it non-leaf*/
		p->isleaf=0;
		/*add the new node to the queue*/
		pq_insert(p);
	}
	/*get root*/
	root=pq_pop();
	/*build code*/
	generate_code(root,0,&total_code);
	return total_code;
}
//calculates the frequency of the word relative to other letters
float calc_freq(struct word * word)
{
	float freq = 0;
	unsigned char c;
	int i, len = strlen(word->word);
	for (i = 0; i < len; i++)
	{
		c = word->word[i];
		char_counter[c] -= word->count;
		TOTAL_CHARS -= word->count;

	}
	TOTAL_CHARS += word->count;
	if (TOTAL_CHARS)
		freq = ((word->count)*(len))/TOTAL_CHARS;
	return freq;
}
struct tnode * slalloc(struct word * word)
{
	if (!word)
	{
		printf("word empty\n");
		return NULL;
	}
	struct tnode * tword = NULL;
	tword = talloc(0,calc_freq(word));
	tword->word = word->word;
	tword->source = word;
	return tword;
}
//calc_code_len returns the number of digits of code required to represent the word
//by its alphabetical spelling
int calc_code_len(struct word * word) //calculates length of code generated from using letters//
{
	int str_len = strlen(word->word);
	int code_len = 0;
	for (int i = 0; i < str_len; i++)
	{
		if (code[(word->word)[i]] < 0){
			printf("Signed Char being used to access code[].\n");
			return 0;
		}
		code_len += strlen(code[(word->word)[i]]);

	}
	return code_len*(word->count);
}
//calc_cost returns the number of digits of code that must be added to
//the encoded text as a result of adding a new member to the
//binary tree (all symbols/words bellow the added parent node
//have their code length increased by 1).
//Takes the parent node of the added node as argument.
int calc_cost(struct tnode * root)
{
	if (root->word)
		return root->word_count;
	else if (root->isleaf)
		return char_counter[root->symbol];
	else
		return calc_cost(root->left) + calc_cost(root->right);
}
//finalize_cost takes the difference between the returns of calc_code_len and calc_cost and
//subtracts the number of bits required to store the
//word in the key file and returns this value (assumes 8bits per letter plus 1 space and 1 return)
int finalize_cost(int len1,int len2, struct word* word)
{
	int diff = len1- len2;
	//This formula accounts for the added space of storing the word in a key file
	return (diff - (8*strlen(word->word)) - 16 - (8*strlen(word->code)));
}
//frees node in binary search tree
void delete_node(struct tnode* target_node)
{
	unsigned char c;
	int i,len = strlen(target_node->word);
	for (i = 0;i<len;i++)
	{
		c = target_node->word[i];
		char_counter[c] += target_node->word_count;
		TOTAL_CHARS += target_node->word_count;
	}
	TOTAL_CHARS -= target_node->word_count;
	free(target_node);
}
//func2() reads words from hash table and determines if they
//are efficient enough to use in a key
void func2()
{
	int finalize = 0;
	int added_words = 0;
	int skipped_words = 0;
	int i, len1, len2,count = 0;
	struct word * word = NULL;
	struct word * temp = NULL;
	struct tnode * sl; //superletter
	for(i = 0; i < HASH_BUCKETS; i++)
	{
		word = hash_table[i];
		//needed to prevent linking between hash buckets
		while(word != NULL)
		{
			//len1 only accounts for space saved, not space used by new word code
			len1 = calc_code_len(word);
			sl = slalloc(word);
			num_unique_char++;
			pq_insert(sl);
			//old tree is deconstructed (tnodes moved back into prior queue with new sl)
			//new tree is constructed and new code is generated
			deconstruct_tree(root);
			generate_tree();
			//len2 has to be adjusted for length of word code
			//subtract word count from len2 bc len2 should not consider the code
			//for the new word
			//to be lengthened by addition of itself
			len2 = calc_cost(sl->parent);
			len2 = len2 + (strlen(word->code) * (word->count));
			finalize = finalize_cost(len1,len2,word);
			if(finalize > 0)
			{
				//printf("word added = %s %d finalize = %d\n",word->word,word->count,finalize);
				sl->word_count = word->count;
				added_words++;
				word = word->next;
			}
			else
			{
				if (!word)
				{
					printf("error\n");
					break;
				}
				//printf("word removed = %s %d finalize = %d\n",word->word,word->count,finalize);
				if ((sl->parent)->left == sl)
					(sl->parent)->left = NULL;
				else
					(sl->parent)->right = NULL;
				delete_node(sl);

				num_unique_char--;
				//clear_word() frees struct word memory, leaves string memory intact
				temp = word->next;
				clear_word(word, i);
				word = temp;
				++skipped_words;
				deconstruct_tree(root);
				generate_tree();
			}
			count++;
		}
	}
}
//
//
//sorted list section
//
//
struct word * sorted_list_head;
//determines where in linked list to insert word
void insert_into_list(struct word* word)
{
	struct word * curr, * prev = NULL;
	//code len letters is set equal to the length of code needed to represent
	//the word as a string of encoded letters. This is different from the
	//length of the code that represents the "super letter" of the word.
	int len = word->code_len_letters = calc_code_len(word);
	if (!sorted_list_head)
	{
		word->next = NULL;
		sorted_list_head = word;
		return;
	}
	curr = sorted_list_head;
	while(curr && len < curr->code_len_letters)
	{
		prev = curr;
		curr = curr->next;
	}
	if (!prev)
	{
		sorted_list_head = word;
		word->next = curr;
	}
	else
	{
		prev->next = word;
		word->next = curr;
	}
}
//master function for reading in words from hash table
//and adding them to linked list
void create_list()
{
	struct word* word = NULL, *temp;
	int i;
	sorted_list_head = NULL;
	for(i = 0; i < HASH_BUCKETS; i++)
	{
		word = hash_table[i];
		while(word)
		{
			temp = word->next;
			insert_into_list(word);
			word = temp;
		}
	}
}
void print_list()
{
	struct word* curr = sorted_list_head;
	printf("Ordered list, descending in order of code length taken to generate word using encoded letters.\n");
	while (curr)
	{
		printf("word: %s\tcount: %d\tcode: %s\n",curr->word, curr->count,curr->code);
		curr = curr->next;
	}
}
//Moves word structures from sorted list back
//into hash table for quicker access
void deconstruct_list()
{
	struct word* temp,* word = sorted_list_head;
	int index;
	for (int i = 0; i < HASH_BUCKETS; i++)
	{
		hash_table[i] = NULL;
	}
	while(word)
	{
//		;
		temp = word->next;
		index = determine_index(word->word,0);
		insert_word(word,index);
		word = temp;
	}
}
//Self explanatory, removes word from list and frees associated memory
struct word * remove_from_list(struct word * target_word, struct word * prev_word)
{
	if (!prev_word)
	{
		sorted_list_head = target_word->next;
		return sorted_list_head;
	}
	prev_word->next = target_word->next;
	free(target_word->code);
	free(target_word->word);
	free(target_word);
	return prev_word->next;
}
//This function uses a sorted linked list as its source of words
//operates with the same logic as func2, however, instead of
//reading words based on their sum, it reads them in based on
//a weighting system based off of their length
//and number of occurences.
void func3()
{
	int finalize = 0;
	int added_words = 0;
	int skipped_words = 0;
	int i, len1, len2,count = 0;
	struct word * word = sorted_list_head;
	struct word * prev = NULL;
	struct tnode * sl; //superletter
	//steps along words in word_list, removes them after accessing, frees their
	//memory if they are not going to be used as coded superletters
	//while(word)
	while(word)
	{
		//len1 only accounts for space saved, not space used by new word code
		len1 = calc_code_len(word);
		//create new word node
		sl = slalloc(word);
		//increment this bc words are considered super chars
		num_unique_char++;
		//insert the new word into prior queue
		pq_insert(sl);
		//old tree is deconstructed (tnodes moved back into prior queue with new sl)
		//new tree is constructed and new code is generated
		deconstruct_tree(root);
		generate_tree();
		//len2 has to be adjusted for length of word code
		len2 = calc_cost(sl->parent) - word->count;
		len2 = len2 + (strlen(word->code) * (word->count));
		finalize = finalize_cost(len1,len2,word);
		//printf("finalize = %d\n",finalize);
		if(finalize > 0)
		{
			//printf("word added = %s %d finalize = %d\n",word->word,word->count,finalize);
			sl->word_count = word->count;
			added_words++;
			prev = word;
			word = word->next;
		}
		else
		{
			if (!word)
			{
				printf("error\n");
				break;
			}
			//printf("word removed = %s %d finalize = %d\n",word->word,word->count,finalize);
			if ((sl->parent)->left == sl)
				(sl->parent)->left = NULL;
			else
				(sl->parent)->right = NULL;
			delete_node(sl);
			num_unique_char--;
			//clear_word() frees struct word memory, leaves string memory intact
			word = remove_from_list(word,prev);
			++skipped_words;
			deconstruct_tree(root);
			generate_tree();
		}
	}
}
//
//
//END OF GENERATE CODE SECTION
//START OF ENCODER/DECODER SECTION
//
//
//Encoder in words.c as write_words()
//Decoder implemented here
void decoder_with_words(char * encoded_file, char * decoded_file)
{
	FILE * fin = fopen(encoded_file,"r");
	FILE * fout = fopen(decoded_file, "w");
	char c;
	struct tnode* curr=root;
	while((c=getc(fin))!=EOF)
	{
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

void func_4()
{
	int finalize = 0;
	int added_words = 0;
	int skipped_words = 0;
	int len1, len2,count = 0;
	int current_count, previous_count;
	struct word * word = sorted_list_head;
	struct word * prev = NULL;
	struct tnode * sl; //superletter
	previous_count = generate_tree();
	//steps along words in word_list, removes them after accessing, frees their
	//memory if they are not going to be used as coded superletters
	//while(word)
	while(word)
	{
		//len1 only accounts for space saved, not space used by new word code
		len1 = calc_code_len(word);
		//create new word node
		sl = slalloc(word);
		//increment this bc words are considered super chars
		num_unique_char++;
		//insert the new word into prior queue
		pq_insert(sl);
		//old tree is deconstructed (tnodes moved back into prior queue with new sl)
		//new tree is constructed and new code is generated
		deconstruct_tree(root);
		current_count = generate_tree();
		//len2 has to be adjusted for length of word code
		finalize = finalize_cost(previous_count, current_count, word);

		//printf("finalize = %d\n",finalize);
		if(finalize > 0)
		{
			//printf("word added = %s %d finalize = %d\n",word->word,word->count,finalize);
			sl->word_count = word->count;
			added_words++;
			prev = word;
			word = word->next;
		}
		else
		{
			if (!word)
			{
				printf("error\n");
				break;
			}
			//printf("word removed = %s %d finalize = %d\n",word->word,word->count,finalize);
			if ((sl->parent)->left == sl)
				(sl->parent)->left = NULL;
			else
				(sl->parent)->right = NULL;
			delete_node(sl);
			num_unique_char--;
			//clear_word() frees struct word memory, leaves string memory intact
			word = remove_from_list(word,prev);
			++skipped_words;
			deconstruct_tree(root);
			generate_tree();
		}
	}
}

int main(int argc, char * argv[])
{
	FILE * fp;
	int i;
	char c;
	//Must select a txt file to encode using command line
	if( argc == 2 ) {
	  printf("The file to be encoded is %s\n", argv[1]);
	}
	else if( argc > 2 ) {
	  printf("Too many arguments supplied.\n");
	  return 0;
	}
	else {
	  printf("One argument expected.\n");
	  return 0;
	}
	char book[100];
	strcpy(book, argv[1]);
	char key_file[] = "word_key_file.txt";
	char decoded_book_words[] = "decoded_book_words.txt";
	char encoded_book_words[] = "encoding_words.txt";
	//int char_counter[ASCII_EXT_CHARS];
	float freq[ASCII_EXT_CHARS];
	memset(code,0,sizeof(code));
	for (int i = 0; i < ASCII_EXT_CHARS; i++)
		char_counter[i] = 0;

	fp = fopen(book,"r");
	while((c = fgetc(fp)) != EOF)
	{
		char_counter[c]++;
		TOTAL_CHARS++;
	}
	fclose(fp);

	for (i = 0; i < ASCII_EXT_CHARS; i++)
	{
		freq[i] = (char_counter[i]/TOTAL_CHARS);
	}
	//create queue
    for(i=0;i<ASCII_EXT_CHARS;i++)
    {
    	if (freq[i])
    	{
    		pq_insert(talloc(i,freq[i]));
    		//(talloc(i,freq[i]));
    		num_unique_char++;
    		//printf("%d %f\n",i,freq[i]);
    	}
    	else
    		continue;
    }
    /*build tree*/

    read_words(book); //reads in words from book, stores them in hash table
    clean_hash_table(); //removes words that only occur once
    create_list(); //creates a list ordering words based on occurence and length
    func_4(); //creates a huffman style encoding tree using words and letters, see function definition for more information
    deconstruct_list(); //takes words out of list and puts them into hash table
    write_words(book,encoded_book_words);  //encodes book using words and letters using standard huffman encoding method
    dump_code_words(key_file); //creates key file to rebuild huffman tree later
	freetree(root); //frees all the memory associated with the tree
	read_key_file(key_file); //reads in keys and rebuilds tree
    decoder_with_words(encoded_book_words, decoded_book_words); //decodes encoded file

    printf("done\n");

	return 0;
}
