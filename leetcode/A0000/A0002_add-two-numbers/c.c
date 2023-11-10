#include<stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

#define NULL 0

struct ListNode dummy;
struct ListNode pool[101];
int free;

struct ListNode* new_list(int val) {
    pool[free].val = val;
    return &pool[free++];
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode* cur = &dummy;
    int sum, carry = 0;
    while (l1 != NULL || l2 != NULL || carry > 0) {
        sum = (l1 == NULL ? 0 : l1->val) + (l2 == NULL ? 0 : l2->val) + carry;
        carry = sum / 10;
        cur = cur->next = new_list(sum % 10);
        l1 = (l1 == NULL ? NULL : l1->next);
        l2 = (l2 == NULL ? NULL : l2->next);
    }
    cur->next = NULL;
    return dummy.next;
}