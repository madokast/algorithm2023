from typing import List

vowels = {'a','e','i','o','u'}

class Solution:
    def vowelStrings(self, words: List[str], left: int, right: int) -> int:
        return sum(1 for w in words[left:right+1] if w[0] in vowels and w[-1] in vowels)
    
if __name__ == '__main__':
    s = Solution()
    print(s.vowelStrings(words = ["are","amy","u"], left = 0, right = 2))
    print(s.vowelStrings(words = ["hey","aeo","mu","ooo","artro"], left = 1, right = 4))