#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../header/hash_map.h"
#include <limits.h>

int* get_int_ptr(int value){
	int* temp = malloc(sizeof(int));
	*temp = value;
	return temp;
}
long long int* get_long_ptr(long long int value){
	long long int* temp = malloc(sizeof(long long int));
	*temp = value;
	return temp;
}
int int_hash(void *v){
	return *((int*)v);
}

long long int fib(long long int n, HashMap *map){
	if(n == 0){
		return 0;
	}
	else if(n == 1){
		return 1;
	}
	else if(hash_map_contains(&n, map)==1){
		return *((long long int *)hash_map_get(&n, map));
	}
	else{
		long long int fib_val = fib(n-1, map)+fib(n-2, map);
		long long int *value = get_long_ptr(fib_val);
		hash_map_add(&n, value, map);
		return fib_val;
	}
}

long long int shitinacci(long long int n){
	if(n == 0){
		return 0;
	}
	else if(n == 1){
		return 1;
	}
	else{
		return shitinacci(n-1)+shitinacci(n-2);
	}
}


int main(){
	long long int x = 9223372036854775807;
	printf("%lld\n", x);
	printf("%lld\n", LLONG_MAX);
	HashMap *map = hash_map_constructor(1000, &int_hash);
	printf("%lld\n", fib(45, map));
	printf("%lld\n", shitinacci(45));	
}
