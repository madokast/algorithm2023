package golang

type ListNode[E any] struct {
	Val  E
	Next *ListNode[E]
}

func (l *ListNode[E]) ToSlice() []E {
	var r []E
	for l != nil {
		r = append(r, l.Val)
		l = l.Next
	}
	return r
}

func MakeSinglyLinkedList[E any](es ...E) *ListNode[E] {
	head := ListNode[E]{}
	cur := &head
	for _, v := range es {
		cur.Next = &ListNode[E]{Val: v}
		cur = cur.Next
	}

	return head.Next
}
