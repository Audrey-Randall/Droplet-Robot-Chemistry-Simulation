#ifndef VECTOR_H__
#define VECTOR_H__

typedef struct vector_ {
	void** data;
	uint8_t size;
	uint8_t count;
} vector;

void vector_init(vector*);
uint8_t vector_count(vector*);
void vector_add(vector*, void*);
void vector_set(vector*, uint8_t, void*);
void *vector_get(vector*, uint8_t);
void vector_delete(vector*, uint8_t);
void vector_free(vector*);

#endif