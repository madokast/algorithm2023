from typing import List

class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        m = dict()
        for i in range(len(nums)):
            j = m.get(target- nums[i])
            if j is not None:
                return [j, i]
            m[nums[i]] = i


if __name__ == '__main__':
    s = Solution()
    assert s.twoSum([3, 2, 4], 6) == [1, 2]