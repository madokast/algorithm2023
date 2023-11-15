from typing import List

class Solution:
    def maxProduct(self, words: List[str]) -> int:
        ret = 0
        bit2len = dict()
        for i in range(len(words)):
            bitWord = 0
            for w in bytearray(words[i], 'ascii'):
                bitWord |= (1 << (w - 97))
            bit2len[bitWord] = max(bit2len.get(bitWord, 0), len(words[i]))
        for bit, le in bit2len.items():
            for bit2, le2 in bit2len.items():
                if (bit & bit2) == 0:
                    ret = max(ret, le * le2)
        return ret

    def maxProduct2(self, words: List[str]) -> int:
        ret = 0
        bitWords = [0 for _ in words]
        wordLengths = [0 for _ in words]
        for i in range(len(words)):
            for w in bytearray(words[i], 'ascii'):
                bitWords[i] |= (1 << (w - 97))
            wordLengths[i] = len(words[0])
        for i in range(len(words)):
            for j in range(i, len(words)):
                if (bitWords[i] & bitWords[j]) == 0:
                    ret = max(ret, len(words[i]) * len(words[j]))
        return ret
                
    def maxProduct1(self, words: List[str]) -> int:
        ret = 0
        letterSets = [set(l for l in w) for w in words]
        for i in range(len(words)):
            for j in range(i, len(words)):
                if len(words[i]) * len(words[j]) > ret and len(letterSets[i] & letterSets[j]) == 0:
                    ret = max(ret, len(words[i]) * len(words[j]))
        return ret
    
if __name__ == '__main__':
    s = Solution()
    print(s.maxProduct(["abcw","baz","foo","bar","xtfn","abcdef"]))