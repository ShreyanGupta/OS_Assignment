#ifndef DEQUE_H
#define DEQUE_H

#define SIZE 25

struct Deque{
	int s;
	char arr[SIZE][160];
	int f;
	int b;
	Deque();
	int size();
	char *get_i(int);
	void push(char *);
};

Deque::Deque(){
	// for(int i=0; i<SIZE; ++i) arr[i] = NULL;
	s = f = b = 0;
}

int Deque::size(){
	// if(f == b) return 0;
	// return (b-f+SIZE)%SIZE;
	return s;
}

char *Deque::get_i(int i){
	return arr[(b-i-1+SIZE)%SIZE];
}

void Deque::push(char *c){
	if(s == SIZE){
		// free(arr[f]);
		f = (f+1)%SIZE;
	}
	// arr[b] = (char *)malloc(strlen(c)+1);
	int i=0;
	while(*(c+i) != '\0'){
		arr[b][i] = c[i];
		++i;
	}
	arr[b][i] = '\0';
	// for(int i=0; i<strlen(c); ++i) arr[b][i] = c[i];
	// arr[b][strlen(c)+1] = '\0';
	b = (b+1)%SIZE;
}

#endif