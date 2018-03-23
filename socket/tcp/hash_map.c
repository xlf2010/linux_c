#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "hash_map.h"
#define DEFAULT_SIZE 100

volatile static int array_size=DEFAULT_SIZE;

int map_init(map_t* map){
	entry_t** ent = (entry_t **)malloc(sizeof(entry_t *)*array_size);
	if(ent==NULL){
		return -1;
	}
		
//	memset(ent,0,sizeof(ent));
	map->ent=ent;
	map->size=0;
	return 0;
}

int map_put(map_t* map,int key,int value){
	int position = key % array_size;
	entry_t* e=(entry_t *)malloc(sizeof(entry_t));
	e->key=key;
	e->value=value;
	e->next=NULL;
	if(map->ent[position]==NULL){
		map->ent[position]=e;
	}else{
		entry_t* et=map->ent[position];
		while(et->next!=NULL){
			et=et->next;
		}
		et->next=e;
	}
	map->size++;
	return 0;
}

int map_get(map_t *map,int key,int *value){
	int position = key % array_size;
	entry_t* et=map->ent[position];
	if(et==NULL){
		return -1;
	}
	do{
		if(et->key==key){
			*value=et->value;
			return 0;
		}
	}while((et=et->next)!=NULL);
	return -2;
}

int map_del(map_t *map,int key){
	int position=key % array_size;
	entry_t* et=map->ent[position];
	if(et==NULL){
		return -1;
	}
	// remove head node;
	if(et->key==key){
		map->ent[position]=et->next;
		free(et);
		map->size--;
		return 0;
	}
	entry_t* nt=et->next;
	do{
		if(nt->key==key){
			et->next=nt->next;
			free(nt);
			map->size--;
			return 0;
		}
		et=nt;
	}while((nt=nt->next)!=NULL);
	return -2;	
}

int map_size(map_t* map){
	return map->size;
}

int map_destory(map_t* map){
	entry_t** ent=map->ent;
	int i=0;
	for(i=0;i<array_size;i++){
		entry_t* et=*ent;
		while(et!=NULL){
			entry_t *t=et;
			et=et->next;
			free(t);
		}
		ent++;
	}
	free(map);
	return 0;
}
int main(int argc,char **argv){
	map_t map;
	map_init(&map);
	map_put(&map,1,2);
	map_put(&map,1+array_size,3);
	int val;
	map_get(&map,1+array_size,&val);
	printf("get from map key=1,value=%d\n",val);
	map_del(&map,101);
	val=0;
	map_get(&map,1+array_size,&val);
	printf("get from map key=1,value=%d\n",val);
}
