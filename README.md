# BagOfWords
The bag-of-words model is a simplifying representation used in natural language processing and information retrieval (IR). 


Text representation with bag-of-words (BOW) approach 
 
 
Traditional representation methodology of documents in the literature is called Bag of Words (BOW) feature representation.
This representation symbolizes the terms and their corresponding frequencies in a document and it is also named as Vector Space Model (VSM).
Each of these terms in the same document represents an independent dimension in a vector space [1].
The order of words in the sentences is completely lost in bag representation like in sets. 
This approach mainly emphasizes the frequency of terms. The BOW methodology makes the representation of words simpler in documents; still; it has several problems. One of them is sparse vector representation. This makes the computation expensive especially for real world scenarios, which include big data in textual domains. To address this problem; this project aims to create more a more efficient representation of BOW model by using LinkedLists. Consequently, this project is a programming assignment in C, which aims to build an algorithm based on linked-lists that will build an efficient representation of documents. 
This program opens and reads text files under the following directories: sport, magazine and health. The number of documents in these categories will be arbitrary. Furthermore, the number of terms in these documents are also be arbitrary.  In other words, the length of these files are arbitrary
