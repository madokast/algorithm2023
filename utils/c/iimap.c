#include<stdlib.h>

#define Key int
#define Value int

typedef struct _entry {
    Key key;
    Value value;
    int next;
} entry;


typedef struct _iimap {
    int free;
    int mask;
    entry* data; // entry[]
} iimap;

void m_init(iimap* m, int number) {
    int size = 16;
    while (size < number) {
        size <<= 2;
    }
    m->free = size;
    m->mask = size - 1;
    m->data = (entry*)calloc(size + number, sizeof(entry));
    // memset(m->data, 0, (size + number) * sizeof(entry));
}

void m_destory(iimap* m) {
    free(m->data);
}

void m_put(iimap* m, Key k, Value v) {
    int loc = k & m->mask;
    int next = m->data[loc].next;
    if (next == 0) { // empty
        m->data[loc].key = k;
		m->data[loc].next = -1;
		m->data[loc].value = v;
    } else if (next == -1) { // only one, no link
        if (m->data[loc].key == k) {
            m->data[loc].value = v;
        } else { // add link
            m->data[loc].next = m->free;
			m->data[m->free].key = k;
			m->data[m->free].next = -1; // end
			m->data[m->free].value = v;
			m->free++;
        }
    } else { // has link
        if (m->data[loc].key == k) {
            m->data[loc].value = v;
        } else {
            for (;;) {
                if (m->data[next].key == k) {
					m->data[next].value = v;
					return;
				}
				if (m->data[next].next == -1) { // end
					m->data[next].next = m->free;
					m->data[m->free].key = k;
					m->data[m->free].next = -1; // end
					m->data[m->free].value = v;
					m->free++;
					return;
				}
				next = m->data[next].next;
            }
        }
    }
}

void m_direct_put(iimap* m, Key k, Value v) {
    int loc = k & m->mask;
    int next = m->data[loc].next;
    if (next == 0) { // empty
        m->data[loc].key = k;
		m->data[loc].next = -1;
		m->data[loc].value = v;
    } else if (next == -1) { // only one, no link
        m->data[loc].next = m->free;
        m->data[m->free].key = k;
        m->data[m->free].next = -1; // end
        m->data[m->free].value = v;
        m->free++;
    } else { // has link
        for (;;) {
            if (m->data[next].next == -1) { // end
                m->data[next].next = m->free;
                m->data[m->free].key = k;
                m->data[m->free].next = -1; // end
                m->data[m->free].value = v;
                m->free++;
                return;
            }
            next = m->data[next].next;
        }
    }
}

void m_get(iimap* m, Key k, Value** val) {
    int loc = k & m->mask;
    int next = m->data[loc].next;
    if (next == 0) {
        return;
    } if (next == -1) {
        if (m->data[loc].key == k) {
            *val = &(m->data[loc].value);
            return;
        }
    } else {
        if (m->data[loc].key == k) {
            *val = &(m->data[loc].value);
            return;
        } else {
            for (;;) {
				if (m->data[next].key == k) {
                    *val = &(m->data[next].value);
					return;
				}
				if (m->data[next].next == -1) {
					return;
				}
				next = m->data[next].next;
			}
        }
    }
}

void m_foreach(iimap* m, void (*fun)(Key k, Value v)) {
    entry* e;
    int nx;
    for (int i = 0; i <= m->mask; ++i) {
        e = m->data + i;
        nx = e->next;
        if (nx != 0) {
            fun(e->key, e->value);
            while (nx != -1) {
                e = m->data + nx;
                fun(e->key, e->value);
                nx = e->next;
            }
        }
    }
}

void m_foreach_ctx(iimap* m, void* context, void (*fun)(Key k, Value v, void* context)) {
    entry* e;
    int nx;
    for (int i = 0; i <= m->mask; ++i) {
        e = m->data + i;
        nx = e->next;
        if (nx != 0) {
            fun(e->key, e->value, context);
            while (nx != -1) {
                e = m->data + nx;
                fun(e->key, e->value, context);
                nx = e->next;
            }
        }
    }
}

#include<stdio.h>

void _foreach(Key k, Value v) {printf("%d->%d\n", k, v);}
void _sumkey(Key k, Value v, void* ctx) {*((int*)(ctx))+=k;}

int main() {
    iimap m;
    m_init(&m, 10);
    for (int i = 0; i < 20 ; ++i) m_direct_put(&m, i, i*100);
    
    Value* val = NULL;
    for (Key i = 0; i <= 8; i++) {
        m_get(&m, i, &val);
        if (val != NULL) {
            printf("find %d->%d\n", i, *val);
            val = NULL;
        } else {
            printf("no key %d in map\n", i);
        }
    }

    m_foreach(&m, _foreach);

    int sum = 0;
    m_foreach_ctx(&m, &sum, _sumkey);
    printf("sum key = %d\n", sum);

    m_destory(&m);
}