#include "http_fetcher.h"
#include "cvector.h"
#include "config.h"
#include "text2token.h"
#include "address.hpp"
#define DEF_BUFF_SIZE 1024*10

/** 
* Function: free_token
* ---------------------------------
* free_token defines the space of functions that can be used as the
* clean-up function for each element as it is deleted from the vector
* or when the entire cvector is destroyed.  The cleanup function is 
* called with a pointer to an element about to be deleted.
*/
void free_token(void *elemAddr) {
	assert(elemAddr);
	//	NGram * n=(NGram*)elemAddr;
	//	printf("%s\n",n->ngram);
	free (((Token*)elemAddr)->term);
}

int main (int argc, char* argv[]) {
	char* text;
	char* tokens = (char*)malloc(DEF_BUFF_SIZE);
	char* t = tokens;
	Token *token;
	int i;
	cvector tokenVector;
	http_setTimeout(2);//seconds 
	int ret=http_fetch(argv[1], &text);
	if (ret == -1) {
		printf("%s\n",http_strerror());
		return 0;
	}
	//printf("%s\n",text);

	VectorNew (&tokenVector, sizeof (Token),free_token, DEF_VECTOR_SIZE);
	getTokensFromText(text, &tokenVector);
	for (i=0;i<tokenVector.ItemsCount;i++) {
		token = (Token*)VectorNth(&tokenVector,i);
		sprintf( t, "%s", token->term);
		t+=strlen(token->term);
		if (i!=tokenVector.ItemsCount)
			*t++=' ';
	} 
	*t='\0';
	extract_address(tokens);
	VectorDispose(&tokenVector);

	free (text);
	free (tokens);


	return 0;

}

