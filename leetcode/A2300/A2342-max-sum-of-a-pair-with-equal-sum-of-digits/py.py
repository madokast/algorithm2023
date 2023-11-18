from typing import List
import array

class Solution:
    def maximumSum(self, nums: List[int]) -> int:
        def digitSum(num:int) -> int:
            s = 0
            while num != 0 :
                s += (num % 10)
                num //= 10
            return s
        
        m = -1
        slots = [0] * (9*9)
        slots.clear()
        for num in nums:
            s = digitSum(num)
            if slots[s] == 0:
                slots[s] = num
            else:
                m = max(m, num + slots[s])
                slots[s] = max(slots[s], num)      
        return m
    
if __name__ == '__main__':
    s = Solution()
    print(s.maximumSum([18,43,36,13,7]))
    print(s.maximumSum([10,12,19,14]))
    print(s.maximumSum([4]))
