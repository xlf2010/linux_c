#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct ent{
	int key;
	int value;
	struct ent* next;
}entry_t;

typedef struct{
	entry_t** ent;
	int size;
}map_t;

extern int map_init(map_t* map);

// put key value to map,this function will return 0 if put success
extern int map_put(map_t* map,int key,int value);

//get value by key,will return by point value ,this function will return 0 if get success;
extern int map_get(map_t* map,int key,int* value);

// delete key value from map
extern int map_del(map_t* map,int key);

//get map sizes,will return by point value ,this function will return 0 if success;
extern int map_size(map_t* map);

// destory map for reuse,this function will return 0 if success;
extern int map_destory(map_t* map);

#endif
