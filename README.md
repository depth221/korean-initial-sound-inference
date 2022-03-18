# korean-initial-sound-inference
Reconstruct the original Korean sentence which has been converted to a set of initial sounds

초성체로 구성된 구 또는 문장을 입력하면 가능한 원본 문자열의 집합을 반환하는 프로그램입니다.

## 구조
### 입력값
입력값은 크게 **단어**와 **문장**으로 분류합니다.
1. **(명사)**는 대명사와 수사를 포함합니다.
2. **(서술어)**는 형용사와 동사, '**(명사)**이다'를 포함합니다.

#### 단어
단어는 다음 구조로 이루어집니다.
* **(명사)** (:= **(명사구)**)
* **(명사구)** **(명사)**
* **(명사구)**의 **(명사)**

#### 문장
* **(명사구)****(조사)** **(명사구)****(조사)** **(서술어)**
