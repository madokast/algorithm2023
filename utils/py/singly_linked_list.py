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
    
if __name__ == '__main__':
    li = ListNode.from_list([1,2,3,4])
    print(li.to_list())
