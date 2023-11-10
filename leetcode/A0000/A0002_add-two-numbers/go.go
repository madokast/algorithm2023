package main

import (
	"algorithm/utils/golang"
	"fmt"
)

type ListNode = golang.ListNode[int]

func addTwoNumbers(l1 *ListNode, l2 *ListNode) *ListNode {
	head := ListNode{}
	cur := &head
	carry := 0
	for l1 != nil && l2 != nil {
		sum := l1.Val + l2.Val + carry
		if sum >= 10 {
			sum -= 10
			carry = 1
		} else {
			carry = 0
		}
		cur.Next = &ListNode{Val: sum}
		cur = cur.Next

		l1 = l1.Next
		l2 = l2.Next
	}

	if l2 != nil {
		l1 = l2
	}

	for l1 != nil {
		sum := l1.Val + carry
		if sum >= 10 {
			sum -= 10
			carry = 1
		} else {
			carry = 0
		}
		cur.Next = &ListNode{Val: sum}
		cur = cur.Next

		l1 = l1.Next
	}

	if carry == 1 {
		cur.Next = &ListNode{Val: 1}
	}

	return head.Next
}

func main() {
	fmt.Printf("%v\n", addTwoNumbers(golang.MakeSinglyLinkedList[int](0),
		golang.MakeSinglyLinkedList[int](0)).ToSlice())
	fmt.Printf("%v\n", addTwoNumbers(golang.MakeSinglyLinkedList[int](2, 4, 3),
		golang.MakeSinglyLinkedList[int](5, 6, 4)).ToSlice())
	fmt.Printf("%v\n", addTwoNumbers(golang.MakeSinglyLinkedList[int](9, 9, 9, 9, 9, 9, 9),
		golang.MakeSinglyLinkedList[int](9, 9, 9, 9)).ToSlice())
}
