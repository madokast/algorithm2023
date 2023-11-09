#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define Key int
#define Value int
#define true 1
#define false 0

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
    m->data = (entry*)malloc((size + number) * sizeof(entry));
    memset(m->data, 0, (size + number) * sizeof(entry));
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

int main() {
    iimap m;
    m_init(&m, 10);
    m_direct_put(&m, 0, 100);
    m_direct_put(&m, 3, 300);
    m_direct_put(&m, 5, 500);
    
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
    m_destory(&m);
}