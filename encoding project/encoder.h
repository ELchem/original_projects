/*
 * encoder.h
 *
 *  Created on: Apr 16, 2020
 *      Author: Dell
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#define HASH_BUCKETS 101
#define MAX_SYMBOLS  256
#define MAX_LEN      50
#define ASCII_EXT_CHARS 256

extern struct word * hash_table[HASH_BUCKETS];
extern char code[MAX_SYMBOLS][MAX_LEN];
extern struct tnode* qhead; /*list of current symbols*/
extern struct tnode* root;

struct word
{
	struct word * next;
	char * word;
	char * code;
	//sum: used by hash table, is the numeric sum of characters
	//count: how many words in document
	//code_len_letters: number of digits needed to encode word currently
	int count, sum, code_len_letters;
};

struct tnode
{
    struct  tnode* left; /*used when in tree*/
    struct  tnode*right; /*used when in tree*/
    struct  tnode*parent;/*used when in tree*/
    struct  tnode* next; /*used when in list*/
    struct  word* source; //word object tnode object derives from
    int     word_count;
    float   freq;
    int     isleaf;
    char    symbol;
    char*   word;
};

struct word * walloc(char * word, int sum);
void read_words(char * filename);
void read_words_fscanf();
void clear_word(struct word* pword, int index);
void write_words(char * file, char * dest);
int determine_index(char * str, int * psum);
void insert_word(struct word* s_word, int index);
void read_two_letters();
void write_two_letters();
void iterate_u_fuck(struct tnode*);
void print_hash_table();
void clean_hash_table();
struct tnode* pq_pop();
void pq_insert(struct tnode*);
void decoder_two_letters();
void freetree(struct tnode* r);
struct tnode* talloc(int symbol,float freq);
void dump_code_words(char * keyfile);
void read_key_file(char * keyfile);
struct tnode * slalloc(struct word * word);
#endif /* ENCODER_H_ */
