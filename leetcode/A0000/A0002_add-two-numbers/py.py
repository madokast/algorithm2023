from typing import Optional

class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next
    def to_list(self):
        r = []
        while self is not None:
            r.append(self.val)
            self = self.next
        return r
    @staticmethod
    def from_list(li):
        head = ListNode()
        cur = head
        for e in li:
            cur.next = ListNode(e)
            cur = cur.next
        return head.next

class Solution:
    def addTwoNumbers(self, l1: Optional[ListNode], l2: Optional[ListNode]) -> Optional[ListNode]:
        ret = ListNode()
        cur = ret
        carry = 0
        sum = 0
        while l1 is not None and l2 is not None:
            sum = l1.val + l2.val + carry
            if sum >= 10:
                sum -= 10
                carry = 1
            else:
                carry = 0
            cur.next = ListNode(val = sum)
            cur = cur.next

            l1 = l1.next
            l2 = l2.next
        
        if l2 is not None:
            l1 = l2

        while l1 is not None:
            sum = l1.val + carry
            if sum >= 10:
                sum -= 10
                carry = 1
            else:
                carry = 0
            cur.next = ListNode(val = sum)
            cur = cur.next

            l1 = l1.next
        
        if carry == 1:
            cur.next = ListNode(val = carry)
        
        return ret.next
    
if __name__ == '__main__':
    s = Solution()
    print(s.addTwoNumbers(ListNode.from_list([2,4,3]), ListNode.from_list([5,6,4])).to_list())
    print(s.addTwoNumbers(ListNode.from_list([0]), ListNode.from_list([0])).to_list())
    print(s.addTwoNumbers(ListNode.from_list([9]*7), ListNode.from_list([9]*4)).to_list())