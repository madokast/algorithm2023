package main

import (
	"runtime"
	"sync"
	"sync/atomic"
	"time"
)

type Node int // 节点

// Children 获取子节点
func (n Node) Children() []Node {
	time.Sleep(1 * time.Millisecond)
	if n > 0 {
		c := make([]Node, 20)
		for i := range c {
			c[i] = Node(n - 1)
		}
		return c
	} else {
		return nil
	}

}

type Stack struct {
	workerNumber int
	data         [][]Node
	numbers      []int
	mutexes      []sync.RWMutex
}

func newStack(workerNumber int) *Stack {
	searchIndexes := make([][]int, workerNumber)
	for wi := 0; wi < workerNumber; wi++ {
		searchIndex := make([]int, workerNumber)
		for i := 0; i < workerNumber; i++ {
			searchIndex[i] = (wi + i) % workerNumber
		}
		searchIndexes[wi] = searchIndex
	}
	return &Stack{
		workerNumber: workerNumber,
		data:         make([][]Node, workerNumber),
		numbers:      make([]int, workerNumber),
		mutexes:      make([]sync.RWMutex, workerNumber),
	}
}

func (s *Stack) pop(cid int) (top Node, ok bool) { // 并发安全
	for i := 0; i < s.workerNumber; i++ {
		id := (i + cid) % s.workerNumber
		s.mutexes[id].Lock()
		size := len(s.data[id])
		if size > 0 {
			top = s.data[id][size-1]
			s.data[id] = s.data[id][:size-1]
			s.numbers[id]--
			s.mutexes[id].Unlock()
			return top, true
		} else {
			s.mutexes[id].Unlock()
		}
	}

	// 没有任务
	return top, false
}

func (s *Stack) empty() bool {
	for i := 0; i < s.workerNumber; i++ {
		s.mutexes[i].RLock()
		if s.numbers[i] > 0 {
			return false
		}
		s.mutexes[i].RUnlock()
	}
	return true
}

func (s *Stack) push(nodes ...Node) { // 并发安全
	size := len(nodes)
	if size == 0 {
		return
	}
	batch := (size + s.workerNumber - 1) / s.workerNumber
	for i := 0; i < s.workerNumber; i++ {
		start := batch * i
		if start >= size {
			break
		}
		end := batch * (i + 1)
		if end > size {
			end = size
		}
		number := start - end
		s.mutexes[i].Lock()
		s.data[i] = append(s.data[i], nodes[start:end]...)
		s.numbers[i] += number
		s.mutexes[i].Unlock()
	}
}

func search(root Node) int32 {

	var waitCond = sync.NewCond(&sync.Mutex{}) // 条件变量
	var mainCond = sync.NewCond(&sync.Mutex{}) // 主协程等待一个协程结束
	var mainShouldWait = true                  // 主协程是否应该等待
	var runningNumber atomic.Int32             // 正在往下搜索的协程数目。如果数目为0且栈为空，则说明搜索结束
	var parallel = runtime.NumCPU() + 1        // 并发度

	var stack = newStack(parallel)
	stack.push(root)

	var nodeNumber atomic.Int32

	for i := 0; i < parallel; i++ { // 启动 parallel 个协程并发搜索
		go func(i int) {
		startRuning:
			runningNumber.Add(1)
		takeLoop:
			top, ok := stack.pop(i) // 出栈一个
			if ok {
				nodeNumber.Add(1)
				stack.push(top.Children()...) // 拿到子节点入栈
				waitCond.Broadcast()          // 唤醒所有节点
				goto takeLoop
			} else {
				waitCond.L.Lock()
				if runningNumber.Add(-1) == 0 && stack.empty() {
					waitCond.L.Unlock()
					waitCond.Signal() // 唤醒一个子协程
					{
						mainCond.L.Lock() // 唤醒主协程
						if mainShouldWait {
							mainShouldWait = false
						}
						mainCond.Signal()
						mainCond.L.Unlock()
					}
					return
				}
				waitCond.Wait()
				waitCond.L.Unlock()
				goto startRuning
			}

		}(i)
	}
	mainCond.L.Lock()
	if mainShouldWait {
		mainCond.Wait()
	}
	mainCond.L.Unlock()

	return nodeNumber.Load()
}

func main() {
	s := time.Now()
	for i := 0; i < 5; i++ {
		println("done", i, search(Node(3)))
	}
	println("tiem", int(time.Since(s).Seconds()), "s")
}
