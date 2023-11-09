package main

import (
	"algorithm/utils/golang"
)

func twoSum(nums []int, target int) []int {
	m := make(map[int]int, len(nums))
	for i, n := range nums {
		if j, ok := m[target-n]; ok {
			return []int{j, i}
		}
		m[n] = i
	}
	panic("no solution")
}

func main() {
	golang.AssertEqual(twoSum([]int{2, 7, 11, 15}, 9), []int{0, 1})
	golang.AssertEqual(twoSum([]int{3, 3}, 6), []int{0, 1})
	golang.AssertEqual(twoSum([]int{3, 2, 4}, 6), []int{1, 2})
}
