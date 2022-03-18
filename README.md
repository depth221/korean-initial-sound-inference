# korean-initial-sound-inference
Reconstruct the original Korean sentence which has been converted to a set of initial sounds

초성체로 구성된 구 또는 문장을 입력하면 가능한 원본 문자열의 집합을 반환하는 프로그램입니다.

## 구조
### 입력값
입력값은 크게 **단어**와 **문장**으로 분류합니다.
1. (명사)는 대명사와 수사를 포함합니다.
2. (서술어)는 형용사와 동사의 종결형, '(명사)이다'를 포함합니다.
3. (형용사구)는 형용사와 동사의 'ㄴ' 활용, '(명사구)의'를 포함합니다.
4. (부사구)는 부사, 형용사와 동사의 '게' 활용, '(명사구)에'를 포함합니다.

#### 단어
단어는 다음 구조로 이루어집니다.
* (명사) 또는 (명사구) (명사) (:= (명사구))
* (형용사구) (명사구)
* (부사구) (형용사구)(명사구)

#### 문장

* (단어)(조사) (서술어) (주어 + 서술어가 가능한 서술어의 경우)
* (단어)은/는/이/가 (단어)을/를 ((부사구)) (서술어) (주어 + 목적어 + 서술어가 가능한 서술어의 경우)   
(단어)을/를 (단어)은/는/이/가 ((부사구)) (서술어) (주어 + 목적어 + 서술어가 가능한 서술어의 경우)
