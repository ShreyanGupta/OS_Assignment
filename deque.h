#ifndef DEQUE_H
#define DEQUE_H

#define SIZE 24

struct Deque{
	int s = 0;
	char arr[SIZE][160];
	int f = 0;
	int b = 0;
	int size() const;
	char *get_i(int);
	const char *read_i(int) const;
	void push(char *);
	void clear();
};

// inline Deque::Deque(){
// 	// for(int i=0; i<SIZE; ++i) arr[i] = NULL;
// 	s = f = b = 0;
// }

inline int Deque::size() const{
	// if(f == b) return 0;
	// return (b-f+SIZE)%SIZE;
	return s;
}

inline char *Deque::get_i(int i){
	return arr[(b-i-1+SIZE)%SIZE];
}

inline const char *Deque::read_i(int i) const{
	return (arr[(b-i-1+SIZE)%SIZE]);
}

inline void Deque::push(char *c){
	if(s == SIZE){
		// free(arr[f]);
		f = (f+1)%SIZE;
		--s;
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
	++s;
}

inline void Deque::clear(){
	s = f = b = 0;
}

#endif