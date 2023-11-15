#include<stdio.h>
#include<string.h>
#include <stdlib.h>

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


int m = 0;
iimap map;
int outbitWord, outlength;
void foreach_inner(int bitWord, int length) {
    int t;
    if ((outbitWord & bitWord) == 0) {
        t = outlength * length;
        if (t > m) m = t;
    }
}

void foreach_outer(int bitWord, int length) {
    outbitWord = bitWord;
    outlength = length;
    m_foreach(&map, foreach_inner);
}

int maxProduct(char ** words, int wordsSize) {
    int* lengthValue;
    char* word;
    char c;
    int bitWord, length;

    m = 0;
    m_init(&map, wordsSize);

    for (int i = 0; i < wordsSize; ++i) {
        word = words[i];
        bitWord = 0;
        while (1) {
            c = *word;
            if (c == '\0') break;
            else bitWord |= (1 << (c - 'a'));
            ++word;
        }
        length = word - words[i];

        lengthValue = NULL;
        m_get(&map, bitWord, &lengthValue);
        if (lengthValue == NULL) {
            m_direct_put(&map, bitWord, length);
        } else if (length > *lengthValue) {
            m_put(&map, bitWord, length);
        }
    }

    m_foreach(&map, foreach_outer);
    return m;
}

int main() {
    char* words[] = {"ccd","accaceddeeeaefc","bcaffa","bbcfafbb","accacfebbabbeedfbfdb","beddecbffcdaededdaefdedfdea","cf","ddafdcbd","bbafacebacaefdaffccebddff","ebccffcddbeddccacceccaec","becfbfdccdfdeadfbfaddbcded","cbabeaaeabefedbaeaedc","dfadbbdbead","cafaefdcd","eccdbfceafeeeacfcddc","dbabbcdbb","abfbfbffcbebde","cfaadaa","fc","faebcabb","adbacebabcaaccbdeaffff","aeaefccf","dbacbeeabdbcdfccabebaecfef","ecdadeefcaddffaececffa","defcabf","abbcecbccbdaebaecaefabed","dfeeebcbaaefc","aecccbcbbdddb","dcfabacec","fccfbacbacddeaaea","dfdbfacbacbecb","cbfeebdbfecb","cffaacacbde","aafd","bdcebbbebd","afeffadcfcdacfba","dafeefbcdfaffcfacee","dcbbebfbedafedcdbab","cafaf","bcbcccfdebdd","efaaaacccff","cffbead","ebcfccfcddffdec","fffdfdcec","beeafefbdfa","cdfdbccfbaaeffcabab","ddadcbabbcb","decfaeabbecebaebeaddedae","cdcbfffbebae","aeccefcbcbbddfdc","ffefedaf","cddbabccafaffeafeedcbedbdad","eddeeccfedcefadfdfebfacb","aca","ffdcafaddcddf","ef","bbbbffe","ffccfebabaadcffacbbb","cbdeddfddffacbeeeebafebabda","ddeecb","cffdc","edcffcebadf","becbcadcafddcfbbeeddbfffcab","abcbaceeaeaddd","cfeffceebfaeefadaaccfa","eaccddb","caeafbfafecd","becaafdbaadbfecfdfde","ecabaaeafbfbcbadaac","bdcdffcfaeebeedfdfddfaf","dbbfbaeecbfcdebad","cceecddeeecdbde","beec","adbcfdbfdbccdcffffbcffbec","bbbbfe","cdaedaeaad","dadbfeafadd","fcacaaebcedfbfbcddfc","ceecfedceac","dada","ccfdaeffbcfcc","eadddbbbdfa","beb","fcaaedadabbbeacabefdabe","dfcddeeffbeec","defbdbeffebfceaedffbfee","cffadadfbaebfdbadebc","fbbadfccbeffbdeabecc","bdabbffeefeccb","bdeeddc","afcbacdeefbcecff","cfeaebbbadacbced","edfddfedbcfecfedb","faed","cbcdccfcbdebabc","efb","dbddadfcddbd","fbaefdfebeeacbdfbdcdddcbefc","cbbfaccdbffde","adbcabaffebdffad"};
    printf("%d\n", maxProduct(words, sizeof(words)/sizeof(char*)));

    // int a = 1, b = 2;
    // printf("a & b = %d\n", a & b);
    // printf("a & b == 0 = %d\n", (a & b) == 0);
    // printf("!(a & b) %d\n", !(a & b));
}