from typing import List
import bisect
import functools

class Solution:
    def maximumSumQueries(self, nums1: List[int], nums2: List[int], queries: List[List[int]]) -> List[int]:
        table = [(nums1[i]+nums2[i], nums1[i], nums2[i]) for i in range(len(nums1))]
        tableBySum = sorted(table, key=lambda t:-t[0])

        @functools.cache
        def query(x:int, y:int) -> int:
            for item in tableBySum:
                if item[1] >= x and item[2] >= y:
                    return item[0]
            return -1

        return [query(q[0], q[1]) for q in queries]
    
    # 超时
    def maximumSumQueries_slow(self, nums1: List[int], nums2: List[int], queries: List[List[int]]) -> List[int]:
        table = [(nums1[i], nums2[i], nums1[i]+nums2[i]) for i in range(len(nums1))]
        tableBy1 = sorted(table, key=lambda t:t[0])
        tableBy2 = sorted(table, key=lambda t:t[1])

        answer = []
        for query in queries:
            x, y = query[0], query[1]
            bound1 = bisect.bisect_right(tableBy1, x-1, key=lambda t:t[0])
            bound2 = bisect.bisect_right(tableBy2, y-1, key=lambda t:t[1])
            if bound1 > bound2:
                max_sum = max((t[2] for t in tableBy1[bound1:] if t[1]>=y), default=-1)
            else:
                max_sum = max((t[2] for t in tableBy2[bound2:] if t[0]>=x), default=-1)
            answer.append(max_sum)
        return answer

        

if __name__ == '__main__':
    s = Solution()
    print(s.maximumSumQueries(nums1 = [4,3,1,2], nums2 = [2,4,9,5], queries = [[4,1],[1,3],[2,5]]))
