#include<stdlib.h>
#include<string.h>
#include<stdio.h>

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
    int size = 1;
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
    } else if (next == -1) {
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

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;
    int* r = (int*)malloc(2 * sizeof(int));
    iimap m;
    m_init(&m, numsSize);
    Value* val = NULL;
    for (size_t i = 0; i < numsSize; i++) {
        m_get(&m, target - nums[i], &val);
        if (val != NULL) {
            r[0] = *val;
            r[1] = i;
            m_destory(&m);
            return r;
        }
        m_direct_put(&m, nums[i], i);
    }

    exit(-1);
}

int main() {
    int nums[] = {3, 2, 4};
    int returnSize;
    int* ans = twoSum(nums, 3, 6, &returnSize);
    printf("%d %d\n", ans[0], ans[1]);
    free(ans);
}